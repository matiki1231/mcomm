CC=gcc
CFLAGS=-lpthread

SERVER=src/mtk/mcomm/server
CLIENT=src/mtk/mcomm/client
COMMON=src/mtk/mcomm

OUT=bin

SERVER_FILES=$(SERVER)/main.c
CLIENT_FILES=$(CLIENT)/main.c

all: server client
server: $(SERVER_FILES)
	$(CC) $(SERVER_FILES) -o $(OUT)/server $(CFLAGS)
client: $(CLIENT_FILES)
	$(CC) $(CLIENT_FILES) -o $(OUT)/client $(CFLAGS)
clean:
	rm bin/*
