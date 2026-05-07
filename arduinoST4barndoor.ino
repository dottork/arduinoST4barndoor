// This file is part of Arduino ST4.
//
// Arduino ST4 is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Arduino ST4 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Arduino ST4.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright Kevin Ferrare 2014

// Include the AccelStepper library:
// This is written for ESP32





#include "barndoor.h"
#include "connection.h"

#include <WiFi.h>         // To create virtual serial


#define MAXSTEPS 5000     //
#define STEPTO30 31773    // Number of steps to go to around 30 degree open
#define STEPTO60 61380    // Number of steps to go to around 60 degree open
#define STEPTO90 86804    // Number of steps to go to around 90 degree open

 

const int stepsPerRevolution = 200*16;    // number of steps the NEMA motor perform to turn the shaft 360 degrees
const char* ssid = "your ssid";
const char* password = "your password";
WiFiServer server1(23);   // For /dev/ttyESP1
WiFiServer server2(24);   // For /dev/ttyESP2
WiFiClient client1;
WiFiClient client2;
String inputString1 = "";
bool commandReady1 = false;
String inputString2 = "";
bool commandReady2 = false;
bool client2status=false;

unsigned long previousMillis = 0;
unsigned long interval = 1000;
long sequenza=0;

Barndoor bd;              // Initiate the barndoor control 

void checkcon(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >=interval) {
    Serial.print(sequenza);
    sequenza=sequenza+1;
    Serial.print(" ");
    if (client1.connected()) {
      Serial.print("1co ");
    }
    else{
      Serial.print("1di ");
    }
      if (client2.connected()) {
      Serial.println("2co");
    }
    else{
      Serial.println("2di");
    }
    previousMillis=millis();
  }
}



void setup()
{
  delay(500);
  // to add control for connections leds
  Serial.begin(115200, SERIAL_8N1);      
  inputString1.reserve(50);                // Avoid memory fragmentation
  inputString2.reserve(50);
  inputString1="";
  inputString2="";
  delay(500);
  Serial.println(" ");
  Serial.println("INITIALIZED#");
  WiFi.setHostname("barndoor");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    if (WiFi.status() == WL_CONNECT_FAILED){
      Serial.println("Connection failed!");
    }
    else {
      Serial.print(".");
    }
    delay(500);
  }
  Serial.print("WiFi connected with address: ");
  Serial.println(WiFi.localIP());
  server1.begin();
  Serial.println("Server ESP1 created");
  server2.begin();
  Serial.println("Server ESP2 created");
  Serial.println("Servers created. Now goto zero");
  bd.goToZero();                                    //Reset barndoor position to zero or totally close
  bd.setSpeed(0);
  bd.stop();

  
  Serial.setTimeout(1);
  Serial.println("Ready!");
}



void loop()
{
  bd.update();                     //Update motor
  
  checkcon();
  
  // Accept new connections
  if (!client1 || !client1.connected()) {
    client1 = server1.available();
  }
  if (!client2 || !client2.connected()) {
    client2 = server2.available();
    if (!client2status && client2.connected()) {
      client2.print("INITIALIZED#");
      Serial.println("Inizialized!");
      client2status=true;
    }
  }

   if (client1.connected() && client1.available()){
          char inChar = (char)client1.read();
          if (inChar == '#') {
            commandReady1=true;
          }
          else{
            inputString1+=inChar; 
          }
   }

   if (client2.connected() && client2.available()){
          char inChar = (char)client2.read();
          if (inChar == '#') {
            commandReady2=true;
          }
          else{
            inputString2+=inChar; 
          }
   }


    if (commandReady2) {
      String opcode2=inputString2;
      inputString2 = "";
      commandReady2 = false;
      boolean validOpcode2=true;
      Serial.print("Comando 2: ");
      Serial.println(opcode2);
      //Parse opcode
      if(opcode2=="RA0"){             // Reset to sideral speed
        bd.setCorrectionFactor(1);
        Serial.println("RA0");
        }
      else if(opcode2=="RA+"){             // Change trackng speed +
        bd.setCorrectionFactor(1.5);
        Serial.println("RA+");
        }
      else if(opcode2=="RA-"){             // Change tracking speed -
        bd.setCorrectionFactor(0.5);
        Serial.println("RA-");
      }
      else if(opcode2=="DEC0"){
        //declination.reset();
      }
      else if(opcode2=="DEC+"){
        //declination.plus();
      }
      else if(opcode2=="DEC-"){
        //declination.minus();
      }
      else{
        validOpcode2=false;
        }
      if(validOpcode2){
        //Acknowledge valid command
        client2.print("OK#");
        Serial.println("2-OK#");
      }
    }

  
    if (commandReady1) {
    String opcode=inputString1;
    inputString1 = "";
    commandReady1 = false;
    boolean validOpcode=true;
    //Parse opcode
    Serial.print("Comando 1: ");
    Serial.println(opcode);
    if(opcode=="CONNECT"){
      //digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Connect");
    }
    else if (opcode=="DISCONNECT"){
      //digitalWrite(LED_BUILTIN, LOW);
      bd.goToZero();
      Serial.println("Disconnect");
    }
    else if(opcode=="RAS"){             // Start Tracking
      bd.startTrack();
      Serial.println("RAS");
    }

    else if(opcode=="RAE"){             // Stop trackinng
      bd.stopTracking();
      Serial.println("RAE");
    }
    else if(opcode=="GO0"){             // Close fast
      bd.fastGoto(0);
      Serial.println("GO0");
    }     
    else if(opcode=="GO30"){            // Go to 30 degree
      bd.fastGoto(STEPTO30);
      Serial.println("GO30");
    }
    else if(opcode=="GO60"){            // Go to 60 degree
      bd.fastGoto(STEPTO60);
      Serial.println("GO60");
    }
    else if(opcode=="GO90"){            // Go to 90 degree
      bd.fastGoto(STEPTO90);
      Serial.println("GO90");
    }
    else{
      validOpcode=false;
    }
    if(validOpcode){
      //Acknowledge valid command
      client1.print("OK#");
      Serial.println("1-OK#");
    }
  } 
}
