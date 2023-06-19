
/************************************************** Description *******************************************************/
/*
    File : ReadPIR.ino
    Programmer : Mohammad Lotfi
    Used : Sample check PIR sensor with adc
    Design Pattern : Nothing
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2023/6/19
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
const int Pin_Analog =           A0;
/*--------------------------------------------------------------------------------------------------------------------*/
bool Value_LAMP =               false;
int Value_Analog =              0;
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
  //pinMode(Pin_LED_CPU, OUTPUT);
  //pinMode(Pin_LAMP, OUTPUT);
  //}
  
  //Set_Value_LED_CPU(false);
  //Set_Value_LAMP(false);

  Serial.begin(115200);
  Serial.println();

  Serial.println("Start");


}
/************************************************** Tasks *************************************************************/
void loop()
{
  delay(500);
  //Set_Value_LED_CPU(true);
  delay(500);
  //Set_Value_LED_CPU(false);

  // read the analog in value
  Value_Analog = analogRead(Pin_Analog);
 
  // print the readings in the Serial Monitor
  Serial.print("Value_Analog = ");
  Serial.println(Value_Analog);

}
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
