#include "../include/network/utils.h"
#include "../include/classes/card.h"
#include "../include/classes/user.h"
#include "../include/handlers/client/cli_handlers.h"
#include "../include/handlers/client/p2p_auction_handlers.h"
#include "../include/handlers/client/server_msg_handlers.h"

int main(int argc, char* argv[]) {
	/* ------------- INIZIALIZZAZIONE ------------- */
	// controllo argomenti
	if (argc < 2) {
		perror("Utilizzo: ./utente <porta>\n");
		exit(EXIT_FAILURE);
	}

	// recupero porta da argomento
	int my_port = atoi(argv[1]);
	if (my_port < MIN_PORT) {
		printf("Porta non valida: valore minimo = %d\n", MIN_PORT);
		exit(EXIT_FAILURE);
	}

	/* ------------- SETUP SOCKET SERVER ------------- */
	// creazione socket per comunicare col server
	int server_sock;
	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Errore creazione socket server\n");
		exit(EXIT_FAILURE);
	}
	
	// indirizzo server
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(BOARD_PORT);
	inet_pton(AF_INET, BOARD_ADDRESS, &server_addr.sin_addr);

	// connessione col server
	if (connect(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Errore connessione al server\n");
		exit(EXIT_FAILURE);
	}
	printf("Connesso alla Lavagna sulla porta %d\n", BOARD_PORT);

	/* ------------- SETUP SOCKET P2P ------------- */
	// creazione socket per comunicare con altri utenti
	int p2p_sock;
	if ((p2p_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Errore creazione socket p2p\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL) + my_port); // randomicizzazione costo
	
	// imposto indirizzo come riutilizzabile, così client può essere subito riavviato
	int opt = 1;
    if (setsockopt(p2p_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("Errore setsockopt\n");
        exit(EXIT_FAILURE);
    }
	
	// configurazione indirizzo p2p
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(my_port);

	// configurazione socket per l'ascolto 
	if (bind(p2p_sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
		perror("Errore bind socket p2p2\n");
		close(p2p_sock);
		return(EXIT_FAILURE);
	}
	if (listen(p2p_sock, 5) < 0)  {
		perror("Errore listen socket p2p\n");
		close(p2p_sock);
		return(EXIT_FAILURE);
	}
	printf("In ascolto per P2P sulla porta %d\n", my_port);
	
	/* ------------- REGISTRAZIONE ------------- */
	MsgHelloPayload hello;
    hello.port = my_port;
    if (send_msg(server_sock, UtB_HELLO, &hello, sizeof(hello)) < 1) {
        perror("Errore invio HELLO\n");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    printf("Registrazione al server con HELLO avvenuta\n");
	
	/* ------------- MAIN LOOP ------------- */
	// inizializzazione liste descrittori socket
	fd_set read_fds, master_fds;
    FD_ZERO(&master_fds);
    FD_SET(STDIN_FILENO, &master_fds);	// input terminale
    FD_SET(server_sock, &master_fds);	// messaggi dal server
    FD_SET(p2p_sock, &master_fds);		// messaggi P2P
    int max_fd = (server_sock > p2p_sock) ? server_sock : p2p_sock;

	// struct per timeout
	struct timeval tv;

	printf("\nComandi disponibli:\n- QUIT, notifica server e termina esecuzione\n- SHOW_LAVAGNA, mostra stato attuale della lavagna\n- SEND_USER_LIST, ricevi lista utenti attualmente registrati\n- CREATE_CARD <descrizione> (max 30 caratteri), crea nuova card da porre in TODO\n- ACK CARD <id card>, per testare assegnamento card (normalmente fatto tramite asta)\n- CARD DONE, per segnalare termine lavoro (anche questo solo per testare)\n\n");

	// gestione richieste con ciclo infinito
    for (;;) {
        // copia lista master in lettura e calcolo timeout
		read_fds = master_fds;
		struct timeval* tv_ptr = get_task_timer(&tv);
		
		// select socket attivo
        if (select(max_fd + 1, &read_fds, NULL, NULL, tv_ptr) < 0) {
            perror("Errore select\n");
            break;
        }

		// controllo se task terminata
		check_task_completion(server_sock);

        // A) è arrivato un input dal terminale
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            if (user_input_handler(server_sock) < 0)
				break;
        }

        // B) è arrivato un messaggio dal server
        if (FD_ISSET(server_sock, &read_fds)) {
            if (server_msg_handler(server_sock, my_port) <= 0)
				break;
        }

        // C) è arrivato un messaggio da un peer
        if (FD_ISSET(p2p_sock, &read_fds)) {
            p2p_msg_handler(p2p_sock, server_sock);
        }
    }

	/* ------------- PULIZIA FINALE ------------- */
	printf("Chiusura client\n");
	close(server_sock);
	close(p2p_sock);
	
	return 0;
}