rem emcc src/main.c -O2 -s USE_SDL=2 --preload-file assets -o build/main.html
if not exist build mkdir build
emcc src/main.c -O2 -s TOTAL_MEMORY=64MB -s USE_SDL=2 --preload-file assets -o build/main.html
