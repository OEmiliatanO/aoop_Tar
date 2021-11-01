CC:=g++

all: main.o mytar.o
	$(CC) -o main main.o mytar.o
dep: main.cpp src/mytar.cpp
	$(CC) -c -W main.cpp
	$(CC) -c -W src/mytar.cpp
clean:
	rm *.o