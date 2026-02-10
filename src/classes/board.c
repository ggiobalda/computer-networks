#include "../../include/classes/board.h"

// contatore incrementale per id cards
int next_id = 0;

/**
 * @brief Funzione helper interna per recuperare card senza rimuoverla
 * 
 * @param head Puntatore a prima card della lista
 * @param index Posizione in lista della card desiderata
 * 
 * @return Puntatore alla card 
 * 
 */
Card* get_card_at(Card* head, int index) {
    int count = 0;
    while (head != NULL) {
        if (count == index) return head;
        head = head->next;
        count++;
    }
    return NULL;
}

/**
 * @brief Funzione helper interna per contare le card in una lista
 * 
 * @param head Puntatore a prima card della lista
 * 
 * @return Numero cards in lista
 * 
 */
int count_cards(Card* head) {
    int count = 0;
    while (head != NULL) {
        head = head->next;
        count++;
    }
    return count;
}

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
    int offset = 0;
    
    // titolo
    offset += snprintf(buffer + offset, max_len - offset, "\n=========================================== LAVAGNA ============================================\n\n");

    // imposto formato: 30chars | 30chars | 30chars
    const char* row_fmt = "%-30s | %-30s | %-30s\n";
    const char* empty_cell = ""; 

    // stampa intestazioni
    offset += snprintf(buffer + offset, max_len - offset, row_fmt, "            TO DO", "            DOING", "             DONE");
    offset += snprintf(buffer + offset, max_len - offset, row_fmt, "------------------------------", "------------------------------", "------------------------------");

    // calcolo max righe
    int n_todo = count_cards(board->lists[TODO]);
    int n_doing = count_cards(board->lists[DOING]);
    int n_done = count_cards(board->lists[DONE]);
    int max_rows = n_todo;
    if (n_doing > max_rows) max_rows = n_doing;
    if (n_done > max_rows) max_rows = n_done;

    // buffer temporanei per le celle (line1: ID/User, line2: descrizione)
    char todo_line1[MAX_CARD_DESC_CHARS+1], todo_line2[MAX_CARD_DESC_CHARS+1];
    char doing_line1[MAX_CARD_DESC_CHARS+1], doing_line2[MAX_CARD_DESC_CHARS+1];
    char done_line1[MAX_CARD_DESC_CHARS+1], done_line2[MAX_CARD_DESC_CHARS+1];

    // ciclo di stampa card per card
    for (int i = 0; i < max_rows; i++) {
        Card* c;

        // --- COLONNA TODO ---
        c = get_card_at(board->lists[TODO], i);
        if (c) {
            snprintf(todo_line1, sizeof(todo_line1), "[ID: %d]", c->id); // solo ID, niente user in TODO
            snprintf(todo_line2, sizeof(todo_line2), "%s", c->description);
        } else {
            strcpy(todo_line1, empty_cell);
            strcpy(todo_line2, empty_cell);
        }

        // --- COLONNA DOING ---
        c = get_card_at(board->lists[DOING], i);
        if (c) {
            snprintf(doing_line1, sizeof(doing_line1), "[ID: %d] User: %d", c->id, c->user_id);
            snprintf(doing_line2, sizeof(doing_line2), "%s", c->description);
        } else {
            strcpy(doing_line1, empty_cell);
            strcpy(doing_line2, empty_cell);
        }

        // --- COLONNA DONE ---
        c = get_card_at(board->lists[DONE], i);
        if (c) {
            snprintf(done_line1, sizeof(done_line1), "[ID: %d] User: %d", c->id, c->user_id);
            snprintf(done_line2, sizeof(done_line2), "%s", c->description);
        } else {
            strcpy(done_line1, empty_cell);
            strcpy(done_line2, empty_cell);
        }

        // stampa effettiva delle due righe + riga vuota separatrice
        offset += snprintf(buffer + offset, max_len - offset, row_fmt, todo_line1, doing_line1, done_line1);
        offset += snprintf(buffer + offset, max_len - offset, row_fmt, todo_line2, doing_line2, done_line2);
        offset += snprintf(buffer + offset, max_len - offset, row_fmt, "", "", "");

        // controllo buffer pieno
        if (offset >= max_len - 100) {
            offset += snprintf(buffer + offset, max_len - offset, "\n[... Buffer Pieno ...]\n");
            break;
        }
    }

    offset += snprintf(buffer + offset, max_len - offset, "\nUtenti connessi: %d", board->n_users);
    offset += snprintf(buffer + offset, max_len - offset, "\n================================================================================================\n");
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
    card->last_updated = time(NULL); // reset timer per ping
    card->next = NULL;

    // caso particolare: da DOING a TODO la card va messa in testa invece che in coda
    if (from == DOING && to == TODO) {
        card->next = board->lists[to];
        board->lists[to] = card;
        return;
    }

    // aggiunta in coda alla lista to
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