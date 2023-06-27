
/************************************************** Description *******************************************************/
/*
    File : ONF.ino
    Programmer : Mohammad Lotfi
    Used : ONN & OFF Over WIFI
    Design Pattern : Nothing
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2023/6/26
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
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
/************************************************** Defineds **********************************************************/
/*
    Nothing
*/
/************************************************** Names *************************************************************/
/*
    Nothing
*/
/************************************************** Variables *********************************************************/
const char*  _NAME = "ONF";
const char* _VERSION = "v1";
const char* _ID = "00000002"; // "00000001";
const char* _TITLE = "Kitchen"; // "Bedroom";
/*--------------------------------------------------------------------------------------------------------------------*/
const int Pin_LAMP =             9; 
const int Pin_LED_CPU =          10; 
/*--------------------------------------------------------------------------------------------------------------------*/
bool Value_LAMP =               false;
String _IP;
/*--------------------------------------------------------------------------------------------------------------------*/
//SSID and Password of your WiFi router
const char* _SSID = "Mahsen_1010"; //"Your_SSID";
const char* _PASWORD = "03100#Mahsen3tik"; //"Your_PASWORD";
/*--------------------------------------------------------------------------------------------------------------------*/
//Our HTML webpage contents in program memory
const char _PAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>_TITLE</title>
<meta name="NAME" content="_NAME">
<meta name="ID" content="_ID">
<meta name="TITLE" content="_TITLE">
<meta name="STATE" content="_STATE">
</head>
<body>
<center>
<table border='1' style='font-size: 40px'>
<tr>
<th id='Table_th_Title' style='border: 3px solid' colspan="2">
_TITLE
</th>
</tr>
<tr>
<th id='Table_th_ON' style='width: 200px; height: 40px'><a href="ON">ON</a></th>
<th id='Table_th_OFF' style='width: 200px; height: 40px'><a href="OFF">OFF</a></th>
</tr>
</table>
</center>
</body>
<script>
var Table_th_Title = document.getElementById("Table_th_Title");
var Table_th_ON = document.getElementById("Table_th_ON");
var Table_th_OFF = document.getElementById("Table_th_OFF");
var NAME = document.getElementsByTagName("meta")[0];
var ID = document.getElementsByTagName("meta")[1];
var TITLE = document.getElementsByTagName("meta")[2];
var STATE = document.getElementsByTagName("meta")[3];

if(STATE.content == "ON") {
	Table_th_ON.style.backgroundColor = "#00FF00";
	Table_th_OFF.style.backgroundColor = "#FFFFFF";
	Table_th_Title.style.borderColor = "#00FF00";
  Table_th_Title.style.backgroundColor = "#00FF00";
} else {
	Table_th_ON.style.backgroundColor = "#FFFFFF"; 
	Table_th_OFF.style.backgroundColor ="#FF0000";
	Table_th_Title.style.borderColor = "#FF0000";
  Table_th_Title.style.backgroundColor = "#FF0000";
}
</script>
</html>
)=====";
/************************************************** Opjects ***********************************************************/
ESP8266WebServer server(80); //Server on port 80
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
  Set_Value_LED_CPU(!Value);
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool Get_Value_LAMP()
{
  return Value_LAMP;
}
/*--------------------------------------------------------------------------------------------------------------------*/
String Genrate_HTML()
{
  String html = _PAGE;
  html.replace("_NAME", _NAME);
  html.replace("_ID", _ID);
  html.replace("_TITLE", _TITLE);
  html.replace("_STATE", (Get_Value_LAMP()?"ON":"OFF"));
  return html;
}
/*--------------------------------------------------------------------------------------------------------------------*/
// This routine is executed when you open its IP in browser
void handleRoot() {
 Serial.println("You called handleRoot");
 String s = Genrate_HTML();
 server.send(200, "text/html", s); //Send web page
}
/*--------------------------------------------------------------------------------------------------------------------*/
void handle_ON_LAMP() { 
 Serial.println("You called handle_ON_LAMP");
 Set_Value_LAMP(true);
 String s = Genrate_HTML();
 server.send(200, "text/html", s);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void handle_OFF_LAMP() { 
 Serial.println("You called handle_OFF_LAMP");
 Set_Value_LAMP(false);
 String s = Genrate_HTML();
 server.send(200, "text/html", s); 
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Header()
{
  server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  server.sendHeader(F("Access-Control-Max-Age"), F("600"));
  server.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
  server.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Get_Prop()
{
    Serial.println("You called Get_Prop");

    Header();

    String json = "{";
    json += "\"Function\":\"Get_Prop\"";
    json += ",\"Name\":\"" + String(_NAME) + "\"";
    json += ",\"ID\":\"" + String(_ID) + "\"";
    json += ",\"IP\":\"" + _IP + "\"";
    json += ",\"TITLE\":\"" + String(_TITLE) + "\"";
    json += ",\"STATE\":\"" + String((Get_Value_LAMP()?"ON":"OFF")) + "\"";
    json += ",\"Version\":\"" + String(_VERSION) + "\"";    
    json += "}";
    
    server.send(200, "application/json", json);  
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Ajax_Proccess()
{
  if(server.argName(0).indexOf("Function") != -1) 
  {
    if(server.arg(0).indexOf("Get_Prop") != -1) 
    {
      Get_Prop();
    }
  }
  else {
    handleRoot();
  }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void setup() {

  delay(100);
  Serial.println("APP Start");

  Serial.print("Pin Config ");
  pinMode(Pin_LED_CPU, OUTPUT);
  pinMode(Pin_LAMP, OUTPUT);
  Set_Value_LED_CPU(false);
  Set_Value_LAMP(true);
  Serial.println("Ok");
  
  Serial.print("Serial Config ");
  Serial.begin(115200);
  delay(100);
  Serial.println("Ok");
  
  Serial.print("WiFi Config ");
  WiFi.hostname(_ID);
  WiFi.begin(_SSID, _PASWORD);     //Connect to your WiFi router
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Ok");

  //If connection successful show IP address in serial monitor
  Serial.print("Connected to ");
  Serial.println(_SSID);
  Serial.print("IP address: ");
  //IP address assigned to your ESP
  _IP = WiFi.localIP().toString();
  Serial.println(_IP);  
  
  Serial.print("Server Config ");
  //Which routine to handle at root location
  server.on("/", HTTP_GET, []() 
  {
    Ajax_Proccess();
  });
  //as Per  <a href="ON">,
  server.on("/ON", handle_ON_LAMP);  
  //as Per  <a href="OFF">,
  server.on("/OFF", handle_OFF_LAMP);
  //as Per 
  

  //Start server
  server.begin();                  
  Serial.println(" Ok");

}
/************************************************** Tasks *************************************************************/
void loop() {
  server.handleClient();          //Handle client requests
}
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
