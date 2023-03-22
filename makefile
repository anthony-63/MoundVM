OUT = mound.exe
SRC = src/*.c src/*/*.c

FLAGS = -O3 -Wall

$(OUT): $(SRC)
	gcc $(SRC) -o $(OUT) $(FLAGS)

test: $(OUT)
	./$(OUT) programs/test.mvm