
/************************************************** Description *******************************************************/
/*
    File : RGB.ino
    Programmer : Mohammad Lotfi
    Used : ONN & OFF Over WIFI
    Design Pattern : Nothing
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2026/2/19
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
#include <ESP8266WebServerSecure.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <umm_malloc/umm_malloc.h>
#include <umm_malloc/umm_heap_select.h>
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
const char*  _NAME =             "RGB";
const char* _VERSION =           "v1.20260219";
/*--------------------------------------------------------------------------------------------------------------------*/
const int Pin_LED_CPU =          10; 
const int Pin_Red =              13; 
const int Pin_Green =            12; 
const int Pin_Blue =             14; 
const int PWM_Range =            256; 
const uint8_t HSVlights[61] = 
{0, 4, 8, 13, 17, 21, 25, 30, 34, 38, 42, 47, 51, 55, 59, 64, 68, 72, 76,
81, 85, 89, 93, 98, 102, 106, 110, 115, 119, 123, 127, 132, 136, 140, 144,
149, 153, 157, 161, 166, 170, 174, 178, 183, 187, 191, 195, 200, 204, 208,
212, 217, 221, 225, 229, 234, 238, 242, 246, 251, 255};
/*--------------------------------------------------------------------------------------------------------------------*/
int Value_RGB[3] =               {0, 0, 0};
bool BlinkMode =                 false;
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
	background: #5c8dc5;
  vertical-align: middle;
  margin: 10px;
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
    border-radius: 20px;
    color: yellow;
    display: inline-flex;
    align-items: center;
    margin: 15px;
    padding: 5px;
    width: 80px;
    height: 40px;
    appearance: none;
    cursor:pointer;
}
.l:before {
    content:"";
    background:#ddd;
    border-radius:50%;
    width:30px;
    height:30px;
    transition:0.3s;
}
.l:checked:before {
    background:currentColor;
    transform:translateX(35px);
}

.red{ color:red; }
.green{ color:lime; }
.blue{ color:cyan; }

label{
    display:block;
    margin-top:10px;
}
</style>
</head>
<body>
<table><tr>
<th>
<label>
<input id="Switch_RED" class="l red" type="checkbox"> RED
</label>
<label>
<input id="Switch_GREEN" class="l green" type="checkbox"> GREEN
</label>
<label>
<input id="Switch_BLUE" class="l blue" type="checkbox"> BLUE
</label>
</th>
</th>
<th width="500px" style="padding-left:20px">_TITLE</th>
</tr></table>
</body>
<script>
var myTimer;
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
  //alert(" " + xmlhttp.readyState + " " + mlhttp.statusx);
    if (xmlhttp.readyState == 4) {
      clearInterval(myTimer);
		  //alert(JSON.parse(xmlhttp.responseText).STATE);
      if(JSON.parse(xmlhttp.responseText).STATE == "ON") {
        Switch.checked = true;
      } else {
        Switch.checked = false;
      }
    }
}

var Switch_RED = document.getElementById("Switch_RED");
var Switch_GREEN = document.getElementById("Switch_GREEN");
var Switch_BLUE = document.getElementById("Switch_BLUE");

var NAME = document.getElementsByTagName("meta")[0];
var ID = document.getElementsByTagName("meta")[1];
var TITLE = document.getElementsByTagName("meta")[2];
var STATE = document.getElementsByTagName("meta")[3];

Switch_RED.checked = false;
Switch_GREEN.checked = false;
Switch_BLUE.checked = false;
if(STATE.content == "256-0-0") {
	Switch_RED.checked = true;
} 
else if(STATE.content == "0-256-0") {
  Switch_GREEN.checked = true;
}
else if(STATE.content == "0-0-256") {
  Switch_BLUE.checked = true;
}

Switch_RED.addEventListener('click', e => {
  if(e.target.checked){		
    xmlhttp.open("POST", window.location.origin + "/RED", true);
    Switch.checked = false;
  }
  else {
    xmlhttp.open("POST", window.location.origin + "/OFF", true);
    Switch.checked = true;
  }  	
  xmlhttp.send();
  myTimer = setTimeout("alert('Device Disconnected')", 3000);
});
Switch_GREEN.addEventListener('click', e => {
  if(e.target.checked){		
    xmlhttp.open("POST", window.location.origin + "/GREEN", true);
    Switch.checked = false;
  }
  else {
    xmlhttp.open("POST", window.location.origin + "/OFF", true);
    Switch.checked = true;
  }  	
  xmlhttp.send();
  myTimer = setTimeout("alert('Device Disconnected')", 3000);
});
Switch_BLUE.addEventListener('click', e => {
  if(e.target.checked){		
    xmlhttp.open("POST", window.location.origin + "/BLUE", true);
    Switch.checked = false;
  }
  else {
    xmlhttp.open("POST", window.location.origin + "/OFF", true);
    Switch.checked = true;
  }  	
  xmlhttp.send();
  myTimer = setTimeout("alert('Device Disconnected')", 3000);
});
</script>
</html>
)=====";
/************************************************** Opjects ***********************************************************/
ESP8266WebServer server(80); //Server on port 80
ESP8266WebServerSecure serverHTTPS(443);
/************************************************** Functions *********************************************************/
void Set_Value_LED_CPU(bool Value)
{
  digitalWrite(Pin_LED_CPU, Value?LOW:HIGH);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Set_Value_RGB(int Red, int Green, int Blue)
{
  Value_RGB[0] = Red;
  Value_RGB[1] = Green;
  Value_RGB[2] = Blue;
  analogWrite(Pin_Red, Red);
  analogWrite(Pin_Green, Green);
  analogWrite(Pin_Blue, Blue);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void trueHSV(int angle)
{
  byte red, green, blue;

  if (angle<60) {red = 255; green = HSVlights[angle]; blue = 0;} else
  if (angle<120) {red = HSVlights[120-angle]; green = 255; blue = 0;} else 
  if (angle<180) {red = 0, green = 255; blue = HSVlights[angle-120];} else 
  if (angle<240) {red = 0, green = HSVlights[240-angle]; blue = 255;} else 
  if (angle<300) {red = HSVlights[angle-240], green = 0; blue = 255;} else 
                 {red = 255, green = 0; blue = HSVlights[360-angle];} 
  Set_Value_RGB(red, green, blue);
}
/*--------------------------------------------------------------------------------------------------------------------*/
char* Get_Value_RGB()
{
  static char str[12];
  sprintf(str, "%d-%d-%d", Value_RGB[0], Value_RGB[1], Value_RGB[2]);
  return str;
}
/*--------------------------------------------------------------------------------------------------------------------*/
String Genrate_HTML()
{
  String html = _PAGE;
  html.replace("_NAME", _NAME);
  html.replace("_ID", Setting._ID);
  html.replace("_TITLE", Setting._TITLE);
  html.replace("_STATE", Get_Value_RGB());
  return html;
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
	  json += ",\"STATE\":\"" + String(Get_Value_RGB()) + "\"";  
	  json += ",\"ID\":\"" + String(Setting._ID) + "\"";
    json += ",\"TITLE\":\"" + String(Setting._TITLE) + "\"";    
    json += "}";
    
    server.send(200, "application/json", json);  
}
/*--------------------------------------------------------------------------------------------------------------------*/
// This routine is executed when you open its IP in browser
void handleRoot() {
 Serial.println("You called handleRoot");
 String s = Genrate_HTML();
 server.send(200, "text/html", s); //Send web page
}
/*--------------------------------------------------------------------------------------------------------------------*/
void handle_ON() { 
 Serial.println("You called handle_ON");
 BlinkMode = true;
 Get_Prop();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void handle_RED() { 
 Serial.println("You called handle_ON_RED");
 BlinkMode = false;
 Set_Value_RGB(PWM_Range, Value_RGB[1], Value_RGB[2]);
 Get_Prop();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void handle_GREEN() { 
 Serial.println("You called handle_ON_GREEN");
 BlinkMode = false;
 Set_Value_RGB(Value_RGB[0], PWM_Range, Value_RGB[2]);
 Get_Prop();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void handle_BLUE() { 
 Serial.println("You called handle_ON_BLUE");
 BlinkMode = false;
 Set_Value_RGB(Value_RGB[0], Value_RGB[1], PWM_Range);
 Get_Prop();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void handle_OFF() { 
 Serial.println("You called handle_OFF");
 BlinkMode = false;
 Set_Value_RGB(0, 0, 0);
 Get_Prop();
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
		  sprintf(Buffer, "%s,%s,%s,%s,%s,%s,%s,%s", _NAME, _VERSION, _IP.c_str(), Get_Value_RGB(), Setting._SSID, Setting._PASWORD, Setting._ID, Setting._TITLE);
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
void secureRedirect() {
  char Buffer[64];
  sprintf(Buffer, "http://%s/", _IP.c_str());		  
  serverHTTPS.sendHeader("Location", String(Buffer), true);
  serverHTTPS.send(301, "text/plain", "");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void setup() {

  delay(100);
  Serial.println("APP Start");

  Serial.print("Pin Config ");
  pinMode(Pin_LED_CPU, OUTPUT);
  analogWriteRange(PWM_Range);
  Set_Value_LED_CPU(false);
  Set_Value_RGB(0, 0, 0);
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
  char Hostname[64];
  sprintf(Hostname, "%s-%s", Setting._ID, Setting._TITLE);
  Serial.println("Ok");
		  
  Serial.print("WiFi Config ");
  WiFi.hostname(Hostname);
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
    serverHTTPS.on("/", secureRedirect);
    serverHTTPS.begin();
    //Which routine to handle at root location
    server.on("/", HTTP_POST, []() 
    {
      Ajax_Proccess();
    });
    server.on("/", handleRoot);  
    server.on("/ON", handle_ON);
    server.on("/OFF", handle_OFF);
    server.on("/RED", handle_RED);
    server.on("/GREEN", handle_GREEN);
    server.on("/BLUE", handle_BLUE);

    //Start server
    server.begin();                  
    Serial.println(" Ok");

    Serial.print("OTA Config ");
    // Port defaults to 8266
    ArduinoOTA.setPort(8266);
    // Hostname defaults to esp8266-[ChipID]    
    ArduinoOTA.setHostname(Hostname);
    // No authentication by default
    ArduinoOTA.setPassword((const char *)"123");
    ArduinoOTA.onStart([]() {
      Serial.println("OTA Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nOTA End");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("OTA Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("OTA Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("OTA Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("OTA Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("OTA End Failed");
    });
    ArduinoOTA.begin();
    Serial.println(" Ok");

  } else {
    Serial.println(" Fail");
  }
  
}
/************************************************** Tasks *************************************************************/
void loop() {
  if(BlinkMode) {
    static int Timer = 0;
    static int Cycle = 0;
    Timer++;
    if(Timer>=100) {
      trueHSV(Cycle);
      Timer = 0;
      Cycle++;
      if(Cycle>=360)
        Cycle = 0;
    }
  }

  serverHTTPS.handleClient();
  server.handleClient();
  Command_Proccess();
  ArduinoOTA.handle();
  
  delay(1);
}
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
