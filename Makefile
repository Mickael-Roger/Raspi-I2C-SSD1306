LIB = -llgpio
TARGET = screen
CC = gcc


${TARGET}:
	$(CC) -o $(TARGET) main.c $(LIB)
