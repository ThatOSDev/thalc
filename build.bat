gcc.exe -Wall -std=c17 -m64 -DPLATFORM_DESKTOP -O2 -Ilibs\raylib-4.5.0\src -Ilibs\raylib-4.5.0\src\external\glfw\include -c libs\raylib-4.5.0\src\rcore.c -o rcore.o

gcc.exe -Wall -std=c17 -m64 -DPLATFORM_DESKTOP -O2 -Ilibs\raylib-4.5.0\src -Ilibs\raylib-4.5.0\src\external\glfw\include -c libs\raylib-4.5.0\src\rglfw.c -o rglfw.o

gcc.exe -Wall -std=c17 -m64 -DPLATFORM_DESKTOP -O2 -Ilibs\raylib-4.5.0\src -Ilibs\raylib-4.5.0\src\external\glfw\include -c libs\raylib-4.5.0\src\rmodels.c -o rmodels.o

gcc.exe -Wall -std=c17 -m64 -DPLATFORM_DESKTOP -O2 -Ilibs\raylib-4.5.0\src -Ilibs\raylib-4.5.0\src\external\glfw\include -c libs\raylib-4.5.0\src\rshapes.c -o rshapes.o

gcc.exe -Wall -std=c17 -m64 -DPLATFORM_DESKTOP -O2 -Ilibs\raylib-4.5.0\src -Ilibs\raylib-4.5.0\src\external\glfw\include -c libs\raylib-4.5.0\src\rtext.c -o rtext.o

gcc.exe -Wall -std=c17 -m64 -DPLATFORM_DESKTOP -O2 -Ilibs\raylib-4.5.0\src -Ilibs\raylib-4.5.0\src\external\glfw\include -c libs\raylib-4.5.0\src\rtextures.c -o rtextures.o

gcc.exe -Wall -std=c17 -m64 -DPLATFORM_DESKTOP -O2 -Ilibs\raylib-4.5.0\src -Ilibs\raylib-4.5.0\src\external\glfw\include -c libs\raylib-4.5.0\src\utils.c -o utils.o

gcc.exe -Wall -std=c17 -m64 -DPLATFORM_DESKTOP -O2 -Ilibs\raylib-4.5.0\src -Ilibs\raylib-4.5.0\src\external\glfw\include -c main.c -o main.o

gcc.exe  -o thalc.exe rcore.o rglfw.o rmodels.o rshapes.o rtext.o rtextures.o utils.o main.o -static -m64 -lgdi32 -s -mwindows

del *.o

pause
