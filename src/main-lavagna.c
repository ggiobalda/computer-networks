#include "../include/network/utils.h"
#include "../include/classes/board.h"
#include "../include/classes/card.h"
#include "../include/classes/user.h"
#include "../include/handlers/server_handlers.h"

int main() {
	/* 1) INIZIALIZZAZIONE KANBAN E STAMPA */
	Board* kanban = create_board(BOARD_PORT);

	// creazione di qualche card iniziale
	for (int i = 0; i < 5; i++) {
		char desc[MAX_CHARS];
		sprintf(desc, "Card%d", i);
		add_card(kanban, TODO, desc);
	}

	printf("[SERVER] Lavagna inizializzata\n");
	print_board(kanban);

	/* 2) SETUP DEL SERVER */
	// socket per l'ascolto
	int server_sock;
	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("[SERVER] Errore creazione socket\n");
		exit(EXIT_FAILURE);
	}
	
	// indirizzo server
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = BOARD_ADDRESS;
	server_addr.sin_port = htons(BOARD_PORT);

	// configurazione socket per l'ascolto 
	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("[SERVER] Errore bind socket\n");
		close(server_sock);
		return(EXIT_FAILURE);
	}
	if (listen(server_sock, MAX_USERS) < 0)  {
		perror("[SERVER] Errore listen socket\n");
		close(server_sock);
		return(EXIT_FAILURE);
	}
	printf("[SERVER] In ascolto sulla porta %d...\n", BOARD_PORT);

	// inizializzazione liste socket
	fd_set read_fds, master_fds;
	FD_ZERO(&read_fds);
	FD_ZERO(&master_fds);
	FD_SET(server_sock, &master_fds);
	int max_fd = server_sock;
	
	/* 3) GESTIONE RICHIESTE CON CICLO INFINITO */
	for (;;) {
		read_fds = master_fds;

		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
			perror("[SERVER] Errore select\n");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= max_fd; i++) {
			if (FD_ISSET(i, &read_fds)) {
				if (i == server_sock) { // Caso A: nuova connessione
					int newfd;
					struct sockaddr_in client_addr;
					socklen_t addr_len = sizeof(client_addr);

					if (newfd = accept(server_sock, (struct sockaddr*)&server_addr, &addr_len) < 0) {
						perror("[SERVER] Errore accept\n");
						continue;
					}

					printf("[SERVER] Nuovo socket connesso: %d\n", newfd);
					FD_SET(newfd, &master_fds);
					if (newfd > max_fd)
						max_fd = newfd;
				}
				else { // caso B: client già connesso
					MsgHeader head;
					char buffer[MAX_PAYLOAD];
					int ret;
					
					if ((ret = recv_msg(i, &head, buffer, sizeof(buffer))) < 0) {
						if (ret == 0) {
							printf("[SERVER] Socket %d disconnesso\n");
							remove_user(kanban, i);
							continue;
						}
						else {
							printf("[SERVER] Errore ricezione messaggio\n");
							continue;
						}
					}

					switch(head.type) {
						case UtB_HELLO:
                            server_hello_handler(kanban, i, buffer);
                            break;

                        case UtB_QUIT:
							server_quit_handler(kanban, i, &master_fds);
                            break;

                        default:
                                printf("[SERVER] Messaggio tipo %d non gestito\n", head.type);
					}
				}
			}
		}
	}

	return 0;
}
