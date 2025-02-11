CC := gcc

# ヘッダーファイルのディレクトリ
INCLUDE := include 

# コンパイルオプション
CFLAGS := -Wall -Wextra -Werror -I$(INCLUDE)

TARGET := Game
BUILD_DIR := build

COMMON_SRCS := src/board.c src/game.c src/plays.c src/queue.c src/cpu.c
SRCS := src/main.c $(COMMON_SRCS)
OBJS := $(SRCS:src/%.c=$(BUILD_DIR)/%.o)


all: $(TARGET)

# 実行ファイルの生成
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -rf $(TEST_BUILD_DIR) $(TEST_TARGET)


TEST_TARGET := tests/build/test
TEST_BUILD_DIR := tests/build

TEST_SRCS := tests/test_main.c tests/test_utils.c tests/test_board.c tests/test_plays.c tests/test_game.c tests/test_queue.c
TEST_OBJS := $(TEST_SRCS:tests/%.c=$(TEST_BUILD_DIR)/%.o) $(COMMON_SRCS:src/%.c=$(TEST_BUILD_DIR)/%.o)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $(TEST_TARGET)

$(TEST_BUILD_DIR)/%.o: tests/%.c | $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: src/%.c | $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

test: clean $(TEST_TARGET)
	./$(TEST_TARGET)
