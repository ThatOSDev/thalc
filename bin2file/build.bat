
gcc.exe -Wall -std=c17 -m64 -O2  -c main.c -o main.o

gcc.exe -o bin2file.exe main.o  -static -m64 -s

del *.o

pause

