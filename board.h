/**
 * @file board.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Definizioni per la struct Board e relative funzioni
 * 
 */
#ifndef BOARD_H
#define BOARD_H

#include "utils.h"
#include "card.h"

/**
 * @brief Lavagna del kanban
 * 
 * @param id Numero di portqa
 * @param n_users Numero di utenti attualmente connessi
 * @param users_ports Array contenente porte degli utenti
 * @param lists Array di liste di carte, una per colonna
 */
typedef struct Board {
    int id;
    int n_users;
    int* users_ports;
    Card* lists[N_COLUMNS];
} Board;

/**
 * @brief Costruttore lavagna
 * 
 * @param port Porta di rete, usata come ID della lavagna
 * 
 * @return Puntatore alla lavagna appena creata, NULL in caso di errore
 * 
 */
Board* create_board(int port);

/**
 * @brief Distruttore lavagna
 * 
 * @param board Puntatore alla lavagna
 * 
 */
void free_board(Board* board);

/**
 * @brief Stampa lavagna intera
 * 
 * @param board Puntatore alla lavagna
 * 
 */
void print_board(const Board* board);

/**
 * @brief Crea nuova card e la aggiunge in fondo alla colonna indicata
 * 
 * @param board Puntatore alla lavagna
 * @param column Colonna in cui inserire la card
 * @param description Descrizione estesa della card
 * 
 */
void add_card(Board* board, Column column, const char* description);

#endif