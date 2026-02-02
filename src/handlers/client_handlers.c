#include "../../include/handlers/client_handlers.h"

int handle_user_input(int server_socket) {
    char command[100];
    
    // legge da STDIN e rimuove newline
    if (fgets(command, sizeof(command), stdin) == NULL)
        return 1;
    command[strcspn(command, "\n")] = 0;

    // switch comandi
    if (!strcmp(command, "HELLO")) {
        // handler
    }
    else if (!strcmp(command, "QUIT")) {
        quit_handler(server_socket);
        return 0;
    }
    else if (!strcmp(command, "SHOW_LAVAGNA")) {
        // handler
    }
    else if (!strcmp(command, "SEND_USER_LIST")) {
        // handler
    }
    else if (!strcmp(command, "PONG_LAVAGNA")) {
        // handler
    }
    else if (!strcmp(command, "CREATE_CARD")) {
        // handler
    }
    else if (!strcmp(command, "ACK_CARD")) {
        // handler
    }
    else if (!strcmp(command, "CHOOSE_USER")) {
        // handler
    }
    else if (!strcmp(command, "CARD_DONE")) {
        // handler
    }
    else printf("[CLIENT] Comando non riconosciuto\n");
    
    return 1;
}
/*
int handle_server_message(int server_socket) {
    MsgHeader head;
    char buffer[1024]; // Buffer generico per i payload

    // 1. Usa la tua funzione recv_msg che gestisce tutto
    int ret = recv_msg(server_socket, &head, buffer, sizeof(buffer));

    if (ret <= 0) {
        printf("[CLIENT] Server disconnesso o errore.\n");
        return 0; // Segnala disconnessione
    }

    switch (head.type) {
        // --- LOGICA MATRICOLA PARI ---
        case UtB_AVAILABLE_CARD:
            printf("[CLIENT] La lavagna propone una nuova card! Inizio Asta...\n");
            // Qui implementerai la logica dell'asta:
            // 1. Casting payload (lista porte utenti)
            // 2. Calcolo costo random
            // 3. Loop connessioni agli altri utenti
            break;
            
        case UtB_ACK_CARD:
             // Conferma assegnazione (se necessario lato client)
             printf("[CLIENT] Card assegnata correttamente.\n");
             break;

        case UtB_ERR:
            printf("[CLIENT] Errore ricevuto dal server.\n");
            break;

        default:
            printf("[CLIENT] Messaggio tipo %d ignorato.\n", head.type);
            break;
    }
    return 1;
}

void handle_p2p_connection(int p2p_socket) {
    int peer_sock;
    struct sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(peer_addr);

    if ((peer_sock = accept(p2p_socket, (struct sockaddr *)&peer_addr, &addr_len)) < 0) {
        perror("[CLIENT] Errore accept P2P");
        return;
    }

    printf("[CLIENT-P2P] Nuova connessione da Peer!\n");
    
    // Qui gestirai la ricezione del costo (MSG_CHOOSE_USER)
    // Per ora chiudiamo subito per testare hello/quit
    close(peer_sock);
}
*/

void quit_handler(int server_socket) {
    printf("[CLIENT] Avvio procedura di disconnessione...\n");

    if (send_msg(server_socket, UtB_QUIT, NULL, 0) < 0)
        perror("[CLIENT] Errore nell'invio di QUIT\n");
}