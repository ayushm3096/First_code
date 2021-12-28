#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <ArduinoJson.h> 
Adafruit_ADS1115 ads; 
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
float KW = 0;
float SOC1=0; // For Battery No.1
float SOC2=0; // For Battery No.2
float SOC3=0; // For Battery No.3
float SOC4=0; // For Battery No.4
float SOCt=0;
float Total_stack = 0;
const int currentPin = A0;
int sensitivity = 185;//185,66
int adcValue= 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;
float POWCOSMP =0;


void setup(void) {
  Serial.begin(9600);
  Serial3.begin(9600);
  ads.begin();  
   
}

void loop(void) {

adcValue = analogRead(currentPin);
adcVoltage = (adcValue / 1024.0) * 5000;
currentValue = (((adcVoltage - offsetVoltage ) / sensitivity))+0.05;//offsetVoltage


int16_t adc0, adc1, adc2, adc3;
  
adc0 = ads.readADC_SingleEnded(0);
adc1 = ads.readADC_SingleEnded(1);
adc2 = ads.readADC_SingleEnded(2);
adc3 = ads.readADC_SingleEnded(3);

voltage1 = ((adc0)*0.1897)/1000;
vbat1 = voltage1/Radjust;

voltage2 = ((adc1)*0.1842)/1000;
vbat2 = voltage2/Radjust;

voltage3 = ((adc2)*0.1858)/1000;
vbat3 = voltage3/Radjust;

voltage4 = ((adc3)*0.1857)/1000;
vbat4 = voltage4/Radjust;
/*
//Serial.println(adc0);
Serial.print("Battery 1 Voltage: ");
Serial.println(vbat1);

//Serial.println(adc1);
Serial.print("Battery 2 Voltage: "); 
Serial.println(vbat2-vbat1);

//Serial.println(adc2);
Serial.print("Battery 3 Voltage: ");
Serial.println(vbat3-vbat2);

//Serial.println(adc3);
Serial.print("Battery 4 Voltage: ");
Serial.println(vbat4-vbat3);

Serial.print("Total Stack Voltage: ");
Serial.println(vbat4);

Serial.print("Total available capacity in kwhr : ");
Serial.println((vbat4*26)/1000);
*/

BATT1 = vbat1;
BATT2 = vbat2-vbat1;
BATT3 = vbat3-vbat2;
BATT4 = vbat4-vbat3;
Total_stack = vbat4;
KW = ((vbat4*26)/1000);
POWCOSMP = ((vbat4)*(currentValue));
SOC1 = ((BATT1-11.4)/1.5)*100; // SOC measurement for battery No.1 
SOC2 = ((BATT2-11.4)/1.5)*100; // SOC measurement for battery No.2 
SOC3 = ((BATT3-11.4)/1.5)*100; // SOC measurement for battery No.3 
SOC4 = ((BATT4-11.4)/1.5)*100; // SOC measurement for battery No.4 
SOCt = ((vbat4-46.04)/5.52)*100;

 if(currentValue<0)
      {
        currentValue=0;
      }

       else
        {
          currentValue = currentValue;
        }

  if(POWCOSMP<0)
      {
        POWCOSMP=0;
      }

         else
      {
        POWCOSMP = POWCOSMP;
      }
  
   if(SOC1<0)// SOC value for Battery 1
        {
          SOC1=0;
        }
         
         else 
        
        {
         SOC1 = SOC1;
        }

    if (SOC2<0)// SOC value for Battery 2
            {
             SOC2 = 0;
             }

          else 
              {
                SOC2 = SOC2;
               }

    if (SOC3<0)// SOC value for Battery 3
           {
            SOC3 = 0;
            }

         else 
            {
              SOC3=SOC3;
             }

    if (SOC4<0)// SOC value for Battery 4 
            
            {
              SOC4=0;
            }

         else 
           {
              SOC4=SOC4;
           }

    if (SOCt<0)// SOC value for Battery 4 
            
            {
              SOCt=0;
            }

         else 
           {
              SOCt=SOCt;
           }
   // SOC measurement for battery No.3 :

   // SOC measurement for battery No.4 :

/*SOC1 = ((BATT1-11.4)/1.6)*100;
SOC2 = ((BATT1-11.4)/1.6)*100;
SOC3 = ((BATT1-11.4)/1.6)*100;
SOC4 = ((BATT1-11.4)/1.6)*100;*/
StaticJsonDocument<200> doc; 
  doc["Battery 1 Voltage: "] =  BATT1;
  doc["Battery 2 Voltage: "] =  BATT2;
  doc["Battery 3 Voltage: "] =  BATT3;
  doc["Battery 4 Voltage: "] =  BATT4;
 /* doc["Battery 1 Charge:  "] =  SOC1;
  doc["Battery 2 Charge:  "] =  SOC2;
  doc["Battery 3 Charge:  "] =  SOC3;
  doc["Battery 4 Charge:  "] =  SOC4;*/
  doc["Total Stack Voltage: "]= Total_stack;
  doc["Total Charge Remaining: "] = SOCt;
  //doc["Total available capacity in kwhr : "] = KW;
  doc["Current Consumption: "] = currentValue;
  //doc["Power Consumption: "] = POWCOSMP;
  

  
  // Send the JSON document over the "link" serial port
  serializeJson(doc, Serial3);
  serializeJson(doc, Serial); 
  Serial.println(" ");
  
delay(2000);
}
