#ifndef SERVER_HANDLERS_H
#define SERVER_HANDLERS_H

#include "../classes/board.h"
#include "../network/utils.h"

/**
 * @brief Gestisce richiesta di HELLO di un nuovo utente
 * 
 * Legge il payload e aggiunge utente alla lista
 * 
 * @param board Puntatore alla lavagna
 * @param socket Descrittore del socket dell'utente
 * @param payload Puntatore al buffer contenente il corpo del messaggio
 * 
 */
void server_hello_handler(Board* board, int socket, void* payload);

/**
 * @brief Gestisce richiesta di QUIT di un utente o disconnessione
 * 
 * Identifica utente tramite socket, rimuove dalla board e chiude socket
 * 
 * @param board Puntatore alla lavagna
 * @param socket Descrittore del socket dell'utente
 * @param master_fds Puntatore alla lista socket
 * 
 */
void server_quit_handler(Board* board, int socket, fd_set* master_fds);

/**
 * @brief Avvia asta per prima card in todo
 * 
 * @param board Puntatore alla lavagna
 */
void server_available_card_handler(Board* board);

/**
 * @brief Gestisce richiesta di SHOW LAVAGNA di un utente
 * 
 * @param board Puntatore alla lavagna
 * @param socket Descrittore del socket dell'utente
 * 
 */
void server_show_lavagna_handler(Board* board, int socket);

/**
 * @brief Gestisce richiesta di SEND LIST USER di un utente
 * 
 * @param board Puntatore alla lavagna
 * @param socket Descrittore del socket dell'utente
 *  
 */
void server_send_user_list_handler(Board* board, int socket);

#endif