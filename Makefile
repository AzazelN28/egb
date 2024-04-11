DATE=$(shell date +'%Y.%m.%d.%H%M%S')

#
#
#
all:
	mkdir -p dist
	gcc -o dist/game.exe \
		-DGAME_VERSION=\"${DATE}\" \
		-funroll-loops \
		-O2 \
		src/main.c \
		src/math/fixed.c \
		src/math/rand.c \
		src/math/range.c \
		src/math/point.c \
		src/math/rect.c \
		src/math/vec2f.c \
		src/math/vec2i.c \
		src/core/game.c \
		src/core/timer.c \
		src/core/log.c \
		src/video/video.c \
		src/video/palette.c \
		src/video/font.c \
		src/video/fade.c \
		src/input/keyboard.c \
		src/input/mouse.c \
		src/sound/speaker.c \
		src/sound/sb.c \
		src/sound/opl2.c \
		src/game/map.c \
		src/game/entity.c \
		src/game/raycaster.c \
		-lm

run:
	dosbox -conf dist/dosbox.conf

debug:
	dosbox-debug -conf dist/dosbox.debug.conf

iso:
	rm -f dist/egb.iso
	mkisofs -input-charset ascii -o dist/egb.iso -V "EGB" dist

clean:
	rm dist/game.exe
