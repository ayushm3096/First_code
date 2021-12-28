#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char* ssid ="Vinod";
const char* password = "archana6887";
const char* host = "192.168.100.4";
#define CS 15 
WiFiClient client;
String docstring;
float voltage1 = 0;
float voltage2 = 0;
float voltage3 = 0;
float voltage4 = 0;
float Radjust = 0.066478076;
float vbat1 = 0;
float vbat2 = 0;
float vbat3 = 0;
float vbat4 = 0;
float BATT1 = 0; // For Battery No.1
float BATT2 = 0; //For Battery No.2
float BATT3 = 0; // For Battery No.3
float BATT4 = 0; // For Battery No.4
float KW = 0;
float SOC1=0; // For Battery No.1
float SOC2=0; // For Battery No.2
float SOC3=0; // For Battery No.3
float SOC4=0; // For Battery No.4
float Total_stack = 0;
const int currentPin = A0;
int sensitivity = 185;//185,66
int adcValue= 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;
float POW_COSMP =0;

void setup(){
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Start ESP8266");
  while(WiFi.status() != WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
  Serial.println("\n\nBOOTING ESP8266...");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Station IP address: ");
  Serial.println(WiFi.localIP());
  }

  void loop() {
    

     if(Serial.available())
     {

      StaticJsonDocument<400> doc;

      DeserializationError err = deserializeJson(doc, Serial);

      if (err == DeserializationError::Ok)

      {

        Serial.println("ESP8266 Talking Now => ");
      //  Serial.println(doc["Battery 1 Voltage: "].as<float>());
       // BATT1 = doc["Battery 1 Voltage : "];

        // Serial.println(doc["Battery 2 Voltage: "].as<float>());
      //  BATT2 = doc["Battery 2 Voltage : "];

        //Serial.println(doc["Battery 3 Voltage: "].as<float>());
        //BATT3 = doc["Battery 3 Voltage : "];

        //Serial.println(doc["Battery 4 Voltage: "].as<float>());
        //BATT4 = doc["Battery 4 Voltage : "];

        Serial.println(doc["Battery 1 Charge: "].as<float>());
        SOC1 = doc["Battery 1 Charge: "];

        Serial.println(doc["Battery 2 Charge: "].as<float>());
        SOC2 = doc["Battery 2 Charge: "];

        Serial.println(doc["Battery 3 Charge: "].as<float>());
        SOC3 = doc["Battery 3 Charge: "];

        Serial.println(doc["Battery 4 Charge: "].as<float>());
        SOC4 = doc["Battery 4 Charge: "];
        
        
        Serial.println(doc["Total Stack Voltage: "].as<float>());
        Total_stack = doc["Total Stack Voltage: "];

       // Serial.println(doc["Total available capacity in kwhr : "].as<float>());
        //KW = doc["Total available capacity in kwhr : "];

         Serial.println(doc["Total Charge Remaining: "].as<float>());
        SOCt = doc["Total Charge Remaining: "];
 
        Serial.println(doc["Current Consumption: "].as<float>());
        currentValue = doc["Current Consumption: "];

       // Serial.println(doc["Power Consumption: "].as<float>());
       //POW_COSMP = doc["Power Consumption: "];

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

      StaticJsonDocument<200> fromesp;
  //fromesp["Battery 1 Voltage: "] = BATT1+1;
  //fromesp["Battery 2 Voltage: "] = BATT2+1;
  //fromesp["Battery 3 Voltage: "]= BATT3+1;
  //fromesp["Battery 4 Voltage: "]= BATT4+1;
  fromesp["Battery 1 Charge: "]= SOC1+1;
  fromesp["Battery 2 Charge: "]= SOC2+1;
  fromesp["Battery 3 Charge: "]= SOC3+1;
  fromesp["Battery 4 Charge: "]= SOC4+1;
  fromesp["Total Stack Voltage: "]= Total_stack+1;
  fromesp["Total Charge Remaining: "] = SOCt+1;
  //fromesp["Total available capacity in kwhr : "]= KW+1;
  fromesp["Current Consumption: "]= currentValue+1;
  //fromesp["Power Consumption: "]= POW_COSMP+1;

   serializeJson(fromesp, Serial);
  Serial.println(" done ");
  delay(2000);
  
  if(!client.connect(host, 80)){
    Serial.println("connection failed");
    return;
    }

    client.print(String("GET http://192.168.100.4/code1/vol1.php?")+
                        //  ("&BATT1=") + BATT1 +
                        //  ("&BATT2=") + BATT2 +
                         // ("&BATT3=") + BATT3 +
                         // ("&BATT4=") + BATT4 + 
                          ("&SOC1=") + SOC1 +
                          ("&SOC2=") + SOC2 +
                          ("&SOC3=") + SOC3 +
                          ("&SOC4=") + SOC4 +
                          ("&Total_stack=") + Total_stack +
                        //  ("KW=") + KW +
                          ("&SOCt=")+SOCt+
                          ("currentValue=") + currentValue +
                         // ("POWCOSMP=") + POW_COSMP +

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
