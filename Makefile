#
#
#
all:
	mkdir -p dist
	gcc -o dist/game.exe src/main.c src/math/rand.c src/math/point.c src/math/rect.c src/math/vec2f.c src/math/vec2i.c src/core/game.c src/core/timer.c src/video/video.c src/video/palette.c src/input/keyboard.c src/input/mouse.c src/sound/speaker.c src/sound/sb.c

run:
	dosbox -conf dist/dosbox.conf

clean:
	rm dist/game.exe
