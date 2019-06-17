#Nikolay Nikolov
#Class ECE 362
#Assignment # A5- part 1
#Date 03/03/2019



#The compiler: gcc
CC = gcc


#compiler flags:
# -pthread
# -g adds debugging information to the executable file
# -Wall turns on most, but not all , compiler warnings

TARGET = threads



all:$(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -pthread  $(CFLAGS) -o $(TARGET) $(TARGET).c



clean:
	$(RM) $(TARGET)

