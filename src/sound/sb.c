#include "sb.h"

sb_t sb;
volatile bool sb_playing = false;
static uint8_t *sb_dma_buffer = NULL;
static _go32_dpmi_seginfo prev_handler, new_handler;
static _go32_dpmi_seginfo dma_buffer;
static uint32_t dma_offset = 0;

/**
 * Este es el manejador de IRQ.
 */
void sb_irq_handler()
{
  inportb(sb.base + SB_PORT_DSP_READ_STATUS);
  sb_playing = false;
  outportb(0x20, 0x20);
}

/**
 * Reiniciamos el DSP de la Sound Blaster.
 */
bool sb_dsp_reset(uint16_t port)
{
  outportb(port + SB_PORT_DSP_RESET, 1);
  delay(10);
  outportb(port + SB_PORT_DSP_RESET, 0);
  delay(10);
  if (
      (inportb(port + SB_PORT_DSP_READ_STATUS) & SB_STATUS_DATA_AVAILABLE) == SB_STATUS_DATA_AVAILABLE &&
      inportb(port + SB_PORT_DSP_READ) == SB_STATUS_RESET_OK)
  {
    return true;
  }
  return false;
}

/**
 * Leemos datos del DSP.
 */
uint8_t sb_dsp_read()
{
  while ((inportb(sb.base + SB_PORT_DSP_READ_STATUS) & SB_STATUS_DATA_AVAILABLE) != SB_STATUS_DATA_AVAILABLE);
  return inportb(sb.base + SB_PORT_DSP_READ);
}

/**
 * Escribimos datos en el DSP.
 */
void sb_dsp_write(uint8_t command)
{
  while ((inportb(sb.base + SB_PORT_DSP_WRITE) & SB_STATUS_DATA_AVAILABLE) == SB_STATUS_DATA_AVAILABLE);
  outportb(sb.base + SB_PORT_DSP_WRITE, command);
}

/**
 *
 */
void sb_set_base_from_hex(uint16_t base) {
  sb.base = 0x200 + (((base - 200) / 10) << 4);
}

void sb_irq_init() {
  _go32_dpmi_get_protected_mode_interrupt_vector(sb.irq + 8, &prev_handler);
  new_handler.pm_offset = (int32_t)sb_irq_handler;
  new_handler.pm_selector = _go32_my_cs();
  _go32_dpmi_chain_protected_mode_interrupt_vector(sb.irq + 8, &new_handler);
  outportb(0x21, inportb(0x21) & ~(1 << sb.irq));
}

void sb_irq_restore() {
  _go32_dpmi_set_protected_mode_interrupt_vector(sb.irq, &prev_handler);
  outportb(0x21, inportb(0x21) & (1 << sb.irq));
}

void sb_dma_init() {
  _go32_dpmi_seginfo temp; // Temporary pointer
  uint32_t page1, page2;  // Words

  // Assign 32K to DMA Buffer
  sb_dma_buffer = (uint8_t*)malloc(SB_DMA_BUFFER_SIZE);

  // Assign 32K (2048 paragraphs) of DOS memory
  temp.size = 2048;
  _go32_dpmi_allocate_dos_memory(&temp);

  // Calculate linear address
  dma_offset = temp.rm_segment << 4;

  // Calculate page at start of buffer
  page1 = dma_offset >> 16;

  // Calculate page at end of buffer}
  page2 = (dma_offset + (SB_DMA_BUFFER_SIZE - 1)) >> 16;

  // Check to see if a page boundary is crossed
  if (page1 != page2)
  {
    // If so, assign another part of memory to the buffer
    dma_buffer.size = 2048;
    _go32_dpmi_allocate_dos_memory(&dma_buffer);
    dma_offset = dma_buffer.rm_segment << 4;
    _go32_dpmi_free_dos_memory(&temp);
  }
  else // otherwise, use the part we've already allocated
    dma_buffer = temp;

  // Clear DMA buffers
  memset(sb_dma_buffer, 128, 0x7FFF);
  dosmemput(sb_dma_buffer, SB_DMA_BUFFER_SIZE, dma_offset);
}

/**
 * Reproduce un archivo de sonido utilizando el DSP de la Sound Blaster
 */
bool sb_play_file(const char* filename) {
  // Leemos el archivo de sonido.
  FILE* file = fopen(filename, "rb");
  if (file == NULL)
    return false;

  fread(sb_dma_buffer, 1, SB_DMA_BUFFER_SIZE, file);
  fclose(file);

  // Volcamos en la memoria convencional un buffer
  // que tenemos en la memoria extendida.
  dosmemput(sb_dma_buffer, SB_DMA_BUFFER_SIZE, dma_offset);

  sb_playing = true;

  // Calculamos la frecuencia de muestreo.
  sb_dsp_write(SB_CMD_SET_PLAYBACK_FREQUENCY);
  sb_dsp_write(256 - 1000000 / 11000);

  // Necesitamos calcular la página y el offset.
  uint32_t page = dma_offset >> 16;          // Calculate page
  uint32_t offset = dma_offset & 0xFFFF;     // Calculate offset in the page

  // Configuramos el DMA.
  outportb(MASK_REGISTER, 4 | sb.dma8);
  outportb(MSB_LSB_FLIP_FLOP, 0);
  outportb(MODE_REGISTER, 0x48 | sb.dma8);
  outportb(sb.dma8 << 1, offset & 0xFF);
  outportb(sb.dma8 << 1, offset >> 8);
  /*
      The mode consists of the following:
      0x58 + x = binary 01 00 10 xx
                        |  |  |  |
                        |  |  |  +- DMA channel
                        |  |  +---- Read operation (the DSP reads from memory)
                        |  +------- Single cycle mode
                        +---------- Block mode
  */

  // Dependiendo del canal DMA, escribimos en un puerto u otro.
  if (sb.dma8 == 0) outportb(DMA_CHANNEL_0, page);
  if (sb.dma8 == 1) outportb(DMA_CHANNEL_1, page);
  if (sb.dma8 == 3) outportb(DMA_CHANNEL_3, page);

  // Seteamos el tamaño del DMA.
  outportb((sb.dma8 << 1) + 1, 0xFF); // Set the block length to 0x7FFF = 32 Kbyte
  outportb((sb.dma8 << 1) + 1, 0x7F);
  outportb(0x0A, sb.dma8); // Unmask DMA channel

  // Indicamos a la SB que debemos reproducir un ciclo.
  sb_dsp_write(SB_CMD_SINGLE_CYCLE_PLAYBACK);
  sb_dsp_write(0xFF);
  sb_dsp_write(0x7F);
  return true;
}

void sb_start() {
  char* blaster_env = getenv("BLASTER");
  if (blaster_env == NULL) {
    log_line("  BLASTER Not set\n");
    return;
  } else {
    log_line("  BLASTER Set: %s\n", blaster_env);
  }
  sscanf(
    blaster_env,
    "A%d I%d D%d H%d M%d P%d",
    &sb.base,  // Base I/O Port
    &sb.irq,   // Interrupt Request Line
    &sb.dma8,  // DMA 8-bit
    &sb.dma16, // DMA 16-bit
    &sb.mixer, // Mixer I/O Port
    &sb.mpu401 // MPU-401 I/O Port
  );

  sb_set_base_from_hex(sb.base);

  log_line("  Base I/O Port: 0x%03X\n", sb.base);
  log_line("  IRQ: %d\n", sb.irq);
  log_line("  DMA 8-bit: %d\n", sb.dma8);
  log_line("  DMA 16-bit: %d\n", sb.dma16);
  log_line("  Mixer I/O Port: %d\n", sb.mixer);
  log_line("  MPU-401 I/O Port: %d\n", sb.mpu401);

  if (sb_dsp_reset(sb.base)) {
    log_line("  DSP Reset OK\n");
  } else {
    log_line("  DSP Reset Failed\n");
  }

  sb_irq_init();
  sb_dma_init();
  sb_dsp_write(SB_CMD_ENABLE_SPEAKER);
}

void sb_stop() {
  sb_dsp_write(SB_CMD_DISABLE_SPEAKER);
  sb_irq_restore();
}
