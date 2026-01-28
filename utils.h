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
int next_id = 0;

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

const char* ColumnNames[] = {"TO DO", "DOING", "DONE"};

#endif