#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

const char* ssid = "WAREHOUSE";
const char* password = "india@12345";

const int ledPin = 2;

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 230);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

AsyncWebServer server(80);

void setup(){
  Serial.begin(115200);        // initialize serial
  pinMode(ledPin, OUTPUT);
  Serial.println();
  
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/ledon", HTTP_GET, [](AsyncWebServerRequest *request){
   request->send_P(100, "text/plain", "ON responce!");
   Serial.println("Send ON!"); 
   digitalWrite(ledPin, HIGH);       
  });

  server.on("/ledoff", HTTP_GET, [](AsyncWebServerRequest *request){
   request->send_P(100, "text/plain", "OFF responce!");
   Serial.println("Send OFF!"); 
   digitalWrite(ledPin, LOW);
  });
  
  server.begin();  
}

void loop(){   
  
}
     
