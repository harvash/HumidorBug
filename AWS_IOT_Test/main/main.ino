#pragma once
#include "Arduino.h"
#include <time.h>
#include <Udp.h>
#include "FS.h"             // File system commands to access files stored on flash memory
#include <ESP8266WiFi.h>    // WiFi Client to connect to the internet
#include <PubSubClient.h>   // MQTT Client to connect to AWS IoT Core
#include <NTPClient.h>      // Network Time Protocol Client, used to validate certificates
#include <WiFiUdp.h>        // UDP to communicate with the NTP server
#include "secrets.h"        // Connection data for AWS IOT and Home WiFi
#include "DHTesp.h"         // For the DHT11 temp/humidity sensor


// Basic DHT11 setup

DHTesp dht;
unsigned long epochTime;
float t = 0.0;
float h = 0.0;
String temperature = "T:";
String humidity = " H:";
bool hasData = false;
String entryTime = "";

// const long offSetTime = ;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "north-america.pool.ntp.org");



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient);

long lastMsg = 0;

void setup_wifi() {
  Serial.println();
  Serial.println("Connecting:");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }

  espClient.setX509Time(timeClient.getEpochTime());
}

void reconnect() {

  // Loop until the client connects
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (client.connect("WEMO_D1")) {
      Serial.println("connected");

      // Once connected, publish an announcement.
      client.publish("outTopic", "hello world");

      // Resubscribe to the inbound topic.
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      espClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  //timeClient.begin();
  //
  // SETUP FOR MQTT 
  //
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  setup_wifi();
  
  // TPH: I've played around with this and I don't think it's necessary.
  //delay(3000);
  
// Attempt to mount the file system
if (!SPIFFS.begin()) {
  Serial.println("Failed to mount file system");
  return;
}
else {
  Serial.println("Filesystem Mounted");
}

// FS upload issues
delay(3000);

 // Load certificate file from file system
  File cert = SPIFFS.open("/b57cc438d7-certificate.pem.crt", "r");
  if (!cert) {
    Serial.println("Failed to open certificate from file system");
  }
  else {
    Serial.println("Successfully opened certificate file");
  }

  delay(1000);

  // Load certificate to client connection
  if (espClient.loadCertificate(cert)) {
    Serial.println("Certificate loaded to client connection");
  }
  else {
    Serial.println("Certificate not loaded to client connection");
  }

  // Load private key file from file system
  File private_key = SPIFFS.open("/b57cc438d7-private.pem.key", "r");
  if (!private_key) {
    Serial.println("Failed to open private key from file system");
  }
  else {
    Serial.println("Successfully opened private key file");
  }

  delay(1000);

  // Load private key to client connection
  if (espClient.loadPrivateKey(private_key)) {
    Serial.println("Private key loaded to client connection");
  }
  else {
    Serial.println("Private key not loaded to client connection");
  }

  // Load CA file from file system
  File ca = SPIFFS.open("/Amazon_Root_CA_1.pem", "r");
  if (!ca) {
    Serial.println("Failed to open CA from file system");
  }
  else {
    Serial.println("Successfully opened CA file");
  }

  delay(1000);

  if (espClient.loadCACert(ca)) {
    Serial.println("CA loaded to client connection");
  }
  else {
    Serial.println("CA not loaded to client connection");
  } 

  //
  // SETUP FOR DHT
  // 
  dht.setup(14, DHTesp::DHT11); // Connect DHT sensor to GPIO 14  
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 30000) {
    lastMsg = now;
    
    // Setup timestamp
    //epochTime = timeClient.getEpochTime();
    //Serial.println("timestamp: " + epochTime);
//    struct tm *ptm = gmtime ((time_t *)&epochTime); 
//    
//    int curMonth = ptm->tm_mon+1;
//    char currentMonth[3];
//    sprintf(currentMonth, "%02d", curMonth);
//    // Serial.print("Month: ");
//    // Serial.println(currentMonth);
//
//    int curYear = ptm->tm_year+1900;
//    char currentYear[5];
//    sprintf(currentYear, "%d", curYear);
//    //Serial.print("Year: ");
//    //Serial.println(currentYear);
//
//    int curDay = ptm->tm_mday;
//    char monthDay[3];
//    sprintf(monthDay, "%02d", curDay);
//    //Serial.print("Month day: ");
//    //Serial.println(monthDay);
//
//    int HR = timeClient.getHours();
//   // String hour = String(HR);
//    char hour[3];
//    sprintf(hour, "%02d", HR);
//    
//    int MI = timeClient.getMinutes();
//    char minute[3];
//    sprintf(minute, "%02d", MI);
//    
//    int SS = timeClient.getSeconds();
//    char sec[3];
//    sprintf(sec, "%02d", SS);
//
//   entryTime = "";
//      entryTime+= currentYear;
//      entryTime += "-"; 
//      entryTime+= currentMonth; 
//      entryTime += "-"; 
//      entryTime+= monthDay; 
//      entryTime+= "UTC"; 
//      entryTime+= hour ;
//      entryTime+= minute;
//      entryTime+= sec;
//    Serial.print("Time: ");
//    Serial.println(entryTime);

    yield();  
    
    // Check for sensor data
    getData();
    if (hasData = true) {
      String json = buildJson();
      char jsonStr[500];
      json.toCharArray(jsonStr,500);
      boolean pubresult = client.publish("sensorData", jsonStr);
      Serial.print("Sending: ");
      Serial.println(jsonStr);
      if (pubresult)
        Serial.println("Done sending");
      else
        Serial.println("Error sending");  
    }
  }
}

String buildJson(){
  String data = "{";
  //data+="\n";
  //data+= "\"d\": {";
  data+="\n";
  data+="\"sensorName\": \"Humidor_Large\",";
  data+="\n";
  data+="\"timestamp\": \"";
  data+= timeClient.getEpochTime();
  data+= "\",";
  data+="\n";
  data+="\"temperature\": \"";
  data+= t;
  data+= "\",";
  data+="\n";
  data+="\"humidity\": \"";
  data+= h;
  data+= "\"";
  data+="\n";
  data+="}";
 // data+="\n";
 // data+="}";
  return data;
}

void getData() {
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    bool hasData = false;
  }
  else {
   bool hasData = true;
   float newT = dht.getTemperature();
    if (isnan(newT)){
      t = 0.0;
    }
    else {
      t = dht.toFahrenheit(newT);
    }

    float newH = dht.getHumidity();
    if (isnan(newH)){
      h = 0.0;    
    }
    else {
      h = newH;
    }
  }
}
