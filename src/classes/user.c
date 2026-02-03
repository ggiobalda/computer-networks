#include "../../include/classes/user.h"

User* create_user(int port, int socket){
    // allocazione memoria
    User* new_user = (User*)malloc(sizeof(User));
    if (new_user == NULL) {
        perror("Errore allocazione memoria per struttura user\n");
        return NULL;
    }

    // inizializza campi e ritorna puntatore
    new_user->id = port;
    new_user->current_card_id = -1;
    new_user->socket = socket;
    new_user->next = NULL;
    
    return new_user;
}

void free_user(User* user) {
    if (user != NULL)
        free(user);
}

User* find_user_by_port(User* user_list, int port) {
    User* u = user_list;
    while (u != NULL && u->id != port)
        u = u->next;

    return u;
}

User* find_user_by_socket(User* user_list, int socket) {
    User* u = user_list;
    while (u != NULL && u->socket != socket)
        u = u->next;

    return u;
}