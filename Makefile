CC      = gcc

CFLAGS  = -Wall -std=c11 -Iinclude \
           -finput-charset=UTF-8 \
           -fexec-charset=UTF-8

TARGET  = qlks.exe

SRCS    = SRC/main.c \
           SRC/room.c \
           SRC/customer.c \
           SRC/booking.c \
           SRC/billing.c \
           SRC/service.c \
           SRC/membership.c \
           SRC/file.c

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