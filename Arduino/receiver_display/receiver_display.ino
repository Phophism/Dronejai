#include <Wire.h>
#include <SPI.h>
#include <nRF24L01p.h>
#include <SoftwareSerial.h>
nRF24L01p receiver(7,8);//CSN,CE

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);

void setup(){
  
  delay(150);
  lcd.begin();
  Serial.begin(115200);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(90);  // ตั้งช่องความถี่ให้ตรงกัน
  receiver.RXaddress("ALL");  // ตั้งชื่อตำแห่นงให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
  receiver.init();
}

String message;

void loop(){
  if(receiver.available()){
    receiver.read();
    receiver.rxPL(message);
    Serial.println(message);
    lcd.print(message);
  }else{
    Serial.println("No data received.");
    lcd.setCursor(0,0);
    lcd.println("No data received.");
  }
  message="";
  delay(1000);
}

