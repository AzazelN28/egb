#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>

#define SB_DMA_BUFFER_SIZE 32768

#define SB_PORT_FM_STATUS 0x0
#define SB_PORT_FM_REGISTER 0x0
#define SB_PORT_FM_DATA 0x1
#define SB_PORT_AFM_STATUS 0x2
#define SB_PORT_AFM_REGISTER 0x2
#define SB_PORT_AFM_DATA 0x3

#define SB_PORT_MIXER_REGISTER 0x4
#define SB_PORT_MIXER_DATA 0x5

// Ports to control the DSP
#define SB_PORT_DSP_RESET 0x6
#define SB_PORT_DSP_READ 0xA
#define SB_PORT_DSP_WRITE 0xC
#define SB_PORT_DSP_WRITE_STATUS 0xC
#define SB_PORT_DSP_READ_STATUS 0xE

// Ports to control the CD-ROM
#define SB_PORT_CD_COMMAND 0x10
#define SB_PORT_CD_STATUS 0x11
#define SB_PORT_CD_RESET 0x12
#define SB_PORT_CD_ENABLE 0x13

#define SB_STATUS_DATA_AVAILABLE 0x80
#define SB_STATUS_RESET_OK 0xAA

#define SB_CMD_ENABLE_SPEAKER 0xD1
#define SB_CMD_DISABLE_SPEAKER 0xD3
#define SB_CMD_SET_PLAYBACK_FREQUENCY 0x40
#define SB_CMD_SINGLE_CYCLE_PLAYBACK 0x14
#define SB_CMD_START_AUTOINIT_PLAYBACK 0x1C
#define SB_CMD_SET_BLOCK_SIZE 0x48
#define SB_CMD_STOP_AUTOINIT_PLAYBACK 0xDA

#define MASK_REGISTER 0x0A
#define MODE_REGISTER 0x0B
#define MSB_LSB_FLIP_FLOP 0x0C
#define DMA_CHANNEL_0 0x87
#define DMA_CHANNEL_1 0x83
#define DMA_CHANNEL_3 0x82

typedef struct sb_ {
  int base;
  int irq;
  int dma8;
  int dma16;
  int mixer;
  int mpu401;
} sb_t;

extern sb_t sb;
extern volatile bool sb_playing;

void sb_start();
bool sb_dsp_reset(uint16_t port);
uint8_t sb_dsp_read();
void sb_dsp_write(uint8_t command);
void sb_stop();
bool sb_play_file(const char *filename);
