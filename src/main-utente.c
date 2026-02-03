#include "../include/network/utils.h"
#include "../include/classes/card.h"
#include "../include/classes/user.h"
#include "../include/handlers/client_handlers.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		perror("Utilizzo: ./utente <porta>\n");
		exit(EXIT_FAILURE);
	}

	int my_port = atoi(argv[1]);
	if (my_port < MIN_PORT) {
		printf("Porta non valida: valore minimo = %d", MIN_PORT);
		exit(EXIT_FAILURE);
	}

	/* 1) SETUP SOCKET SERVER*/
	int server_sock;
	struct sockaddr_in server_addr;

	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("[CLIENT] Errore creazione socket server\n");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(BOARD_PORT);
	inet_pton(AF_INET, BOARD_ADDRESS, &server_addr.sin_addr);

	if (connect(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("[CLIENT] Errore connessione al server\n");
		exit(EXIT_FAILURE);
	}

	printf("[CLIENT] Connesso alla Lavagna sulla porta %d\n", BOARD_PORT);

	/* 2) SETUP SOCKET P2P */
	int p2p_sock;
	struct sockaddr_in my_addr;

	if ((p2p_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("[CLIENT] Errore creazione socket p2p\n");
		exit(EXIT_FAILURE);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(my_port);

	// configurazione socket per l'ascolto 
	if (bind(p2p_sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
		perror("[CLIENT] Errore bind socket p2p2\n");
		close(p2p_sock);
		return(EXIT_FAILURE);
	}
	if (listen(p2p_sock, 5) < 0)  {
		perror("[CLIENT] Errore listen socket p2p\n");
		close(p2p_sock);
		return(EXIT_FAILURE);
	}

	printf("[CLIENT] In ascolto per P2P sulla porta %d\n", my_port);
	
	/* 3) INVIO HELLO (REGISTRAZIONE)*/
	MsgHelloPayload hello;
    hello.port = my_port;

    if (send_msg(server_sock, UtB_HELLO, &hello, sizeof(hello)) < 1) {
        perror("Errore invio HELLO\n");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("[CLIENT] HELLO inviato. Digita QUIT per uscire.\n");
	
	/* 4) MAIN LOOP */
	fd_set read_fds, master_fds;
    FD_ZERO(&master_fds);
    FD_SET(STDIN_FILENO, &master_fds);	// Input tastiera
    FD_SET(server_sock, &master_fds);	// Messaggi dal server
    FD_SET(p2p_sock, &master_fds);		// Messaggi P2P

    int max_fd = (server_sock > p2p_sock) ? server_sock : p2p_sock;

    for (;;) {
        read_fds = master_fds;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("[CLIENT] Errore select\n");
            break;
        }

        // A) INPUT TASTIERA
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            if (user_input_handler(server_sock) < 0)
				break;
        }

        // B) MESSAGGI DAL SERVER
        if (FD_ISSET(server_sock, &read_fds)) {
            if (server_msg_handler(server_sock) <= 0)
				break;
        }

        // C) CONNESSIONI P2P
        if (FD_ISSET(p2p_sock, &read_fds)) {
            //p2p_msg_handler(p2p_sock);
        }
    }

	/* 5) CLEANUP */
	printf("[CLIENT] Chiusura client\n");
	close(server_sock);
	close(p2p_sock);
	
	return 0;
}