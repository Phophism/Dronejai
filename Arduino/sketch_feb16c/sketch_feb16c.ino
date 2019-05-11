// Prueba de medidas de distancia y temperatura con US-100 en modo serie
// Por RafaG, 2014
 
#include <SoftwareSerial.h>;

const int buzzer = 10; 
const int US100_TX = 2;
const int US100_RX = 3;
 
// Instancia nuevo canal serie
SoftwareSerial puertoUS100(US100_RX, US100_TX);
 
unsigned int MSByteDist = 0;
unsigned int LSByteDist = 0;
unsigned int cmDist = 0;
int temp = 0;

 
void setup() {
    Serial.begin(9600);
    puertoUS100.begin(9600);
}
 
void loop() {
 
    puertoUS100.flush(); // limpia el buffer del puerto serie
    puertoUS100.write(0x55); // orden de medición de distancia
 
    delay(500);
 
    if(puertoUS100.available() >= 2) // comprueba la recepción de 2 bytes
    {
        MSByteDist = puertoUS100.read(); // lectura de ambos bytes
        LSByteDist  = puertoUS100.read();
        cmDist  = (MSByteDist * 256 + LSByteDist)/10; // distancia
        if((cmDist > 1) && (cmDist < 1000)) // comprobación de la distancia dentro de rango
        {
            Serial.print("Distance: ");
            Serial.print(cmDist, DEC);
            Serial.println(" cm");
            digitalWrite(buzzer, LOW);
           if(cmDist < 12){
             digitalWrite(buzzer, HIGH);
           }
        }

    }
 
 
    delay(500);
}
