CC      = gcc

CFLAGS  = -Wall -std=c11 -Iinclude \
           -finput-charset=UTF-8 \
           -fexec-charset=UTF-8

TARGET  = qlks.exe

SRCS    = src/main.c \
           src/room.c \
           src/customer.c \
           src/booking.c \
           src/billing.c \
           src/service.c \
           src/membership.c \
           src/file.c

OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /f /q src\*.o $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run