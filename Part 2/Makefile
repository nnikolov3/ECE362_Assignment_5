#Nikolay Nikolov
#Class ECE 362
#Date 03/03/2019



#The compiler: gcc
CC = gcc


#compiler flags:
# -pthread
# -g adds debugging information to the executable file
# -Wall turns on most, but not all , compiler warnings

TARGET = sort



all:$(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -pthread  $(CFLAGS) -o $(TARGET) $(TARGET).c



clean:
	$(RM) $(TARGET)

