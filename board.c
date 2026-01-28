#include "board.h"

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
    new_board->users_ports = NULL;
    for (int i = 0; i < N_COLUMNS; i++)
        new_board->lists[i] = NULL;
    
    return new_board;
}

void free_board(Board* board) {
    if (board != NULL) {
        free(board);
    }
}

void print_board(const Board* board) {
    if (board == NULL){
        perror("Board NULL\n");
        return;
    }

    for (int i = TODO; i <= DONE; i++) {
        printf("%s\n", ColumnNames[i]);

        for (Card* p = board->lists[i]; p != NULL; p = p->next) {
            printf("\n");
            print_card(p);
        }
        printf("Colonna %s terminata", ColumnNames[i]);
    }

}

void add_card(Board* board, Column column, const char* description) {
    Card* c = create_card(next_id++, column, description);
    
    // caso colonna vuota
    Card* p = board->lists[column];
    if (p == NULL) {
        board->lists[column] = c;
        return;
    }

    while (p->next != NULL)
        p = p->next;
    
    p->next = c;

}