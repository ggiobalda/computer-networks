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