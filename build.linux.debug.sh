#!/bin/sh

[ -d "bin/" ]        || mkdir "bin/"
[ -d "bin/shaders" ] || mkdir "bin/shaders"
[ -d "obj/" ]        || mkdir "obj/"

optimization_level="0"

cflags="-std=gnu++14 -Iinclude -O${optimization_level}"
dflags="-g3"
define="-DPLATFORM_LINUX=1"
lflags="-Llibs/linux/x64/debug"

# Single-step compilation
# echo "[$(date +%T)] Building main"
# g++ -obin/main ${cflags} ${dflags} ${define} "code/main.cpp" ${lflags} -lyocto_gl -lglfw -lGLEW -lGL -pthread
# [ $? -eq 0 ] || exit 1

echo "[$(date +%T)] Building main"
g++ -c -oobj/main.o ${cflags} ${dflags} ${define} "code/main.cpp"
[ $? -eq 0 ] || exit 1

echo "[$(date +%T)] Linking main"
g++ -obin/main "obj/main.o" ${lflags} -lyocto_gl -lglfw -lGLEW -lGL -pthread
[ $? -eq 0 ] || exit 1


# Relocate shaders to a more findable path (from the exe)
if [ -f "code/shaders/fullscreen_quad.vert" -a ! -f "bin/shaders/shader.vert" ]; then
	echo "Hardlinking \"code/shaders/fullscreen_quad.vert\" -> \"bin/shaders/shader.vert\""
	ln "code/shaders/fullscreen_quad.vert" "bin/shaders/shader.vert"
fi

if [ -f "code/shaders/raymarcher.frag" -a ! -f "bin/shaders/shader.frag" ]; then
	echo "Hardlinking \"code/shaders/raymarcher.frag\" -> \"bin/shaders/shader.frag\""
	ln "code/shaders/raymarcher.frag" "bin/shaders/shader.frag"
fi
