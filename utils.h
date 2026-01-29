/**
 * @file consts.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Costanti utili per il progetto
 * 
 */
#ifndef CONSTS_H
#define CONSTS_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

// lavagna
#define BOARD_PORT 5678
#define BOARD_ADDRESS "127.0.0.1"

// utenti
#define MIN_PORT 5679

// card
#define MAX_CHARS 30

// colonne
#define N_COLUMNS 3

/**
 * @brief Colonne nella lavagna
 *
 */
typedef enum Column {
    TODO,
    DOING,
    DONE
} Column;

/**
 * @brief Board to User messages
 *
 * @param BtU_AVAILABLE_CARD 
 */
typedef enum {
    BtU_AVAILABLE_CARD,
    BtU_PING
} BtU_msg;

/**
 * @brief User to Board messages
 *
 */
typedef enum {
    UtB_HELLO,
    UtB_QUIT,
    UtB_SHOW_BOARD,
    UtB_PONG
    UtB_CREATE_CARD,
    UtB_ACK_CARD,
    UtB_CARD_DONE
} UtB_msg;

/**
 * @brief User to User messages
 *
 */
 typedef enum {
    UtU_COST
} UtB_msg;

extern const char* ColumnNames[];

#endif