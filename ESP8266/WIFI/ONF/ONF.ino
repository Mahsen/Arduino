
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
    Last Update : 2023/6/28
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
#include <EEPROM.h>
/************************************************** Defineds **********************************************************/
#define EEPROM_SIZE 256
/************************************************** Names *************************************************************/
/*
    Nothing
*/
/************************************************** Variables *********************************************************/
struct struct_Setting {
	int Validation;
	char _ID[16];
	char _TITLE[32];
	char _SSID[64];
	char _PASWORD[64];
	char _RESERVED[76];
} Setting;
/*--------------------------------------------------------------------------------------------------------------------*/
const char*  _NAME =             "ONF";
const char* _VERSION =           "v1.20230628";
/*--------------------------------------------------------------------------------------------------------------------*/
const int Pin_LAMP =             9; 
const int Pin_LED_CPU =          10; 
/*--------------------------------------------------------------------------------------------------------------------*/
bool Value_LAMP =                false;
String _IP = "0.0.0.0";
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
<style>
*, *:before, *:after {
	box-sizing: border-box;
	margin: 0;
	padding: 0;
}
:root {
	/* minFontSize + (maxFontSize - minFontSize) * (100vw - minVWidth)/(maxVWidth - minVWidth) */
	font-size: calc(64px + (80 - 64) * (100vw - 320px)/(960 - 320));
}
body, input {
	font-size: 1em;
	line-height: 1.5;
}
body {
	background: #bbb;
  vertical-align: middle;
  padding: 20px;
}
input {
	display: block;
	margin-bottom: 1.5em;
}
main {
	padding: 1.5em 0 0 0;
	text-align: center;	
}
.l {
	background-color: rgba(0,0,0,0.7);
	border-radius: 0.75em;
	box-shadow: 0.125em 0.125em 0 0.125em rgba(0,0,0,0.3) inset;
	color: #fdea7b;
	display: inline-flex;
	align-items: center;
	margin: auto;
	padding: 0.15em;
	width: 3em;
	height: 1.5em;
	transition: background-color 0.1s 0.3s ease-out, box-shadow 0.1s 0.3s ease-out;
	-webkit-appearance: none;
	-moz-appearance: none;
	appearance: none;
}
.l:before, .l:after {
	content: "";
	display: block;
}
.l:before {
	background-color: #d7d7d7;
	border-radius: 50%;
	width: 1.2em;
	height: 1.2em;
	transition: background-color 0.1s 0.3s ease-out, transform 0.3s ease-out;
	z-index: 1;
}
.l:after {
	background:
		linear-gradient(transparent 50%, rgba(0,0,0,0.15) 0) 0 50% / 50% 100%,
		repeating-linear-gradient(90deg,#bbb 0,#bbb,#bbb 20%,#999 20%,#999 40%) 0 50% / 50% 100%,
		radial-gradient(circle at 50% 50%,#888 25%, transparent 26%);
	background-repeat: no-repeat;
	border: 0.25em solid transparent;
	border-left: 0.4em solid #d8d8d8;
	border-right: 0 solid transparent;
	transition: border-left-color 0.1s 0.3s ease-out, transform 0.3s ease-out;
	transform: translateX(-22.5%);
	transform-origin: 25% 50%;
	width: 1.2em;
	height: 1em;
}
/* Checked */
.l:checked {
	background-color: rgba(0,0,0,0.45);
	box-shadow: 0.125em 0.125em 0 0.125em rgba(0,0,0,0.1) inset;
}
.l:checked:before {
	background-color: currentColor;
	transform: translateX(125%)
}
.l:checked:after {
	border-left-color: currentColor;
	transform: translateX(-2.5%) rotateY(180deg);
}
/* Other States */
.l:focus {
	/* Usually an anti-A11Y practice but set to remove an annoyance just for this demo */
	outline: 0;
}
</style>
</head>
<body>
<input id='Switch' class="l" type="checkbox"> _TITLE
</body>
<script>
var xmlhttp;
if (window.XMLHttpRequest) {
    // code for IE7+, Firefox, Chrome, Opera, Safari
    xmlhttp = new XMLHttpRequest();
}
else {
    // code for IE6, IE5
    xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
}

xmlhttp.onreadystatechange = function () {
    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
		  //alert(JSON.parse(xmlhttp.responseText).STATE);
      if(JSON.parse(xmlhttp.responseText).STATE == "ON") {
        Switch.checked = true;
      } else {
        Switch.checked = false;
      }
    }
}

var Switch = document.getElementById("Switch");

var NAME = document.getElementsByTagName("meta")[0];
var ID = document.getElementsByTagName("meta")[1];
var TITLE = document.getElementsByTagName("meta")[2];
var STATE = document.getElementsByTagName("meta")[3];

if(STATE.content == "ON") {
	Switch.checked = true;
} else {
	Switch.checked = false;
}

Switch.addEventListener('click', e => {
  if(e.target.checked){		
    xmlhttp.open("POST", window.location.origin + "/ON", true);
    Switch.checked = false;
  }
  else {
    xmlhttp.open("POST", window.location.origin + "/OFF", true);
    Switch.checked = true;
  }  	
  xmlhttp.send();
});
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
  html.replace("_ID", Setting._ID);
  html.replace("_TITLE", Setting._TITLE);
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
 Get_Prop();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void handle_OFF_LAMP() { 
 Serial.println("You called handle_OFF_LAMP");
 Set_Value_LAMP(false);
 Get_Prop();
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
	  json += ",\"Version\":\"" + String(_VERSION) + "\"";  
    json += ",\"IP\":\"" + _IP + "\"";
	  json += ",\"STATE\":\"" + String((Get_Value_LAMP()?"ON":"OFF")) + "\"";  
	  json += ",\"ID\":\"" + String(Setting._ID) + "\"";
    json += ",\"TITLE\":\"" + String(Setting._TITLE) + "\"";    
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
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Command_Proccess()
{
  if(Serial.available()) {
	  char Buffer[256], *pch, idx=0;
	  String Data = Serial.readString();  //read until timeout	  
	  if(strstr(Data.c_str(), "Get()")) {
		  sprintf(Buffer, "%s,%s,%s,%s,%s,%s,%s,%s", _NAME, _VERSION, _IP, (Get_Value_LAMP()?"ON":"OFF"), Setting._SSID, Setting._PASWORD, Setting._ID, Setting._TITLE);
		  Serial.println(Buffer);
	  }
	  else if(strstr(Data.c_str(), "Set(")) {
		  strcpy(Buffer, Data.c_str());
		  pch = strtok (Buffer, "(,)");
		  while (pch)
		  {
        switch(idx) {				  
          case 1: {
            strcpy(Setting._SSID, pch);
            break;
          }
          case 2: {
            strcpy(Setting._PASWORD, pch);
            break;
          }
          case 3: {
            strcpy(Setting._ID, pch);
            break;
          }
          case 4: {
            strcpy(Setting._TITLE, pch);
            break;
          }
        }
        idx++;
        pch = strtok (NULL, "(,)");
		  }
		  Serial.print("EEPROM Config ");
		  EEPROM.begin(EEPROM_SIZE);
		  EEPROM.put(0, Setting);
		  EEPROM.commit();
		  EEPROM.end();
		  Serial.print("Save ");
		  Serial.println("Ok");		  
	  } else if(strstr(Data.c_str(), "Reset()")) {
      Serial.println("Ok");		
      ESP.reset();      
    }	  
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
  
  Serial.print("EEPROM Config ");
  //Init EEPROM
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, Setting);
  if(Setting.Validation != sizeof(Setting)) {
	  strcpy(Setting._ID, "00000000");
	  strcpy(Setting._TITLE, "none");
	  strcpy(Setting._SSID, "SSID");
	  strcpy(Setting._PASWORD, "PASSWORD");
	  Setting.Validation = sizeof(Setting);
	  EEPROM.put(0, Setting);
	  EEPROM.commit();
	  Serial.print("Reset ");
  } else {
	  Serial.print("Load ");
  }
  EEPROM.end();
  Serial.println("Ok");
		  
  Serial.print("WiFi Config ");
  WiFi.hostname(Setting._ID);
  WiFi.begin(Setting._SSID, Setting._PASWORD);     //Connect to your WiFi router
  // Wait for connection
  int Timeout=0;
  for (Timeout=0; ((Timeout < 60) && (WiFi.status() != WL_CONNECTED)); Timeout++) {
    delay(500);
    Set_Value_LED_CPU(true);
    delay(500);
    Set_Value_LED_CPU(false);
    Serial.print(".");
    Command_Proccess();
  }
  Set_Value_LED_CPU(false);
  if(Timeout != 60) {
    Serial.println(" Ok");

    //If connection successful show IP address in serial monitor
    Serial.print("Connected to ");
    Serial.println(Setting._SSID);
    Serial.print("IP address: ");
    //IP address assigned to your ESP
    _IP = WiFi.localIP().toString();
    Serial.println(_IP);  
    
    Serial.print("Server Config ");
    //Which routine to handle at root location
    server.on("/", HTTP_POST, []() 
    {
      Ajax_Proccess();
    });
    server.on("/", handleRoot);  
    server.on("/ON", handle_ON_LAMP);
    server.on("/OFF", handle_OFF_LAMP);

    //Start server
    server.begin();                  
    Serial.println(" Ok");
  } else {
    Serial.println(" Fail");
  }
}
/************************************************** Tasks *************************************************************/
void loop() {
  server.handleClient();          //Handle client requests
  Command_Proccess();
}
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
