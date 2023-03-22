OUT = mound.exe
SRC = src/*.c src/*/*.c

FLAGS = -O2

$(OUT): $(SRC)
	gcc $(SRC) -o $(OUT) $(FLAGS)

test: $(OUT)
	./$(OUT) programs/test/test.mvm > programs/test/test.output