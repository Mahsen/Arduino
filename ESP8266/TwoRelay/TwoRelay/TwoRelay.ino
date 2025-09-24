
/************************************************** Description *******************************************************/
/*
    File : TwoRelay.ino
    Programmer : Mohammad Lotfi
    Used : Sample two lump switch on with toggel power 
    Design Pattern : Nothing
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2025/9/24
*/
/************************************************** Warnings **********************************************************/
/*
    Turning on a pin after open door
*/
/************************************************** Wizards ***********************************************************/
/*
    Nothing
*/
/************************************************** Includes **********************************************************/
#include <ESP8266WiFi.h>
#include <EEPROM.h>
/************************************************** Defineds **********************************************************/
#define EEPROM_SIZE 256
/************************************************** Names *************************************************************/
/*
    Nothing
*/
/************************************************** Variables *********************************************************/
const int Pin_LAMP_A =             5; 
const int Pin_LAMP_B =             4; 
const int Pin_LED_CPU =            2; 
/************************************************** Opjects ***********************************************************/
struct struct_Setting {
	int Validation;
	int _State;
} Setting;
/************************************************** Functions *********************************************************/
void Set_Value_LED_CPU(bool Value)
{
  digitalWrite(Pin_LED_CPU, Value?LOW:HIGH);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Run_LAMP(void) {
  switch(Setting._State) {
    case 0:{
      digitalWrite(Pin_LAMP_A, HIGH);
      digitalWrite(Pin_LAMP_B, HIGH);
      Setting._State = 1;
      break;      
    }
    case 1:{
      digitalWrite(Pin_LAMP_A, HIGH);
      digitalWrite(Pin_LAMP_B, LOW);
      Setting._State = 2;
      break;      
    }
    case 2:{
      digitalWrite(Pin_LAMP_A, LOW);
      digitalWrite(Pin_LAMP_B, HIGH);
      Setting._State = 0;
      break;      
    }
  }
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(0, Setting);
  EEPROM.commit();
  Serial.print("Change ");
  EEPROM.end();
  Serial.println("Ok");
  
}
/*--------------------------------------------------------------------------------------------------------------------*/
void setup() {

  Serial.println("APP Start");

  Serial.print("Serial Config ");
  Serial.begin(115200);
  Serial.println("Ok");

  Serial.print("Pin Config ");
  pinMode(Pin_LED_CPU, OUTPUT);
  pinMode(Pin_LAMP_A, OUTPUT);
  pinMode(Pin_LAMP_B, OUTPUT);
  Serial.println("Ok");

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, Setting);
  if(Setting.Validation != sizeof(Setting)) {
	  Setting._State = 0;
	  Setting.Validation = sizeof(Setting);
	  EEPROM.put(0, Setting);
	  EEPROM.commit();
	  Serial.print("Reset ");
  } else {
	  Serial.print("Load ");
  }
  EEPROM.end();
  Serial.println("Ok");

  Run_LAMP();

  Serial.println("Run");

}
/************************************************** Tasks *************************************************************/
void loop() {
  static bool LED = false;
  delay(1000);
  digitalWrite(Pin_LED_CPU, LED);
  LED ^= true;
}
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
