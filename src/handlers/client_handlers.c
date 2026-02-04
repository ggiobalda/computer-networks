#include "../../include/handlers/client_handlers.h"

int user_input_handler(int server_socket) {
    // legge da STDIN e rimuove newline
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL)
        return 1;
    input[strcspn(input, "\n")] = 0;

    char* command = strtok(input, " ");
    char* args = strtok(NULL, "");

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
    else if (!strcmp(command, "CREATE_CARD")) {
        command_create_card_handler(server_socket, args);
    }
    else if (!strcmp(command, "ACK_CARD")) {
        command_ack_card_handler(server_socket, args);
    }
    else if (!strcmp(command, "CARD_DONE")) {
        command_card_done_handler(server_socket);
    }
    else printf("[CLIENT] Comando non riconosciuto\n");
    
    return 1;
}

int server_msg_handler(int server_socket) {
    // ricezione header messaggio e gestione errori
    MsgHeader head;
    char buffer[MAX_PAYLOAD_SIZE];
    int ret = recv_msg(server_socket, &head, buffer, sizeof(buffer));
    if (ret <= 0) {
        printf("[CLIENT] Lavagna disconnessa. Terminazione.\n");
        return 0;
	}

    // switch in base all'header
    switch (head.type) {
        case BtU_AVAILABLE_CARD:
            //handler
            break;
        
        case BtU_PING:
            send_msg(server_socket, UtB_PONG, NULL, 0);
            printf("[CLIENT] Risposto al PING della lavagna.\n");
            break;
        
        case BtU_SHOW_LAVAGNA:
            response_show_lavagna_handler(&head, buffer);
            break;
        
        case BtU_SEND_USER_LIST:
            response_send_user_list_handler(&head, buffer);
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
    if (send_msg(server_socket, UtB_QUIT, NULL, 0) < 0)
        perror("[CLIENT] Errore nell'invio di QUIT\n");
    else
        printf("[CLIENT] Richiesta QUIT inviata\n");
}

void command_show_lavagna_handler(int server_socket) {
    if (send_msg(server_socket, UtB_SHOW_LAVAGNA, NULL, 0) < 0)
        perror("[CLIENT] Errore nell'invio di SHOW LAVAGNA\n");
    else
        printf("[CLIENT] Richiesta SHOW LAVAGNA inviata\n");
}

void command_send_user_list_handler(int server_socket) {
    if (send_msg(server_socket, UtB_SEND_USER_LIST, NULL, 0) < 0)
        perror("[CLIENT] Errore nell'invio di SEND USER LIST\n");
    else
        printf("[CLIENT] Richiesta SEND USER LIST inviata\n");
}

void command_create_card_handler(int server_socket, char* args) {
    if (args == NULL || strlen(args) == 0) {
        printf("[CLIENT] Errore: Inserisci una descrizione. Uso: CREATE_CARD <descrizione>\n");
        return;
    }

    MsgCreateCardPayload payload;
    strncpy(payload.description, args, MAX_CARD_DESC_CHARS - 1);
    payload.description[MAX_CARD_DESC_CHARS - 1] = '\0';

    if (send_msg(server_socket, UtB_CREATE_CARD, &payload, sizeof(payload)) < 0)
        perror("[CLIENT] Errore nell'invio di SEND USER LIST\n");
    else
        printf("[CLIENT] Richiesta creazione card inviata: \"%s\"\n", payload.description);
}

void command_ack_card_handler(int server_socket, char* args) {
    if (args == NULL) {
        printf("[CLIENT] Errore: Specifica ID. Uso: ACK_CARD <id>\n");
        return;
    }
    
    int card_id = atoi(args);
    MsgAckCardPayload payload;
    payload.card_id = card_id;

    if (send_msg(server_socket, UtB_ACK_CARD, &payload, sizeof(payload)) < 0)
        perror("[CLIENT] Errore nell'invio di SEND USER LIST\n");
    else
        printf("[CLIENT] Inviato ACK per card %d\n", card_id);
}

void command_card_done_handler(int server_socket) {
    if (send_msg(server_socket, UtB_CARD_DONE, NULL, 0) < 0)
        perror("[CLIENT] Errore nell'invio di SEND USER LIST\n");
    else
        printf("[CLIENT] Inviato completamento lavoro (CARD_DONE).\n");
}

void response_show_lavagna_handler(MsgHeader* head, char* payload) {
    MsgShowLavagnaPayload* response_payload = (MsgShowLavagnaPayload*)payload;
    
    response_payload->string[head->payload_len - 1] = '\0';
    printf("%s\n", response_payload->string);
}

void response_send_user_list_handler(MsgHeader* head, char* payload) {
    MsgUserListPayload* response_payload = (MsgUserListPayload*)payload;
    
    printf("\n--- LISTA UTENTI CONNESSI (%d) ---\n", response_payload->n_users);
    
    if (response_payload->n_users == 1) {
        printf("Nessun altro utente connesso.\n");
    }
    else {
        for (int i = 0; i < response_payload->n_users; i++) {
            printf("- Utente porta: %d\n", response_payload->users_ports[i]);
        }
    }
    printf("----------------------------------\n");
}