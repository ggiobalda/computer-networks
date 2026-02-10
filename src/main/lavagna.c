#include "../include/network/utils.h"
#include "../include/classes/board.h"
#include "../include/classes/card.h"
#include "../include/classes/user.h"
#include "../include/handlers/server_handlers.h"

char* init_cards[10] = {
	"Brainstorm progetto",
	"Assegnazione ruoli progetto",
	"Acquisizione risorse",
	"Mockup iniziale",
	"Sviluppo mockup in prototipo",
	"Implementazione prototipo",
	"Testing funzionalita' base",
	"Raffinamento prototipo",
	"Esposizione con investitori",
	"Lancio sul mercato"
};

int main() {
	/* ------------- INIZIALIZZAZIONE ------------- */
	// ignora sigpipe per evitare crash se un client si disconnette mentre scriviamo
	signal(SIGPIPE, SIG_IGN);

	// creazione kanban con qualche card
	Board* kanban = create_board(BOARD_PORT);
	for (int i = 0; i < 10; i++)
		add_card(kanban, TODO, init_cards[i]);
	printf("Lavagna inizializzata\n");

	// stampa iniziale
	char printbuf[MAX_PAYLOAD_SIZE];
	board_to_string(kanban, printbuf, sizeof(printbuf));
	printf("%s", printbuf);

	/* ------------- SETUP SOCKET SERVER ------------- */
	// creazione socket per l'ascolto
	int server_sock;
	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Errore creazione socket\n");
		exit(EXIT_FAILURE);
	}

	// imposto indirizzo come riutilizzabile, così lavagna può essere subito riavviata
	int opt = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Errore setsockopt\n");
        exit(EXIT_FAILURE);
    }
	
	// configurazione indirizzo server
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(BOARD_PORT);

	// configurazione socket 
	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Errore bind socket\n");
		close(server_sock);
		return(EXIT_FAILURE);
	}
	if (listen(server_sock, MAX_USERS) < 0)  {
		perror("Errore listen socket\n");
		close(server_sock);
		return(EXIT_FAILURE);
	}
	printf("In ascolto sulla porta %d...\n", BOARD_PORT);

	/* ------------- MAIN LOOP ------------- */
	// inizializzazione lista descrittori socket
	fd_set read_fds, master_fds;
	FD_ZERO(&master_fds);
	FD_SET(server_sock, &master_fds);
	int max_fd = server_sock;

	// struct per timeout
	struct timeval tv;
	
	// gestione richieste con ciclo infinito
	for (;;) {
		// copia lista master in lettura e reset timeout
		read_fds = master_fds;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		// select socket attivo
		if (select(max_fd + 1, &read_fds, NULL, NULL, &tv) < 0) {
			perror("Errore select\n");
			exit(EXIT_FAILURE);
		}

		// scorro lista descrittori
		for (int i = 0; i <= max_fd; i++) {
			if (FD_ISSET(i, &read_fds)) {
				
				// nuova connessione
				if (i == server_sock) {
					// creazione nuovo socket e accept
					int newfd;
					struct sockaddr_in client_addr;
					socklen_t addr_len = sizeof(client_addr);

					if ((newfd = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len)) < 0) {
						perror("Errore accept\n");
						continue;
					}
					printf("Nuovo socket connesso: %d\n", newfd);

					// aggiornamento fdset
					FD_SET(newfd, &master_fds);
					if (newfd > max_fd)
						max_fd = newfd;
				}

				// richiesta da socket già nel set
				else {
					// ricezione header messaggio e gestione errori
					MsgHeader head;
					char buffer[MAX_PAYLOAD_SIZE];
					int ret = recv_msg(i, &head, buffer, sizeof(buffer));
					if (ret == 0) {
						server_quit_handler(kanban, i, &master_fds);
						continue;
					}
					else if (ret < 0){
						printf("Errore ricezione messaggio\n");
						continue;
					}

					// switch in base all'header
					switch(head.type) {
						case UtB_HELLO:
                            server_hello_handler(kanban, i, buffer);
							if (kanban->n_users > 1)
								server_available_card_handler(kanban);
                            break;

                        case UtB_QUIT:
							server_quit_handler(kanban, i, &master_fds);
                            break;
						
						case UtB_SHOW_LAVAGNA:
							server_show_lavagna_handler(kanban, i);
							break;
						
						case UtB_SEND_USER_LIST:
							server_send_user_list_handler(kanban, i);
							break;

						case UtB_PONG:
							server_pong_handler(kanban, i);
							break;
							
						case UtB_CREATE_CARD:
							server_create_card_handler(kanban, i, buffer);
							board_to_string(kanban, printbuf, sizeof(printbuf));
							printf("%s", printbuf);
							break;
						
						case UtB_ACK_CARD:
							server_ack_card_handler(kanban, i, buffer);
							board_to_string(kanban, printbuf, sizeof(printbuf));
							printf("%s", printbuf);
							break;
						
						case UtB_CARD_DONE:
							server_card_done_handler(kanban, i);
							board_to_string(kanban, printbuf, sizeof(printbuf));
							printf("%s", printbuf);
							break;
						
                        default:
                                printf("Messaggio tipo %d non gestito\n", head.type);
					}
				}
			}
		}
		
		// controllo ping/pong
		server_check_timeouts(kanban);
	}

	return 0;
}
