#include "WPA_secrets.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
String GAS_ID = SECRET_GSCRIPT_ID;   
const char* host = "script.google.com";
const int httpsPort = 443;
// Use WiFiClientSecure class to create TLS connection
WiFiClientSecure client;

unsigned long currentMillis;
unsigned long lastMillis = 0;
unsigned long measurementDelay = 10000;
int temp;
int humid;

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  currentMillis = millis();
  if((currentMillis - lastMillis) > measurementDelay){
    temp = random(32,100);
    humid = random(20,90);
    sendData(temp, humid);  
    lastMillis = currentMillis;
  }  
}

void sendData(int tem, int hum)
{
  Serial.print("connecting to ");
  Serial.println(host);
  client.setInsecure();
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String string_temperature =  String(tem, DEC); 
  String string_humidity =  String(hum, DEC); 
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
    Serial.println("headers received");
    break;
  }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
  } else {
  Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
} 
