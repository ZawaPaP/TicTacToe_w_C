CC := gcc

# ヘッダーファイルのディレクトリ
INCLUDE := include 

# コンパイルオプション
CFLAGS := -Wall -Wextra -Werror -I$(INCLUDE)

TARGET := Game

SRCS := src/main.c src/game.c
OBJS := $(SRCS:.c=.o)

# 実行ファイルの生成
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)


# cleanした後に、オブジェクトファイルと実行ファイルを生成する。
all: clean $(OBJS) $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

