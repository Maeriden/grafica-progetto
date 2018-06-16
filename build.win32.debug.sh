#!/bin/sh

[ -d "bin/" ]        || mkdir "bin/"
[ -d "bin/shaders" ] || mkdir "bin/shaders"
[ -d "obj" ]         || mkdir "obj"

optimization_level="d"

cflags="/nologo /MDd /EHsc /Iinclude /O${optimization_level}"
dflags="/Zi"
define="/DPLATFORM_WIN32=1"
lflags="/nologo /subsystem:console /incremental:no /opt:ref /libpath:libs\\win32\\x64\\debug"

# Single-step compilation
# echo "[$(date +%T)] Building main"
# cl "code\\main.cpp" /Fe"bin\\main.exe" /Fo"obj\\" /Fd"bin\\" ${cflags} ${dflags} ${define} /link ${lflags} yocto_gl.lib glfw3dll.lib glew32.lib OpenGL32.Lib
# [ $? -eq 0 ] || exit 1

echo "[$(date +%T)] Compiling main"
cl.exe /c "code\\main.cpp" /Fo"obj\\" /Fd"bin\\main.pdb" ${cflags} ${dflags} ${define}
[ $? -eq 0 ] || exit 1

echo "[$(date +%T)] Linking main"
link.exe "obj\\main.obj" /out:"bin\\main.exe" /debug /pdb:"bin\\main.pdb" ${lflags} yocto_gl.lib glfw3dll.lib glew32.lib OpenGL32.Lib
[ $? -eq 0 ] || exit 1


# Relocate shaders to a more findable path (from the exe)
if [ -f "code/shaders/fullscreen_quad.vert" -a ! -f "bin/shaders/raymarcher.vert" ]; then
	echo "Hardlinking \"code/shaders/fullscreen_quad.vert\" -> \"bin/shaders/raymarcher.vert\""
	ln "code/shaders/fullscreen_quad.vert" "bin/shaders/raymarcher.vert"
fi

if [ -f "code/shaders/raymarcher.frag" -a ! -f "bin/shaders/raymarcher.frag" ]; then
	echo "Hardlinking \"code/shaders/raymarcher.frag\" -> \"bin/shaders/raymarcher.frag\""
	ln "code/shaders/raymarcher.frag" "bin/shaders/raymarcher.frag"
fi
