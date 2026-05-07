/* #ifndef barndoor_h
#define barndoor_h

#include <Arduino.h>
#include <WiFi.h>         // To create virtual serial

class Connection {
  private:
    String inputString;
    bool commandReady;
    WiFiClient clientwifi;
    WiFiServer serverwifi;
  public:
    Connection(int port);
    void connect(const char* ssid, const char* password);
    void startserver();
    void handleClient(WiFiClient &client, const char* label);
};

#endif

*/
