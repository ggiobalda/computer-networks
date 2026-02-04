#include "../../../include/handlers/client/server_msg_handlers.h"
#include "../../../include/handlers/client/p2p_auction_handlers.h"

int server_msg_handler(int server_socket, int my_port) {
    // ricezione header messaggio e gestione errori
    MsgHeader head;
    char buffer[MAX_PAYLOAD_SIZE];
    int ret = recv_msg(server_socket, &head, buffer, sizeof(buffer));
    if (ret <= 0) {
        printf("Lavagna disconnessa. Terminazione.\n");
        return 0;
	}

    // switch in base all'header
    switch (head.type) {
        case BtU_AVAILABLE_CARD:
            response_available_card_handler(&head, buffer, server_socket, my_port);
            break;
        
        case BtU_PING:
            send_msg(server_socket, UtB_PONG, NULL, 0);
            printf("Risposto al PING della lavagna.\n");
            break;
        
        case BtU_SHOW_LAVAGNA:
            response_show_lavagna_handler(&head, buffer);
            break;
        
        case BtU_SEND_USER_LIST:
            response_send_user_list_handler(&head, buffer);
            break;
        
        default:
            printf("Messaggio tipo %d ignorato.\n", head.type);
            break;
    }
    return 1;
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
