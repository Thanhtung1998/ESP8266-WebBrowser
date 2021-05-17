//----------------------------------------Include the NodeMCU ESP8266 Library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//----------------------------------------

#include "DHT.h"  //--> Include the DHT Sensor Library 
#define DHTTYPE DHT11 //--> Defines the type of DHT sensor used (DHT11, DHT21, and DHT22), in this project the sensor used is DHT11.

#include "html.h" //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

#define LEDonBoard 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router
String statusD1="1",statusD2="1",statusD3="1",statusD4="1";
const int output = D7;
const int buttonPin = D6;
int ledState = LOW;        
int buttonState;      
int lastButtonState = LOW;   
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;  
const char* PARAM_INPUT_1 = "state";
//----------------------------------------SSID and Password of your WiFi router
const char* ssid = "DANGTHANH";
const char* password = "0915359928";
//----------------------------------------

ESP8266WebServer server(80);  //--> Server on port 80
int ButtonValue = 0;
const int DHTPin = D5; //--> The pin used for the DHT11 sensor is Pin D1=Pin 5
DHT dht(DHTPin, DHTTYPE); //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);

//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot () {
  server.send (200, "text / html", MAIN_page); // -> Gửi trang web
}
//----------------------------------------
//----------------------------------------Procedure for reading the temperature value of a DHT11 sensor
void handleDHT11Temperature() {
  float t = dht.readTemperature();  //--> Read temperature as Celsius (the default). If you want to read the temperature sensor in Fahrenheit, use this -> float t = dht.readTemperature (true);
  String Temperature_Value = String(t);
 
  server.send(200, "text/plane", Temperature_Value); //--> Send Temperature value only to client ajax request
  
  if (isnan(t)) { //--> Check if any reads failed and exit early (to try again).  
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    Serial.print("DHT11 || Temperature : ");
    Serial.print(t);
    Serial.print(" || ");
  }
}
//----------------------------------------
//----------------------------------------Procedure for reading humidity values from DHT11 sensors
void handleDHT11Humidity() {
  float h = dht.readHumidity();
  String Humidity_Value = String(h);
 
  server.send(200, "text/plane", Humidity_Value); //--> Send Humidity value only to client ajax request
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor !");
  }
  else {
    Serial.print("Humidity : ");
    Serial.println(h);
  }
}
//----------------------------------------
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    String outputStateValue = outputState();
    buttons+= "<h4>GPIO 2 - State <span id=\"outputState\"><span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}
String outputState(){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}
//----------------------------------------Setup
void setup(void){
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,HIGH);
  Serial.begin(115200);
  delay(500);
  dht.begin();  //--> Start reading DHT11 sensors
  delay(500);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("THANHTUNG","123456789");
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(LEDonBoard,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(LEDonBoard, HIGH); //--> Turn off Led On Board
  
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(LEDonBoard, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //----------------------------------------
  server.on("/",mainpage);
 server.on("/getSTATUSD",get_STATUSD);
  server.on("/D1on",D1_on);
  server.on("/D1off",D1_off);
  server.on("/D2on",D2_on);
  server.on("/D2off",D2_off);
  server.on("/D3on",D3_on);
  server.on("/D3off",D3_off);
  server.on("/D4on",D4_on);
  server.on("/D4off",D4_off);
  server.on("/Allon",All_on);
  server.on("/Alloff",All_off);
  server.on("/", handleRoot); //--> Routine to handle at root location. This is to display web page.
  server.on("/readTemperature", handleDHT11Temperature);  //--> Routine to handle the call procedure handleDHT11Temperature
  server.on("/readHumidity", handleDHT11Humidity);  //--> Routine to handle the call procedure handleDHT11Humidity

  server.begin(); //--> Start server
  Serial.println("HTTP server started");
}
//----------------------------------------
//----------------------------------------Loop

void loop() {
  server.handleClient();
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
 digitalWrite(output, ledState);
  lastButtonState = reading;
}

//==========Chương trình con=================//
void mainpage(){
  String s = FPSTR(MAIN_page);
  server.send(200,"text/html",s);
}
void get_STATUSD(){
  if(digitalRead(D1)==0){
    statusD1 = "1";
  }else{
    statusD1 = "0";
  }
  if(digitalRead(D2)==0){
    statusD2 = "1";
  }else{
    statusD2 = "0";
  }
  if(digitalRead(D3)==0){
    statusD3 = "1";
  }else{
    statusD3 = "0";
  }
  if(digitalRead(D4)==0){
    statusD4 = "1";
  }else{
    statusD4 = "0";
  }
  String s = "{\"D1\": \""+ statusD1 +"\"," +
              "\"D2\": \""+ statusD2 + "\"," +
              "\"D3\": \""+ statusD3 + "\"," +
              "\"D4\": \""+ statusD4 +"\"}";
  server.send(200,"application/json",s);
}
//--------------Điều khiển chân D1----------------
void D1_off(){
  digitalWrite(D1,LOW);
  get_STATUSD();
}
void D1_on(){
  digitalWrite(D1,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D2----------------
void D2_off(){
  digitalWrite(D2,LOW);
  get_STATUSD();
}
void D2_on(){
  digitalWrite(D2,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D3----------------
void D3_off(){
  digitalWrite(D3,LOW);
  get_STATUSD();
}
void D3_on(){
  digitalWrite(D3,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D4----------------
void D4_off(){
  digitalWrite(D4,LOW);
  get_STATUSD();
}
void D4_on(){
  digitalWrite(D4,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D----------------
void All_off(){
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  get_STATUSD();
}
void All_on(){
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,HIGH);
  get_STATUSD();
}
//----------------------------------------
