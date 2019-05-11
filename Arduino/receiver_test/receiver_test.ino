
#include <SPI.h>
#include <nRF24L01p.h>
#include <Wire.h>

nRF24L01p receiver(7,8); //CSN,CE


String Send = "";
String message = "";

String Latitude = "";
String Longitude = "";
String Altitude = "";
String Humid = "";
String C = "";
String F = "";
String heatC = "";
String heatF = "";
String Signal = "";
String Voltage = "";
String Density = "";
String Altmean = "";
String Preusure = "";
String StaAtmos = "";




  void setup() {
    // delay(150);
    Serial.begin(115200);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    receiver.channel(30);  // ตั้งช่องความถี่ให้ตรงกัน
    receiver.RXaddress("AAA");  // รับค่า
    receiver.TXaddress("PUT");  // ส่งค่า
    receiver.init();
  }
  
  
  
  void loop() {
  
    if (receiver.available()) {
      receiver.read(); // สั่งให้เริ่มอ่าน
      receiver.rxPL(message); // สั่งใหอ่านเก็บไว้ที่ตัวแปร
    }
 
    if (message.indexOf("2:") > -1) {
      int i = 2;
      Humid = "";
      C = "";
      F = "";
      heatC = "";
      heatF = "";
  
      do {
        Humid += message.charAt(i);
        i++;
      } while (message.charAt(i) != ',') ;
      i++;
      do {
        C += message.charAt(i);
        i++;
      } while (message.charAt(i) != ',') ;
      i++;
      do {
        F += message.charAt(i);
        i++;
      } while (message.charAt(i) != ',') ;
      i++;
      do {
        heatC += message.charAt(i);
        i++;
      } while (message.charAt(i) != ',') ;
      i++;
      do {
        heatF += message.charAt(i);
        i++;
      } while (message.charAt(i) != 'x');
  
  
    }
    delay(200);
 
    Serial.println("Humid : " + Humid +"%");
  
  
    Serial.println("C : " + C +"c");
   
    Serial.println("F : " + F);
  
    Serial.println("heatC: " + heatC);
  
    Serial.println("heatF: " + heatF);

    Serial.println("\n");
  

  Serial.println("-------------------------------------------\n-------------------------------------------");
  message = "";


}
