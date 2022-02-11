TARGET = main.out
TARGET_1 = test.out
HDRS_DIR = project/include

SRCS = \
project/src/main.c \
project/src/process.c \
project/src/changefile.c \

TESTS = project/src/test.c \
project/src/changefile.c


.PHONY: all test clean

all: main test

main: $(SRCS)
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

test: $(TESTS)
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET_1) $(CFLAGS) $(TESTS)

clean:
	rm -rf $(TARGET) $(TARGET_1)
