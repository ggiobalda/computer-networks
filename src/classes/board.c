#include "../../include/classes/board.h"

int next_id = 0;

Board* create_board(int port) {
    // allocazione memoria
    Board* new_board = (Board*)malloc(sizeof(Board));
    if (new_board == NULL) {
        perror("Errore allocazione memoria per struttura Board\n");
        return NULL;
    }

    // inizializza campi e ritorna puntatore
    new_board->id = port;
    new_board->n_users = 0;
    new_board->users = NULL;
    new_board->auction_in_progress = 0;
    for (int i = 0; i < N_COLUMNS; i++)
        new_board->lists[i] = NULL;
    
    return new_board;
}

void free_board(Board* board) {
    // deallocazione lista utenti
    User* u = board->users;
    User* v;
    while (u != NULL) {
        v = u->next;
        free_user(u);
        u = v;
    }

    // deallocazione liste card
    for (int i = 0; i < 3; i++) {
        Card* p = board->lists[i];
        while (p != NULL) {
            Card* q = p->next;
            free_card(p);
            p = q;
        }
    }

    if (board != NULL) {
        free(board);
    }
}

void board_to_string(const Board* board, char* buffer, int max_len) {
    int offset = 0; // contatore posizione nel buffer

    offset += snprintf(buffer + offset, max_len - offset, "\n===== STATO LAVAGNA =====\n");

    for (int i = TODO; i <= DONE; i++) {
        offset += snprintf(buffer + offset, max_len - offset, "\n--- %s ---\n", ColumnNames[i]);
        
        Card* p = board->lists[i];
        if (p == NULL) {
             offset += snprintf(buffer + offset, max_len - offset, "(Vuota)\n");
             continue;
        }
        
        while (p != NULL) {
            // formattazione singola card
            offset += snprintf(buffer + offset, max_len - offset, 
                                   "[ID: %d] %s (User port: %d)\n", 
                                   p->id, p->description, p->user_id);
            p = p->next;
                
            if (offset >= max_len - 1) // controllo overflow
                return;
        }
    }

    offset += snprintf(buffer + offset, max_len - offset, "\n Numero utenti: %d", board->n_users);
    offset += snprintf(buffer + offset, max_len - offset, "\n=========================\n\n");
}

void add_card(Board* board, Column column, const char* description) {
    Card* c = create_card(next_id++, column, description);
    Card* p = board->lists[column];
    
    // caso colonna vuota
    if (p == NULL) {
        board->lists[column] = c;
        return;
    }

    // caso normale
    while (p->next != NULL)
        p = p->next;
    
    p->next = c;
}

void add_user(Board* board, int port, int socket) {
    User* u = create_user(port, socket);
    User* p = board->users;

    // caso lista vuota
    if (p == NULL) {
        board->users = u;
        board->n_users++;
        return;
    }

    // caso normale
    while (p->next != NULL)
        p = p->next;

    p->next = u;
    board->n_users++;
}

void remove_user(Board* board, int socket) {
    User* p = board->users;
    
    // caso utente in testa
    if (p->socket == socket) {
        board->users = p->next;
        free_user(p);
        
        board->n_users--;
        return;
    }
    
    User* q = p->next;
    while (q != NULL && q->socket != socket) {
        p = q;
        q = q->next;
    }

    // caso utente non presente
    if (q == NULL) {
        printf("Utente non presente in lista\n");
        return;
    }

    // caso normale
    p->next = q->next;
    free_user(q);
    board->n_users--;
}

Card* extract_card_list(Board* board, Column column) {
    Card* p = board->lists[column];

    // caso lista vuota
    if (p == NULL)
        return NULL;
    
    board->lists[column] = p->next;
    return p;
}

void board_move_card(Board* board, int card_id, Column from, Column to, int user_id) {
    Card* p = NULL;
    Card* card = board->lists[from];

    while (card != NULL && card->id != card_id) {
        p = card;
        card = card->next;
    }

    // card non presente
    if (card == NULL) {
        printf("Card non presente in lista\n");
        return;
    }

    // rimozione card da lista from
    if (p == NULL) 
        board->lists[from] = card->next;
    else 
        p->next = card->next;

    // aggiorna dati card
    card->column = to;
    card->user_id = user_id;
    card->last_updated = time(NULL); // resete timer per ping
    card->next = NULL;

    // aggiunta alla lista to
    Card* dest = board->lists[to];
    if (dest == NULL) {
        board->lists[to] = card;
    } else {
        while (dest->next != NULL) {
            dest = dest->next;
        }
        dest->next = card;
    }
}