#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p receiver(7,8); //CSN,CE

 String Send = "";
  String message = "";
  
  String latitude = "";
  String longitude = "";
  String Altitude = "";
  String Humid = "";
  String C = "";
  String F = "";
  String heatC = "";
  String heatF = "";
  String Signal = "";
  String voltage = "";
  String Density = "";
  String Altmean = "";
  String Preusure = "";
  String StaAtmos= "";
  
void setup() {
 // delay(150);
  Serial.begin(9600);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(90);  // ตั้งช่องความถี่ให้ตรงกัน
  receiver.RXaddress("GET");  // รับค่า
   receiver.TXaddress("PUT");  // ส่งค่า
  receiver.init();
}

 

void loop() {


  if (receiver.available()) {
    receiver.read(); // สั่งให้เริ่มอ่าน
    receiver.rxPL(message); // สั่งใหอ่านเก็บไว้ที่ตัวแปร
  }

  //-------------------------------------------------------------------------------//
  //------------------------------GPS---------------------------------------------//
  //------------------------------------------------------------------------------//
  
  if (message.indexOf("1:") > -1) { 
    int i = 2;
      latitude = "";
      longitude = "";
      Altitude = "0 M";
      
   do {
      latitude += message.charAt(i);
      i++;
     }while(message.charAt(i) != ',') ;
     i++;
     do {
      longitude += message.charAt(i);
      i++;
     }while(message.charAt(i) != ',') ;
     i++;
     if(message.charAt(i) != 'x'){
       do {
        Altitude = "";
        Altitude += message.charAt(i);
        i++;
       } while (message.charAt(i) != 'x' );
     }
  }
  delay(200);
  Serial.println("latitude: " + latitude);
  Serial.println("longitude: " + longitude);
  Serial.println("Altitude: " + Altitude);

/*  //-------------------------------------------------------------------------------//
  //------------------------------HUMIDITY-----------------------------------------//
  //------------------------------------------------------------------------------//

  
  
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
     }while(message.charAt(i) != ',') ;
     i++;
     do {
      C += message.charAt(i);
      i++;
     }while(message.charAt(i) != ',') ;
     i++;
     do {
      F += message.charAt(i);
      i++;
     }while(message.charAt(i) != ',') ;
     i++;
     do {
      heatC += message.charAt(i);
      i++;
     }while(message.charAt(i) != ',') ;
     i++;
     do {
      heatF += message.charAt(i);
      i++;
        } while (message.charAt(i) != 'x');
     
    
  }
  delay(200);
  Serial.println("Humid: " + Humid);
  Serial.println("C: " + C);
  Serial.println("F: " + F);
  Serial.println("heatC: " + heatC);
  Serial.println("heatF: " + heatF);


*/  //-------------------------------------------------------------------------------//
  //------------------------------DUST--------------------------------------------//
  //------------------------------------------------------------------------------//



  if (message.indexOf("3:") > -1) {
    int i = 2;
    Signal = "";
    voltage = "";
    Density = "";
    
    do {
      Signal += message.charAt(i);
      i++;
     }while(message.charAt(i) != ',') ;
     i++;
      do {
      voltage += message.charAt(i);
      i++;
     }while(message.charAt(i) != ',') ;
     i++;
    do {
      Density += message.charAt(i);
      i++;
        } while (message.charAt(i) != 'x');
  }
  
  delay(200);
  Serial.println("Signal: " + Signal);
  Serial.println("voltage: " + voltage);
  Serial.println("Densitytude: " + Density);




  //-------------------------------------------------------------------------------//
  //------------------------------Jiro---------------------------------------------//
  //------------------------------------------------------------------------------//

  
  if (message.indexOf("4:") > -1) {
    Preusure = "";
    StaAtmos = "";
    int i = 2;
    do {
      Preusure += message.charAt(i);
      i++;
     }while(message.charAt(i) != ',') ;
     i++;
    do {
      StaAtmos += message.charAt(i);
      i++;
        } while (message.charAt(i) != 'x');
  }
  delay(200);
  
  Serial.println("Preusure: " + Preusure);
  Serial.println("Standard Atmosphere: " + StaAtmos);



   //------------------------------------------------------------------------------//
  //------------------------------Altmean-----------------------------------------//
  //------------------------------------------------------------------------------//


  if (message.indexOf("5:") > -1) {
    int i = 2 ;
    Altmean = "";
    if(message.charAt(i)!='x'){
      i++;
      do {
        Altmean += message.charAt(i);
        i++;
      } while (message.charAt(i) != 'x');
    }
  }
  delay(200);
  Serial.println("Altmean: " + Altmean);


  Serial.println("-------------------------------------------\n-------------------------------------------");
  message = "";
  
}
