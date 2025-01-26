#include <coap-simple.h>

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

// WiFi credentials
const char* ssid     = "OEAD";
const char* password = "Welcome@OEAD";

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port);

// UDP and CoAP classes
WiFiUDP udp;
Coap coap(udp);

// Simulate a temperature sensor
float generateRandomTemperature() {
  return random(150, 550) / 10.0; // Generates a temperature between 15.0 and 55.0
}

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response]");
  
  char response[packet.payloadlen + 1];
  memcpy(response, packet.payload, packet.payloadlen);
  response[packet.payloadlen] = NULL;  
  
  Serial.println(response);
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up CoAP client response callback
  coap.response(callback_response);

  // Start CoAP server/client
  coap.start();
}

void loop() {
  delay(1000);
  coap.loop();

  // Simulate and send temperature data
  float temperature = generateRandomTemperature();
  String tempString = String(temperature, 1); // Convert float to String
  Serial.println("Sending temperature: " + tempString);

  coap.put(IPAddress(10, 10, 5, 199), 5683, "sensor", tempString.c_str());
}
