# compiler e flags
CC = gcc
CFLAGS = -Wall -g -Iinclude

# sorgenti comuni (utils, user, card)
COMMON_SRC = src/network/utils.c src/classes/user.c src/classes/card.c

# sorgenti specifici per lavagna e utente
SERVER_SRC = src/main-lavagna.c src/classes/board.c src/handlers/server_handlers.c
CLIENT_SRC = src/main-utente.c src/handlers/client/cli_handlers.c src/handlers/client/p2p_auction_handlers.c src/handlers/client/server_msg_handlers.c

all: lavagna utente

lavagna: $(COMMON_SRC) $(SERVER_SRC)
	$(CC) $(CFLAGS) $(COMMON_SRC) $(SERVER_SRC) -o lavagna

utente: $(COMMON_SRC) $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(COMMON_SRC) $(CLIENT_SRC) -o utente

clean:
	rm -f lavagna utente
