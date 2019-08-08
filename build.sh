clang -g -fPIC -shared ./src/van_lex.c ./src/van_lexDebug.c ./src/van_lexUtil.c ./src/van_parse.c ./src/van_parseDebug.c ./src/van_type.c ./src/van_parseUtil.c ./src/van_data.c ./src/van_dataUtil.c ./src/van_environ.c ./src/van_environBridge.c ./src/van_error.c -o ./libvan.so

clang -g ./src/van_main.c -L. -lvan -o van
