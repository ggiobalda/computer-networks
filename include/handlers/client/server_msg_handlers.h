#ifndef SERVER_MSG_HANDLERS_H
#define SERVER_MSG_HANDLERS_H

#include "../../network/utils.h"

/**
 * @brief Gestisce messaggi in arrivo dalla lavagna
 * 
 * @param server_socket Descrittore socket del server
 * @param my_port Porta dell'utente chiamante
 *
 * @return 0 se bisogna chiudere il client, 1 altrimenti 
 */
int server_msg_handler(int server_socket, int my_port);

/**
 * @brief Gestisce risposta inviata dal server in seguito al comando SHOW LAVAGNA
 * 
 * @param head Puntatore all'header della risposta
 * @param payload Puntatore al payload della risposta
 *  
 */
void response_show_lavagna_handler(MsgHeader* head, char* payload);

/**
 * @brief Gestisce risposta inviata dal server in seguito al comando SEND USER LIST
 * 
 * @param head Puntatore all'header della risposta
 * @param payload Puntatore al payload della risposta
 *  
 */
void response_send_user_list_handler(MsgHeader* head, char* payload);

#endif