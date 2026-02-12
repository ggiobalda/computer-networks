/**
 * @file cli_handlers.h
 * @author Giorgio Baldacci
 * @brief Funzioni handler chiamate dall'utente per gestire input terminale
 * 
 */
#ifndef CLI_HANDLERS_H
#define CLI_HANDLERS_H

#include "../../network/utils.h"

/**
 * @brief Gestisce input da tastiera
 * 
 * @param server_socket Descrittore socket del server
 *
 * @return 0 se bisogna chiudere il client, 1 altrimenti 
 */
int user_input_handler(int server_socket);

/**
 * @brief Gestisce comando QUIT inviando richiesta al server e terminando
 * 
 * @param server_socket Descrittore socket del server
 * 
 */
void command_quit_handler(int server_socket);

/**
 * @brief Gestisce comando SHOW LAVAGNA inviando richiesta al server
 * 
 * @param server_socket Descrittore socket del server
 * 
 */
void command_show_lavagna_handler(int server_socket);

/**
 * @brief Gestisce comando SEND USER LIST inviando richiesta al server
 * 
 * @param server_socket Descrittore di socket del server
 * 
 */
void command_send_user_list_handler(int server_socket);

/**
 * @brief Gestisce comando CREATE CARD
 * 
 * @param server_socket Descrittore di socket del server
 * @param args Argomento passato col comando (descrizione card)
 * 
 */
void command_create_card_handler(int server_socket, char* args);

/**
 * @brief Gestisce comando ACK CARD 
 * 
 * @param server_socket Descrittore di socket del server
 * @param args Argomento passato col comando (id card)
 * 
 */
void command_ack_card_handler(int server_socket, char* args);

/**
 * @brief Gestisce comando CARD DONE
 * 
 * @param server_socket Descrittore di socket del server
 * 
 */
void command_card_done_handler(int server_socket);

#endif