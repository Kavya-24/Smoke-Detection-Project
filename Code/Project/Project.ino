#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "MQ135.h"

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
  
char authCode[] = "......sth";         //Auth code for BLYNK
char ssid[] = "wifi_name";                                        //Wifi Password
char pass[] = "wifi_password";                                    //Password



//Constants used
const int bitRate = 115200;
const int gasThreshold = 190;                                 //Threshold Value

int relay = 13;

//Sensor
MQ135 gasSensor = MQ135(A0);

    
//Buzzer
int Buzzer = 8;


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(bitRate);
  delay(10);

  pinMode(relay, OUTPUT);
  
  //Not low because the connections were reveresed with glue
  digitalWrite(relay, HIGH);
  
  Blynk.config(authCode); 
        
}


void loop() {

     delay(6000);    

     Serial.println("In Loop");
     
     float h = analogRead(A0);
     float ppm = gasSensor.getPPM();

     if(isnan(h)){
      Serial.println("Failed to read data from the sensor");
      return;
     }
     
    //H values
    Serial.println(" ");
    Serial.print(h);
    

    //Detect Gas
    int conductivity = round((h/1023)*100);

    //String status and state
    String gasStatus = "Normal";
    int gasState = 0;

        
    if(h <=190){
      gasStatus = "Normal";
      gasState = 0; 
    }

    else if(h <=250){
      gasStatus = "Critical";
      gasState = 1;
    }


    else{
      
      gasStatus = "Dangerous";
      gasState = 1;
      
    }


    if(gasState == 1){
        
         //Not high because the connections were reveresed with glue
         digitalWrite(relay, LOW);
         tone(14,300,500);
         Serial.println("Firing message");
         String statusToSend = "Gas Levels is " + gasStatus + "\n" + "PPM Level is " + String(ppm);
         Serial.println(statusToSend);
         Blynk.connectWiFi(ssid,pass);
          
//         while(!Blynk.connected()){
//            Serial.println("Connecting..");
//            delay(500);             //Check every 0.5 seconds
//         }
         
          Blynk.run();
          Blynk.notify(statusToSend);
          Serial.println("Message sent");
         

        delay(5000);
    }
    
    
    

    //Add buzzer for 9
    //Not low because the connections were reveresed with glue
    digitalWrite(relay, HIGH);

    
  
}
