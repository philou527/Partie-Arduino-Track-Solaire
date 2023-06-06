#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <ESP8266WebServer.h>
// 192.168.220.24

const char* ssid = "flash";
const char* password = "philouphilou";
WiFiServer server(5000);
WiFiClient client;
//ESP8266WebServer server2; // web server
boolean envoye = false;
int n = 0;
int instruction;
String message;



void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED) {
      delay(500); Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  Wire.begin();

  /*server2.on("/",[](){
    server2.send_P(200,"text/html",webpage);
  });

  server2.on("/auto",[](){
    server2.send_P(200,"text/html",webpage2);
    });
  
  */
  server.begin(); // server de lou
  //server2.begin(); // server pour une webpage
 
}

void loop() {

  //server2.handleClient();
  
  if(!client.connected()) {
    client = server.available();
    if(client.connected()) {
      Serial.println("Le client est connected");
    }
    
  }
  if(client.available()>0) {
    char instruction = client.read();
    message+= instruction;
    //client.println(1);
    Wire.beginTransmission(8);
    if(client.available() == 0) {
      Serial.println(message);
      Wire.write(message.toInt());

      // On regarde si le message est égale à 122:'z'
      if(message!=""){
        Serial.println("on rentre dans le if");
        Wire.requestFrom(8,9);
       String msg = "";
        while(Wire.available()){
          //Serial.println("aaa");
          byte c = Wire.read();
          client.println(c);
          Serial.println(c);
          
        }
        
        msg="";
        }
      
      message = "";
    }
    Wire.endTransmission();
    Serial.println(message);
    //client.println("received");
  }
  
  
}
