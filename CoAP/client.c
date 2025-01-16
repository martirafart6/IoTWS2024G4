#include <coap3/coap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_URI "coap://127.0.0.1/sensor"

static coap_response_t message_handler(coap_session_t *session, const coap_pdu_t *sent,
                                       const coap_pdu_t *received, const coap_mid_t id) {
    const uint8_t *data;
    size_t data_len;

    if (coap_get_data(received, &data_len, &data)) {
        printf("Response: %.*s\n", (int)data_len, data);
    } else {
        printf("No data received.\n");
    }

    return COAP_RESPONSE_OK;
}

int main(void) {
    coap_startup(); // Initialize libcoap
    coap_set_log_level(LOG_INFO); // Set logging level to show information

    coap_context_t *ctx = coap_new_context(NULL); // Create a CoAP context
    if (!ctx) {
        fprintf(stderr, "Failed to create CoAP context.\n");
        return -1;
    }

    coap_address_t dst;
    coap_address_init(&dst);
    //coap_address_set_port(&dst, 5683); // Default CoAP port
    dst.addr.sin.sin_family = AF_INET; // Force IPv4
    //dst.addr.sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    //dst.addr.sin.sin_port = htons(5683);

    coap_session_t *session = coap_new_client_session(ctx, NULL, &dst, COAP_PROTO_UDP);
    if (!session) {
        fprintf(stderr, "Failed to create CoAP session.\n");
        coap_free_context(ctx);
        return -1;
    }

    while (1) {
        coap_pdu_t *request = coap_pdu_init(COAP_MESSAGE_CON, COAP_REQUEST_GET, coap_new_message_id(session), coap_session_max_pdu_size(session));
        if (!request) {
            fprintf(stderr, "Failed to create CoAP request PDU.\n");
            break;
        }

        coap_add_option(request, COAP_OPTION_URI_PATH, strlen("sensor"), (const uint8_t *)"sensor");
        coap_register_response_handler(ctx, message_handler);

        if (coap_send(session, request) == COAP_INVALID_TID) {
            fprintf(stderr, "Failed to send request.\n");
            break;
        }

        printf("Request sent to %s. Waiting for response...\n", SERVER_URI);
        coap_io_process(ctx, 5000); // Process IO (wait up to 5 seconds for response)
        sleep(5); // Wait before sending the next request
    }

    coap_session_release(session);
    coap_free_context(ctx);
    coap_cleanup(); // Cleanup libcoap resources

    return 0;
}
