#include "board-manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Board* create_board(int id, int port) {
    Board* new_board = (Board*)malloc(sizeof(Board));
    if (new_board == NULL) {
        perror("Malloc error in create_board: impossible to create a new board\n");
        return NULL;
    }

    // initialize card fields and return pointer to the new board
    new_board->id = id;
    new_board->port = port;
    new_board->todo_list = NULL;
    new_board->doing_list = NULL;
    new_board->done_list = NULL;
    return new_board;
}

void free_board(Board* board) {
    if (board != NULL) {
        free(board);
    }
}

void print_board(const Board* board) {
    if (board == NULL){
        printf("Board is NULL\n");
    }

    // pointer to scan the three lists
    Card* l = board->todo_list;
    Card* m = board->doing_list;
    Card* n = board->done_list;

    printf("\n");
    for (int i = 0; i < 80; i++)
        printf("-");
}