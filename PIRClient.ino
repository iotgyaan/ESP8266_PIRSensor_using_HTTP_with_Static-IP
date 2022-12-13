#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "WAREHOUSE";
const char* password = "india@12345";

const char* serverNameledOn = "http://192.168.0.230/ledon";
const char* serverNameledOff = "http://192.168.0.230/ledoff";

int led = 12;                // the pin that the LED is atteched to
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)                

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 230);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

WiFiClient client; 

String httpGETRequestledOn(const char* serverNameledOn) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverNameledOn);
  int httpResponseCode = http.GET();  
  String payload = "-"; 
  
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
  }
  http.end();
  return payload;
}

String httpGETRequestledOff(const char* serverNameledOff) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverNameledOff);
  int httpResponseCode = http.GET();  
  String payload = "-"; 
  
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
  }
  http.end();
  return payload;
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
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
}

void loop() {    

  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    delay(10);                 // delay 100 milliseconds 
    
    if (state == LOW) {
      Serial.println("Motion detected!");     
      httpGETRequestledOn(serverNameledOn);
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
    digitalWrite(led, LOW); // turn LED OFF
    delay(10);              // delay 200 milliseconds 
      
    if (state == HIGH){
      Serial.println("Motion stopped!");
      httpGETRequestledOff(serverNameledOff);
      state = LOW;       // update variable state to LOW
    }
  }                
}
