#include "../../include/handlers/server_handlers.h"

/**
 * @brief 
 * 
 * @param board 
 * @param socket 
 */
void server_hello_handler(Board* board, int socket, void* payload) {
    printf("[SERVER] Ricevuto HELLO da socket %d\n", socket);
    
    MsgHelloPayload* new_user = (MsgHelloPayload*)payload;
    add_user(board, new_user->port, socket);

    printf("[SERVER] Utente con socket %d e porta %d aggiunto con successo\n", socket, new_user->port);
}

void server_quit_handler(Board* board, int socket, fd_set* master_fds) {
    printf("[SERVER] Ricevuto QUIT da socket %d\n", socket);

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
    char buf[MAX_PAYLOAD];

    board_to_string(board, buf, sizeof(buf));
    send_msg(socket, BtU_SHOW_LAVAGNA, buf, strlen(buf) + 1);
    
    printf("[SERVER] Inviato stato lavagna al socket %d\n", socket);
}