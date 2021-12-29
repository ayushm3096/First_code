#include <ESP8266WiFi.h> // ESP wifi server header file
#include <WiFiClient.h> // Header file for wifi client 
#include <ESP8266WebServer.h> // header file for ESP8266 Server
#include <ESP8266mDNS.h>
#include <Adafruit_ADS1X15.h> // ADS 1115 file
WiFiClient client;
uint8_t i2c_address_of_ADC1 = 0x48;
uint8_t i2c_address_of_ADC2 = 0x49;
Adafruit_ADS1115 adc1, adc2, adc3, adc4; 
const char* ssid ="Vinod"; // wifi user name
const char* password = "archana6887";// wifi passowrd
const char* host = "192.168.100.3";// ip address
float voltage1 = 0; // used to store voltage value
float voltage2 = 0;
float voltage3 = 0;
float voltage4 = 0;
float Radjust = 0.066478076; // Voltage divider factor ( R2 / R1+R2 )
float vbat1 = 0; // For Battery No.1
float vbat2 = 0; //For Battery No.2
float vbat3 = 0; // For Battery No.3
float vbat4 = 0; // For Battery No.4
float BATT1 = 0; // For Battery No.1
float BATT2 = 0; //For Battery No.2
float BATT3 = 0; // For Battery No.3
float BATT4 = 0; // For Battery No.4
float SOCt=0;
float Total_stack = 0;
int sensitivity = 185;//185,66
int adcValue= 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;


void setup()
{
  Serial.begin(115200);// serial port at 115200
  WiFi.begin(ssid, password);// user name and password 
  Serial.println("Start ESP8266"); //serial print 
  while(WiFi.status() != WL_CONNECTED)// checking wifi network availability
  {
    delay(500);
    Serial.print(".");
  }
  WiFi.mode(WIFI_STA);// wifi mode
  Serial.println("\n\nBOOTING ESP8266...");
  Serial.print("Connected to ");
  Serial.println(ssid);// serial print user name 
  Serial.print("Station IP address: ");
  Serial.println(WiFi.localIP());//print ip address 
  

  
}

void loop() 

{
  {
  if(adc1.begin(i2c_address_of_ADC1) && adc2.begin(i2c_address_of_ADC2)){// && adc3.begin(i2c_address_of_ADC3) && adc4.begin(i2c_address_of_ADC4)) {  // if adc value address connect or not 
    Serial.println("Found all the I2C device!");// if ads1115 connected then it will print 
   // all_ok = true;
  } 
  else
{
    Serial.println("Sorry... Couldn't find the device... Try again!");// if ads1115 is not connected then it will print this statment
    return;
  }
  
  int16_t ad1, ad2, ad3, ad4, ad5, ad6, ad7, ad8 ;// initalizing ads parameter
 

  ad1 = adc1.readADC_SingleEnded(0); // A0 of ADS1115 of first IC
  ad2 = adc1.readADC_SingleEnded(1); // A1 of ADS1115 of first IC
  ad3 = adc1.readADC_SingleEnded(2); // A2 of ADS1115 of first IC
  ad4 = adc1.readADC_SingleEnded(3); // A3 of ADS1115 of first IC

  ad5 = adc2.readADC_SingleEnded(0); // A0 of ADS1115 of second IC
  ad6 = adc2.readADC_SingleEnded(1); // A1 of ADS1115 of second IC
  ad7 = adc2.readADC_SingleEnded(2); // A2 of ADS1115 of second IC
  ad8 = adc2.readADC_SingleEnded(3); // A3 of ADS1115 of second IC

// Voltage Calculation Formula

voltage1 = ((ad1)*0.1897)/1000; 
vbat1 = voltage1/Radjust;

voltage2 = ((ad2)*0.1842)/1000;
vbat2 = voltage2/Radjust;

voltage3 = ((ad3)*0.1858)/1000;
vbat3 = voltage3/Radjust;

voltage4 = ((ad4)*0.1857)/1000;
vbat4 = voltage4/Radjust;

//Current Calculation Formula
adcValue = ad5;
adcVoltage = (adcValue / 1024.0) * 5000;
currentValue = (((adcVoltage - offsetVoltage ) / sensitivity))+0.05;//offsetVoltage

BATT1 = vbat1;// Battery 1 voltage  
BATT2 = vbat2-vbat1;// Battery 2 voltage 
BATT3 = vbat3-vbat2;// Battery 3 voltage 
BATT4 = vbat4-vbat3;// Battery 4 voltage 
Total_stack = vbat4;// total stack voltage 
SOCt = ((vbat4-46.04)/5.52)*100;// SOC formula for battery

// if negative or unwanted value shown then it will be zero
 if(currentValue<0)
      {
        currentValue=0;
      }

       else
        {
          currentValue = currentValue;
        }

  

    if (SOCt<0)// SOC value for Battery 4 
            
            {
              SOCt=0;
            }

         else 
           {
              SOCt=SOCt;
           }
  Serial.print("BATT1: ");
  Serial.println(BATT1);// Serial Print of battery 1 voltage.
  Serial.print("BATT2: ");
  Serial.println(BATT2);// Serial Print of battery 2 voltage.
  Serial.print("BATT3: ");
  Serial.println(BATT3);// Serial Print of battery 3 voltage.
  Serial.print("BATT4: ");
  Serial.println(BATT4);// Serial Print of battery 4 voltage.
  Serial.println("TotalVoltage");
  Serial.println(vbat4); //Serial Print Total Stack voltage 
  Serial.println("ChargeRemaining");
  Serial.println(SOCt);//Serial Print of Charge remaining.
  Serial.println("Current");
  Serial.println(currentValue);// Serial current value print.
  
  

  if(!client.connect(host, 80)){       // connect to client  
    Serial.println("connection failed");
    return;
    }

     client.print(String("GET http://192.168.100.2/code3/vol3.php?")+   // URL in which data is hit for storing 
                          ("&BATT1=")+BATT1+
                          ("&BATT2=")+BATT2+
                          ("&BATT3=")+BATT3+
                          ("&BATT4=")+BATT4+ 
                          ("&Total_stack=")+Total_stack+
                          ("&SOCt=")+SOCt+
                          ("&currentValue=")+currentValue+
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
  
  delay(2000); 
  
  
    }
  
