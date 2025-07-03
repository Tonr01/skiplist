# 編譯器與參數設定
CC = gcc
CFLAGS = -Wall -g
TARGET = skiplist
SRC = main.c skiplist.c qtest.c console.c
OBJS = $(SRC:.c=.o)

# 預設目標（make）
all: $(TARGET)

# 編譯規則
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 執行程式
run: $(TARGET)
	./$(TARGET)

# 清除執行檔
clean:
	rm -f $(TARGET) $(OBJS)
