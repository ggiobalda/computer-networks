#include "../../../include/handlers/client/cli_handlers.h"

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
    else printf("Comando non riconosciuto\n");
    
    return 1;
}

void command_quit_handler(int server_socket) {
    if (send_msg(server_socket, UtB_QUIT, NULL, 0) < 0)
    perror("Errore nell'invio di QUIT\n");
    else
        printf("Richiesta QUIT inviata\n");
}

void command_show_lavagna_handler(int server_socket) {
    if (send_msg(server_socket, UtB_SHOW_LAVAGNA, NULL, 0) < 0)
    perror("Errore nell'invio di SHOW LAVAGNA\n");
    else
    printf("Richiesta SHOW LAVAGNA inviata\n");
}

void command_send_user_list_handler(int server_socket) {
    if (send_msg(server_socket, UtB_SEND_USER_LIST, NULL, 0) < 0)
    perror("Errore nell'invio di SEND USER LIST\n");
    else
    printf("Richiesta SEND USER LIST inviata\n");
}

void command_create_card_handler(int server_socket, char* args) {
    if (args == NULL || strlen(args) == 0) {
        printf("Errore: Inserisci una descrizione. Uso: CREATE_CARD <descrizione>\n");
        return;
    }
    
    MsgCreateCardPayload payload;
    strncpy(payload.description, args, MAX_CARD_DESC_CHARS - 1);
    payload.description[MAX_CARD_DESC_CHARS - 1] = '\0';
    
    if (send_msg(server_socket, UtB_CREATE_CARD, &payload, sizeof(payload)) < 0)
    perror("Errore nell'invio di SEND USER LIST\n");
    else
    printf("Richiesta creazione card inviata: \"%s\"\n", payload.description);
}

void command_ack_card_handler(int server_socket, char* args) {
    if (args == NULL) {
        printf("Errore: Specifica ID. Uso: ACK_CARD <id>\n");
        return;
    }
    
    int card_id = atoi(args);
    MsgAckCardPayload payload;
    payload.card_id = card_id;
    
    if (send_msg(server_socket, UtB_ACK_CARD, &payload, sizeof(payload)) < 0)
    perror("Errore nell'invio di SEND USER LIST\n");
    else
    printf("Inviato ACK per card %d\n", card_id);
}

void command_card_done_handler(int server_socket) {
    if (send_msg(server_socket, UtB_CARD_DONE, NULL, 0) < 0)
    perror("Errore nell'invio di SEND USER LIST\n");
    else
    printf("Inviato completamento lavoro (CARD_DONE).\n");
}
