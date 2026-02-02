#ifndef CLIENT_HANDLERS_H
#define CLIENT_HANDLERS_H

#include "../classes/user.h"
#include "../network/utils.h"

/**
 * @brief Gestisce input da tastiera
 * 
 * @param server_socket Descrittore del socket per comunicare col server
 *
 * @return 0 se bisogna chiudere il client, 1 altrimenti 
 */
int user_input_handler(int server_socket);

/**
 * @brief Gestisce messaggi in arrivo dalla lavagna
 * 
 * @param server_socket Descrittore del socket per comunicare col server
 *
 * @return 0 se bisogna chiudere il client, 1 altrimenti 
 */
int server_msg_handler(int server_socket);

/**
 * @brief Gestisce messaggi in arrivo da altri utenti
 * 
 * @param p2p_socket Descrittore del socket per comunicare con altri utenti
 * 
 */
void p2p_msg_handler(int p2p_socket);

#endif