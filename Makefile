CXX = g++
CFLAG = -g -Wall
LIBS = -lcapstone # sudo apt-get install libcaptone-dev
SRC = $(wildcard ./*.cpp) # load.cpp run.cpp ...
OBJ = $(SRC:.cpp=.o) # replace all .cpp to .o from SRC
TARGET = ./hw4

all: $(OBJ)
	$(CXX) $(CFLAG) -o $(TARGET) $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) $(CFLAG) -c $^ -o $@ $(LIBS)

clean:
	rm -rf *.o
	rm -rf hw4