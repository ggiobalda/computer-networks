#include "../../include/handlers/client_handlers.h"

int user_input_handler(int server_socket) {
    char command[100];
    
    // legge da STDIN e rimuove newline
    if (fgets(command, sizeof(command), stdin) == NULL)
        return 1;
    command[strcspn(command, "\n")] = 0;

    // switch comandi
    if (!strcmp(command, "QUIT")) {
        command_quit_handler(server_socket);
        return 0;
    }
    else if (!strcmp(command, "SHOW_LAVAGNA")) {
        command_show_lavagna_handler(server_socket);
    }
    else if (!strcmp(command, "SEND_USER_LIST")) {
        command_send_user_list_handler(server_socket);
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

int handle_server_message(int server_socket) {
    MsgHeader head;
    char buffer[MAX_PAYLOAD];
    int ret = recv_msg(server_socket, &head, buffer, sizeof(buffer));
	
    if (ret <= 0) {
        printf("[CLIENT] Lavagna disconnessa. Terminazione.\n");
        return 0;
	}

    switch (head.type) {
        case BtU_AVAILABLE_CARD:
            //handler
            break;
        
        case BtU_PING:
            //handler
            break;
        
        case BtU_SHOW_LAVAGNA:
            //handler
            break;
        
        case BtU_SEND_USER_LIST:
            //handler
            break;
        
        default:
            printf("[CLIENT] Messaggio tipo %d ignorato.\n", head.type);
            break;
    }
    return 1;
}

/*
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

void command_quit_handler(int server_socket) {
    printf("[CLIENT] Avvio procedura di disconnessione...\n");

    if (send_msg(server_socket, UtB_QUIT, NULL, 0) < 0)
        perror("[CLIENT] Errore nell'invio di QUIT\n");
}

void command_show_lavagna_handler(int server_socket) {
    printf("[CLIENT] Invio richiesta al server...\n");

    if (send_msg(server_socket, UtB_SHOW_LAVAGNA, NULL, 0) < 0)
        perror("[CLIENT] Errore nell'invio di SHOW LAVAGNA\n");
}

void command_send_user_list_handler(int server_socket) {
    printf("[CLIENT] Invio richiesta al server...\n");

    if (send_msg(server_socket, UtB_SEND_USER_LIST, NULL, 0) < 0)
        perror("[CLIENT] Errore nell'invio di SEND USER LIST\n");
}