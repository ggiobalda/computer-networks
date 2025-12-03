/**
 * @file card.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Definitions for the Board structure and related functions
 * 
 */
#ifndef BOARD_H
#define BOARD_H

#include "../common/card.h"

/**
 * @brief Kanban board structure
 * 
 * @param id Unique identifier for the board
 * @param port Network port associated with the board
 * @param todo_list Pointer to the head of the TODO column linked list
 * @param doing_list Pointer to the head of the DOING column linked list
 * @param done_list Pointer to the head of the DONE column linked list
 * 
 */
typedef struct Board {
    int id;
    int port;
    Card* todo_list;
    Card* doing_list;
    Card* done_list;
} Board;

/**
 * @brief Create a new board object
 * 
 * @param id New board's id, must be unique
 * @param port New board's port
 * @return Board* - Pointer to newly created board, NULL on error
 * 
 */
Board* create_board(int id, int port);

/**
 * @brief Free memory allocated for board object
 * 
 * @param board Pointer to board
 */
void free_board(Board* board);

/**
 * @brief Print full board
 * 
 * @param board Pointer to board
 */
void print_board(const Board* board);



#endif