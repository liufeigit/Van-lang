clang -g -fPIC -shared ./src/*.c -o ./libvan.so

clang -g ./src/van_main.c -L. -lvan -o van
