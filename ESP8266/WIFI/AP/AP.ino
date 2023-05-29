#include <ESP8266WiFi.h>
#include <WiFiClient.h>

IPAddress apIP(192, 168, 4, 1);
const char* apSSID = "ABC";
const char* apPassword = "123";

WiFiServer server(80);
int serverPort = 80;

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID, apPassword);
  WiFi.enableAP(true);

  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();

}

void loop() {
    WiFiClient client = server.available();

    if (client) {
        //while (client.connected()) {
          //if (client.available()) {
          //  String request = client.readStringUntil('\r');
          //  Serial.print("Received data: ");
          //  Serial.println(request);

            // Respond to the request
            String response = "Port " + String(serverPort);
            Serial.println(response);
          //  client.print(response);

        //    break;
        //  }
        //}
        client.stop();
    }
    server.close();

    serverPort = (serverPort + 1) % 65536; // Change the server port dynamically

    server.begin(serverPort);

}
