/**
 * @file board.h
 * @author your name (you@domain.com)
 * @brief Definitions for the Board structure and related functions
 * 
 */

#ifndef BOARD_H
#define BOARD_H

#include "card.h"

typedef struct Board {
    int id;
    Card* todo_head;
    Card* doing_head;
    Card* done_head;
} Board;

#endif