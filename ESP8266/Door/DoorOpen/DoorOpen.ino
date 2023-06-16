
/************************************************** Description *******************************************************/
/*
    File : DoorOpen.ino
    Programmer : Mohammad Lotfi
    Used : Sample lump on if door open (magnet read switch)
    Design Pattern : Nothing
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2023/6/16
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
/*
    Nothing
*/
/************************************************** Defineds **********************************************************/
/*
    Nothing
*/
/************************************************** Names *************************************************************/
/*
    Nothing
*/
/************************************************** Variables *********************************************************/
const int Pin_LAMP =             9; 
const int Pin_LED_CPU =          10; 
const int Pin_MAGNET =           13; 
/*--------------------------------------------------------------------------------------------------------------------*/
bool Value_LAMP =               false;
/************************************************** Opjects ***********************************************************/
/*
    Nothing
*/
/************************************************** Functions *********************************************************/
void Set_Value_LED_CPU(bool Value)
{
  digitalWrite(Pin_LED_CPU, Value?LOW:HIGH);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Set_Value_LAMP(bool Value)
{
  Value_LAMP = Value;
  digitalWrite(Pin_LAMP, Value_LAMP?LOW:HIGH);
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool Get_Value_LAMP()
{
  return Value_LAMP;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void setup()
{

  ///
  //{
  pinMode(Pin_LED_CPU, OUTPUT);
  pinMode(Pin_LAMP, OUTPUT);
  pinMode(Pin_MAGNET, INPUT_PULLUP);
  //}
  
  Set_Value_LED_CPU(false);
  Set_Value_LAMP(false);

  Serial.begin(115200);
  Serial.println();

  Serial.println("Start");


}
/************************************************** Tasks *************************************************************/
void loop()
{
  delay(500);
  Set_Value_LED_CPU(true);
  bool Value = digitalRead(Pin_MAGNET);
  delay(500);
  Set_Value_LED_CPU(false);
  bool Value_Feed = digitalRead(Pin_MAGNET);
  
  if(Value == Value_Feed) {
    if(Value) {
      if(Get_Value_LAMP() == false) {
        delay(1000);
        Set_Value_LAMP(true);
        Serial.println("Lamp ON");
      }
    }
    else if(Get_Value_LAMP() == true) {
      delay(1000);
      Set_Value_LAMP(false);
      Serial.println("Lamp OFF");
    }
  }

}
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
