#include "client.h"


static coap_response_t message_handler(coap_session_t *session, const coap_pdu_t *sent,
                                       const coap_pdu_t *received, const coap_mid_t id) {
    const uint8_t *data;
    size_t data_len;

    if (coap_get_data(received, &data_len, &data)) {
        printf("Response: %.*s\n", (int)data_len, data);
    } else {
        printf("No data received in response.\n");
    }

    return COAP_RESPONSE_OK;
}

int main(void) {
    coap_startup();

    coap_context_t *context = coap_new_context(NULL);
    if (!context) {
        fprintf(stderr, "Failed to create CoAP context\n");
        return -1;
    }

    coap_address_t server_address;
    coap_address_init(&server_address);
    server_address.addr.sin.sin_family = AF_INET;
    server_address.addr.sin.sin_port = htons(SERVER_PORT);
    server_address.addr.sin.sin_addr.s_addr = inet_addr(SERVER_HOST);

    coap_session_t *session = coap_new_client_session(context, NULL, &server_address, COAP_PROTO_UDP);
    if (!session) {
        fprintf(stderr, "Failed to create CoAP session\n");
        coap_free_context(context);
        return -1;
    }

    srand(time(NULL)); // Seed for random temperature generation

    while (1) {  // Infinite loop to send requests continuously
        float temperature = (rand() % 400) / 10.0; // Random temperature between 0.0 and 40.0

        char payload[64];
        snprintf(payload, sizeof(payload), "Temperature: %.1f", temperature);

        coap_pdu_t *pdu = coap_pdu_init(COAP_MESSAGE_CON, COAP_REQUEST_PUT, coap_new_message_id(session), coap_session_max_pdu_size(session));
        if (!pdu) {
            fprintf(stderr, "Failed to create CoAP PDU\n");
            coap_session_release(session);
            coap_free_context(context);
            return -1;
        }

        coap_add_option(pdu, COAP_OPTION_URI_PATH, strlen(SERVER_URI_PATH), (const uint8_t *)SERVER_URI_PATH);
        coap_add_data(pdu, strlen(payload), (const uint8_t *)payload);

        coap_register_response_handler(context, message_handler);

        if (coap_send(session, pdu) == COAP_INVALID_MID) {
            fprintf(stderr, "Failed to send CoAP request\n");
            coap_session_release(session);
            coap_free_context(context);
            return -1;
        }

        printf("Sent PUT request with payload: %s\n", payload);

        // Wait for a short time before sending the next request
        sleep(2);  // Adjust the delay as needed (in seconds)

        coap_run_once(context, 0);  // Process incoming responses
    }

    coap_session_release(session);
    coap_free_context(context);
    coap_cleanup();

    return 0;
}


