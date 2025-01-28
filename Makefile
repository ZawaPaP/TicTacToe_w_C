CC := gcc

# ヘッダーファイルのディレクトリ
INCLUDE := include 

# コンパイルオプション
CFLAGS := -Wall -Wextra -Werror -I$(INCLUDE)

TARGET := Game

COMMON_SRCS := src/board.c src/game.c
SRCS := src/main.c $(COMMON_SRCS)
OBJS := $(SRCS:.c=.o)

# 実行ファイルの生成
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)


all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)


TEST_TARGET := tests/test
TEST_SRCS := tests/test_main.c tests/test_board.c
TEST_OBJS := $(TEST_SRCS:.c=.o) $(COMMON_SRCS:.c=.o)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $(TEST_TARGET)

test: test_clean $(TEST_TARGET)
	./$(TEST_TARGET)
test_clean:
	rm -f $(TEST_OBJS) $(TEST_TARGET)