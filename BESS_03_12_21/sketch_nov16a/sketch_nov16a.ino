#include <ArduinoJson.h>           // install library arduinojson version 6+ 
#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h> 
//#include <WifiClient.h>
//#include <HTTPClient.h>
//#include <WiFi.h>
//#define CS 15 

//ESP8266WiFi server(80);
const char* ssid = "Vinod";   // router wifi name to connect to
const char* password = "archana6887"; // router wifi password
const char* host = "192.168.100.3";
//const char host[] = "192.168.100.3";
  WiFiClient client;
String webSite, javaScript, XML;
String docstring;
float voltage1 = 0; // used to store voltage value
float voltage2 = 0;
float voltage3 = 0;
float voltage4 = 0;
float Radjust = 0.066478076; // Voltage divider factor ( R2 / R1+R2 )
float vbat1 = 0; //final voltage after calcs- voltage of the battery
float vbat2 = 0;
float vbat3 = 0;
float vbat4 = 0;
float BATT1 = 0;
float BATT2 = 0;
float BATT3 = 0;
float BATT4 = 0;

void setup() {
   
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Start ESP8266");
  while (WiFi.status() != WL_CONNECTED)delay(500);
//  WiFiClient client;
  WiFi.mode(WIFI_STA);
  Serial.println("\n\nBOOTING ESP8266 ...");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Station IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {

//  server.handleClient();

  // Check if the other Arduino is transmitting
  if (Serial.available())
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<400> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok)
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      Serial.println("ESP8266 Talking now => ");
      Serial.print("Battery 1 Voltage: ");
      Serial.println(doc["Battery 1 Voltage: "].as<float>());
      BATT1 = doc["Battery 1 Voltage: "];

      Serial.print("Battery 2 Voltage: ");
      Serial.println(doc["Battery 2 Voltage: "].as<float>());
      BATT2 = doc["Battery 2 Voltage: "];

      Serial.print("Battery 3 Voltage: ");
      Serial.println(doc["Battery 3 Voltage: "].as<float>());
      BATT3 = doc["Battery 3 Voltage: "];

      Serial.print("Battery 4 Voltage: ");
      Serial.println(doc["Battery 4 Voltage: "].as<float>());
      BATT4 = doc["Battery 4 Voltage: "];

      
      docstring="";
      serializeJson(doc, docstring);        //i dont no docstring is a test for display on webpage

    }
    else
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Flush all bytes in the "link" serial port buffer
      while (Serial.available() > 0)
        Serial.read();
    }
  }  
  
  
//send a json back to the mega
// Create the JSON document
  StaticJsonDocument<200> fromesp;
  fromesp["Battery 1 Voltage: "] = BATT1+1;
  fromesp["Battery 2 Voltage: "] = BATT2+1;
  fromesp["Battery 3 Voltage: "]=  BATT3+1;
  fromesp["Battery 4 Voltage: "]=  BATT4+1;

  serializeJson(fromesp, Serial);
  Serial.println(" done ");
  delay(20000);

// Serial.print("connecting to ");
//    Serial.println(host);

    // Use WiFiClient class to create TCP connections
//    WiFiClient client;
//    const int httpPort = 80;
    if (!client.connect(host, 80)) {
        Serial.println("connection failed");
        return;
    }
  client.print(String("GET http://192.168.100.3/code/vol.php?") + 
                          ("&BATT1=") + BATT1 +
                          ("&BATT2=") + BATT2 +
                          ("&BATT3=") + BATT3 +
                          ("&BATT4=") + BATT4 +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
    }
//
    Serial.println();
    Serial.println("closing connection");

}

 
