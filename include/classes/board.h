/**
 * @file board.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Definizioni per la struct Board e relative funzioni
 * 
 */
#ifndef BOARD_H
#define BOARD_H

#include "../network/utils.h"
#include "./card.h"
#include "./user.h"

/**
 * @brief Lavagna del kanban
 * 
 * @param id Numero di portqa
 * @param n_users Numero di utenti attualmente connessi
 * @param users Lista degli utenti connessi
 * @param lists Array di liste di carte, una per colonna
 * @param auction_in_progress Flag per indicare se c'è un asta in corso 
 * 
 */
typedef struct Board {
    int id;
    int n_users;
    User* users;
    Card* lists[N_COLUMNS];
    int auction_in_progress;
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
 * @brief Formatta lavagna come stringa su buffer per poter essere stampata
 * 
 * @param board Puntatore alla lavagna
 * @param buffer Puntatore al buffer
 * @param max_len Dimensione massima stringa
 * 
 */
void board_to_string(const Board* board, char* buffer, int max_len);

/**
 * @brief Crea nuova card e aggiunge in fondo alla colonna indicata
 * 
 * @param board Puntatore alla lavagna
 * @param column Colonna in cui inserire la card
 * @param description Descrizione estesa della card
 * 
 */
void add_card(Board* board, Column column, const char* description);

/**
 * @brief Rimuovi la card in testa alla lista
 * 
 * @param board Puntatore alla lavagna
 * @param column Colonna da cui estrarre
 
 * @return Puntatore alla card estratta, NULL se lista vuota
 */
Card* extract_card_list(Board* board, Column column);

/**
 * @brief Crea nuovo utente e aggiunge in fondo alla lista
 * 
 * Aggiorna il contatore di utenti connessi
 * 
 * @param board Puntatore alla lavagna
 * @param port ID del nuovo utente
 * @param socket Descrittore del socket dell'utente
 * 
 */
void add_user(Board* board, int port, int socket);

/**
 * @brief Cerca ed elimina utente dalla lista
 * 
 * Aggiorna il contatore di utenti connessi
 * 
 * @param board Puntatore alla lavagna
 * @param socket Descrittore del socket dell'utente
 * 
 */
void remove_user(Board* board, int socket);

/**
 * @brief Sposta card fra colonne
 * 
 * @param board Puntatore alla lavagna
 * @param card_id ID della card
 * @param from Colonna di origine
 * @param to Colonna di destinazione
 * @param user_id Utente associato alla card
 * 
 */
void board_move_card(Board* board, int card_id, Column from, Column to, int user_id);

#endif