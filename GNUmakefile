# -*- mode: makefile-gmake; -*-

TARGET=scripts-rofi
SRCS   = src/main.c
OBJS   = $(SRCS:.c=.o)


# ----------------------------------------------


LDFLAGS += -fsanitize=address,undefined

CFLAGS  += -g -O0 -Wall -Werror -Wpedantic \
		-D_GLIBCXX_ASSERTIONS \
		-fstack-protector \
		-fsanitize=address,undefined

CFLAGS  += -Imlib -Iinclude

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)




