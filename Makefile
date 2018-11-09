CC = g++

TARGET = execute_design
SOURCE = fmpart.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(SOURCE) -o $(TARGET) 