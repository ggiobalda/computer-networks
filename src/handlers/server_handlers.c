#include "../../include/handlers/server_handlers.h"

void server_hello_handler(Board* board, int socket, void* payload) {
    printf("[SERVER] Ricevuto HELLO da socket %d\n", socket);
    
    // registrazione utente
    MsgHelloPayload* new_user = (MsgHelloPayload*)payload;
    add_user(board, new_user->port, socket);

    printf("[SERVER] Utente con socket %d e porta %d aggiunto con successo\n", socket, new_user->port);
}

void server_quit_handler(Board* board, int socket, fd_set* master_fds) {
    printf("[SERVER] Ricevuto QUIT da socket %d\n", socket);

    // revoca card assegnata
    User* u = find_user_by_socket(board->users, socket);
    if (u->current_card_id != -1) {
        board_move_card(board, u->current_card_id, DOING, TODO, -1);
        printf("[SERVER] Card %d associata all'utente riposizionata in TODO\n", u->current_card_id);
    }
    
    // rimozione utente da lista, chiusura socket e pulizia fd set
    remove_user(board, socket);
    close(socket);
    FD_CLR(socket, master_fds);
}

void server_available_card_handler(Board* board) {
    if (board->n_users < 2)
        return;
    
    // estrazione card
    Card* task = extract_card_list(board, TODO);
    if (task == NULL)
        return;
    
    printf("[SERVER] Avvio asta per card %d\n", task->id);
    
    // preparazione payload
    MsgAvailableCardPayload payload;
    payload.card_id = task->id;
    strcpy(payload.description, task->description);
    payload.n_users = board->n_users;
    
    int i = 0;
    for (User* u = board->users; u != NULL; u = u->next)
        payload.users_ports[i++] = u->id;
    
    // invio payload a tutti gli utenti
    for (User* u = board->users; u != NULL; u = u->next)
        send_msg(u->socket, BtU_AVAILABLE_CARD, &payload, sizeof(payload));
}

void server_show_lavagna_handler(Board* board, int socket) {
    char buf[MAX_PAYLOAD_SIZE];

    // stringify board e invio
    board_to_string(board, buf, sizeof(buf));
    send_msg(socket, BtU_SHOW_LAVAGNA, buf, strlen(buf) + 1);
    
    printf("[SERVER] Inviato stato lavagna al socket %d\n", socket);
}

void server_send_user_list_handler(Board* board, int socket) {
    MsgUserListPayload payload;
    payload.n_users = 0;

    // popolamento lista e invio
    for (User* p = board->users; p != NULL && payload.n_users < MAX_USERS; p = p->next)
        payload.users_ports[payload.n_users++] = p->id;
    send_msg(socket, BtU_SEND_USER_LIST, &payload, sizeof(payload));

    printf("[SERVER] Inviata lista utenti al socket %d\n", socket);
}

void server_pong_handler(Board* board, int socket) {
    // recupera utente (se presente)
    User* u = find_user_by_socket(board->users, socket);
    if (u == NULL){
        printf("[SERVER] Ricevuto PONG da utente non presente in kanban\n");
        return; 
    }

    // resetta timer card
    Card* c = find_card(board->lists[DOING], u->current_card_id);
    if (c != NULL) {
        c->last_updated = time(NULL);
        printf("[SERVER] PONG da utente %d. Timer resettato per card %d.\n", u->id, c->id);
        return;
    }

    printf("[SERVER] PONG da utente %d, nessuna card associata\n", u->id);
}

void server_check_timeouts(Board* board) {
    time_t now = time(NULL);
    Card* c = board->lists[DOING];
    
    while (c != NULL) {
        Card* next = c->next;
        double elapsed = difftime(now, c->last_updated); // tempo trascorso da timestamp carta
        
        // caso timeout (trascorso tempo anche per PONG): revoca card
        if (elapsed > PING_INTERVAL + PONG_INTERVAL) {
            printf("[SERVER] TIMEOUT Card %d (User %d). Spostamento in TODO.\n", c->id, c->user_id);
            board_move_card(board, c->id, DOING, TODO, -1);
        } 
        // caso ping (trascorso tempo PING_INTERVAL)
        else if (elapsed > PING_INTERVAL) {
            // invio PING a utente associato alla card
            User* u = find_user_by_port(board->users, c->user_id);
            
            if (u != NULL) {
                send_msg(u->socket, BtU_PING, NULL, 0);
                printf("[SERVER] PING inviato a utente %d per Card %d\n", u->id, c->id);
            }
            else { // utente non presente in lista
                board_move_card(board, c->id, DOING, TODO, -1);
                printf("[SERVER] Card %d spostata in TODO data assenza di utente associato\n", c->id);
            }
        }
        
        c = next;
    }
}