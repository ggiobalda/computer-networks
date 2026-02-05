#include "../../include/handlers/server_handlers.h"

void server_hello_handler(Board* board, int socket, void* payload) {
    printf("\n--- NUOVO UTENTE REGISTRATO ---\n");
    
    // registrazione utente
    MsgHelloPayload* new_user = (MsgHelloPayload*)payload;
    add_user(board, new_user->port, socket);

    printf("-> Utente aggiunto: Socket %d | Porta %d\n", socket, new_user->port);
    printf("-------------------------------\n\n");
}

void server_quit_handler(Board* board, int socket, fd_set* master_fds) {
    printf("\n!!! DISCONNESIONE UTENTE !!!\n");
    
    // recupero utente e controllo errore
    User* u = find_user_by_socket(board->users, socket);
    if (u == NULL) {
        printf("[WARNING] Socket %d chiuso ma utente non trovato in lista. Ignoro.\n", socket);
        close(socket);
        FD_CLR(socket, master_fds);
        return;
    }

    // revoca card assegnata
    if (u->current_card_id != -1) {
        board_move_card(board, u->current_card_id, DOING, TODO, -1);
        printf("-> Card %d abbandonata! Riposizionata in TODO.\n", u->current_card_id);
    }
    
    // rimozione utente da lista, chiusura socket e pulizia fd set
    remove_user(board, socket);
    close(socket);
    FD_CLR(socket, master_fds);

    printf("----------------------------\n");

    // avvio asta se possibile
    if (board->n_users > 1)
        server_available_card_handler(board);
    else
        board->auction_in_progress = 0;
}

void server_available_card_handler(Board* board) {
    // controlli: almeno due utenti e nessun'asta in corso
    if (board->n_users < 2) {
        board->auction_in_progress = 0;
        return;
    }
    if (board->auction_in_progress)
        return;
    
    // estrazione card
    Card* task = board->lists[TODO];
    if (task == NULL)
        return;
    
    // avvio asta
    board->auction_in_progress = 1;
    printf("\n\n========================================\n");
    printf("        AVVIO ASTA PER CARD %d\n", task->id);
    printf("========================================\n");
    printf("Descrizione: %s\n", task->description);
    
    // preparazione payload
    MsgAvailableCardPayload payload;
    payload.card_id = task->id;
    strncpy(payload.description, task->description, MAX_CARD_DESC_CHARS-1);
    payload.description[MAX_CARD_DESC_CHARS-1] = '\0';
    
    int i = 0;
    for (User* u = board->users; u != NULL; u = u->next) {
        if (i >= MAX_USERS) {
            printf("Warning: Numero utenti > MAX_USERS. Tronco la lista.\n");
            break;
        }
        payload.users_ports[i++] = u->id;
    }
    payload.n_users = i;
    
    // invio payload a tutti gli utenti
    for (User* u = board->users; u != NULL; u = u->next)
        send_msg(u->socket, BtU_AVAILABLE_CARD, &payload, sizeof(payload));
    
    printf("-> Invito inviato a %d utenti.\n", board->n_users);
}

void server_show_lavagna_handler(Board* board, int socket) {
    char buf[MAX_PAYLOAD_SIZE];

    // stringify board e invio
    board_to_string(board, buf, sizeof(buf));
    send_msg(socket, BtU_SHOW_LAVAGNA, buf, strlen(buf) + 1);
    
    printf("Inviato stato lavagna al socket %d\n", socket);
}

void server_send_user_list_handler(Board* board, int socket) {
    MsgUserListPayload payload;
    payload.n_users = 0;

    // popolamento lista e invio
    for (User* p = board->users; p != NULL && payload.n_users < MAX_USERS; p = p->next)
        payload.users_ports[payload.n_users++] = p->id;
    send_msg(socket, BtU_SEND_USER_LIST, &payload, sizeof(payload));

    printf("Inviata lista utenti al socket %d\n", socket);
}

void server_pong_handler(Board* board, int socket) {
    // recupera utente (se presente)
    User* u = find_user_by_socket(board->users, socket);
    if (u == NULL){
        printf("Ricevuto PONG da utente non presente in kanban\n");
        return; 
    }

    // resetta timer card
    Card* c = find_card(board->lists[DOING], u->current_card_id);
    if (c != NULL) {
        c->last_updated = time(NULL);
        printf("Ricevuto PONG da utente %d. Timer resettato per card %d.\n", u->id, c->id);
        return;
    }

    printf("Ricevuto PONG da utente %d, nessuna card associata\n", u->id);
}

void server_check_timeouts(Board* board) {
    time_t now = time(NULL);
    Card* c = board->lists[DOING];
    
    while (c != NULL) {
        Card* next = c->next;
        double elapsed = difftime(now, c->last_updated); // tempo trascorso da timestamp carta
        
        // caso timeout (trascorso tempo anche per PONG): revoca card
        if (elapsed > PING_INTERVAL + PONG_INTERVAL) {
            printf("TIMEOUT Card %d (User %d). Spostamento in TODO.\n", c->id, c->user_id);
            board_move_card(board, c->id, DOING, TODO, -1);
        } 
        // caso ping (trascorso tempo PING_INTERVAL)
        else if (elapsed > PING_INTERVAL) {
            // invio PING a utente associato alla card
            User* u = find_user_by_port(board->users, c->user_id);
            
            if (u != NULL) {
                send_msg(u->socket, BtU_PING, NULL, 0);
                printf("PING inviato a utente %d per Card %d\n", u->id, c->id);
            }
            else { // utente non presente in lista
                board_move_card(board, c->id, DOING, TODO, -1);
                printf("Card %d spostata in TODO data assenza di utente associato\n", c->id);
            }
        }
        
        c = next;
    }
}

void server_create_card_handler(Board* board, int socket, void* payload) {
    MsgCreateCardPayload* msg = (MsgCreateCardPayload*)payload;
    add_card(board, TODO, msg->description);
    
    printf("Creata nuova card: %s (richiesta da socket %d)\n", msg->description, socket);
}

void server_ack_card_handler(Board* board, int socket, void* payload) {
    MsgAckCardPayload* msg = (MsgAckCardPayload*)payload;
    
    // recupero utente e controllo se disponibile
    User* u = find_user_by_socket(board->users, socket);
    if (u == NULL)
        return;
    if (u->current_card_id != -1) {
        printf("Utente %d ha già la card %d, non può prenderne un'altra!\n", u->id, u->current_card_id);
        return;
    }

    // assegna card, aggiorna stato utente e lavagna
    board_move_card(board, msg->card_id, TODO, DOING, u->id);
    u->current_card_id = msg->card_id;
    board->auction_in_progress = 0;
    
    printf("\nCARD ASSEGNATA: Card %d -> Utente %d (DOING)\n", msg->card_id, u->id);
}

void server_card_done_handler(Board* board, int socket) {
    // recupero utente e controllo
    User* u = find_user_by_socket(board->users, socket);
    if (u == NULL) return;
    if (u->current_card_id == -1) {
        printf("Utente %d ha inviato DONE ma non ha card assegnate.\n", u->id);
        return;
    }

    // recupero card e controllo
    Card* c = find_card(board->lists[DOING], u->current_card_id);
    if (c == NULL) {
        printf("Card %d non trovata in DOING!\n", u->current_card_id);
        u->current_card_id = -1; // reset forzato
        return;
    }

    // sposto card in done e aggiorno stato utente
    board_move_card(board, u->current_card_id, DOING, DONE, u->id);
    u->current_card_id = -1;

    printf("\nCARD COMPLETATA: Card %d (Utente %d) -> Spostata in DONE\n", c->id, u->id);

    // avvia nuova asta 
    server_available_card_handler(board);
}