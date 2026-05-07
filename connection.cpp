/* #include "connection.h"


Connection::Connection(int port)
  : WiFiServer server2(port) {
      inputString.reserve(50);                // Avoid memory fragmentation
      inputString = "";
      commandReady = false;
  }


void Connection::connect(const char* ssid, const char* password);{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void Connection::startserver(){
  serverwifi.begin();
  Serial.print("Server started!");
}


void Connection::handleClient(WiFiClient &client, const char* label){
    if (client.connected() && client.available()){
        while (client.available()) {
          char inChar = (char)client.read();
          if (inChar == '#') {
            commandReady = true;
            Serial.println(inputString);
            break;
          } else if (inChar != '\r' && inChar != '\n') {
            inputString += inChar;
          }
        }
    }
}

*/
