
/************************************************** Description *******************************************************/
/*
    File : sim800.ino
    Programmer : Mohammad Lotfi
    Used : base config modem
    Design Pattern : Nothing
    Types of memory : Nothing
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2023/5/23
*/
/************************************************** Warnings **********************************************************/
/*
    Nothing
*/
/************************************************** Wizards ***********************************************************/
/*
    Nothing
*/
/************************************************** Includes **********************************************************/
#include <SoftwareSerial.h>
#include <Adafruit_FONA.h>
#include <ESP8266WiFi.h>
/************************************************** Defineds **********************************************************/
#define SIM800_TX  12
#define SIM800_RX  14
#define SIM800_RST 13 // SIM800 reset pin
/************************************************** Names *************************************************************/
/*
    Nothing
*/
/************************************************** Variables *********************************************************/
SoftwareSerial Sim800l(SIM800_RX, SIM800_TX);
Adafruit_FONA fona = Adafruit_FONA(SIM800_RST);
const char* ssid = "SHARE";
const char* password = "12345678";
/************************************************** Opjects ***********************************************************/
/*
    Nothing
*/
/************************************************** Functions *********************************************************/
void setup() {

  Serial.begin(115200);
  Sim800l.begin(4800);

  // Initialize SIM800 module
  /*if (!fona.begin(Sim800l)) {
    Serial.println("Couldn't find SIM800 module");
    while (1);
  }*/

  Serial.println("Start.");

  // Configure SIM800 to forward internet traffic to ESP8266
  /*fona.enableGPRS(true);

  fona.sendSMS("09124662703", "Hi");*/
}
/************************************************** Tasks *************************************************************/
void loop() {

  while(Sim800l.available()){
  Serial.write(Sim800l.read());
  }

  
 while(Serial.available()){
  Sim800l.write(Serial.read());
  }

}
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
