#include <WiFiSTA.h>
#include <WiFi.h>
#include <WiFiScan.h>
#include <WiFiMulti.h>
#include <WiFiAP.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <WiFiGeneric.h>
#include <WiFiClient.h>
#include <WiFiType.h>

#include <coap-simple.h>


// WiFi credentials
const char* ssid = "ines";
const char* password = "inesines";

// CoAP client instance
WiFiUDP udp;
Coap coapClient(udp); // Inicializamos Coap con un objeto UDP
IPAddress serverIP(192, 168, 248, 235); // Cambia por la IP de tu servidor CoAP
const int serverPort = 5683;          // Puerto del servidor CoAP

float generateRandomTemperature() {
  return random(150, 550) / 10.0; // Genera una temperatura entre 15.0 y 55.0
}

// Callback para manejar las respuestas del servidor
void responseCallback(CoapPacket &packet, IPAddress ip, int port) {
  Serial.print("Response from server: ");
  for (int i = 0; i < packet.payloadlen; i++) {
    Serial.print((char)packet.payload[i]);
  }
  Serial.println();
}

// Función para enviar la solicitud PUT
void sendCoapPutRequest() {
  float temperature = generateRandomTemperature();
  char payload[64];
  snprintf(payload, sizeof(payload), "{\"temperature\": %.1f}", temperature);

  Serial.printf("Sending PUT request with payload: %s\n", payload);

  coapClient.put(serverIP, serverPort, "sensor", payload, strlen(payload));
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Conectar a WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // Inicializar el cliente CoAP
  coapClient.response(responseCallback);
}

void loop() {
  sendCoapPutRequest();
  delay(5000); // Envía una solicitud cada 5 segundos

  coapClient.loop();
}
