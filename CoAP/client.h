#ifndef CLIENT_H
#define CLIENT_H

#include <coap3/coap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define SERVER_HOST "192.168.248.235"
#define SERVER_PORT 5683
#define SERVER_URI_PATH "sensor"

// Function declarations
static coap_response_t message_handler(coap_session_t *session, const coap_pdu_t *sent,
                                       const coap_pdu_t *received, const coap_mid_t id);
#endif // CLIENT_H
