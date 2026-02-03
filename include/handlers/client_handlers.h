#ifndef CLIENT_HANDLERS_H
#define CLIENT_HANDLERS_H

#include "../classes/user.h"
#include "../network/utils.h"

/**
 * @brief Gestisce input da tastiera
 * 
 * @param server_socket Descrittore socket del server
 *
 * @return 0 se bisogna chiudere il client, 1 altrimenti 
 */
int user_input_handler(int server_socket);

/**
 * @brief Gestisce messaggi in arrivo dalla lavagna
 * 
 * @param server_socket Descrittore socket del server
 *
 * @return 0 se bisogna chiudere il client, 1 altrimenti 
 */
int server_msg_handler(int server_socket);

/**
 * @brief Gestisce messaggi in arrivo da altri utenti
 * 
 * @param p2p_socket Descrittore socket per comunicare con altri utenti
 * 
 */
void p2p_msg_handler(int p2p_socket);

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

#endif