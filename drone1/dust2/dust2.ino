int measurePin = A0;
int ledPower = 2;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  delay(1000);
}

void loop()
{
/*  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 */
  voMeasured = analogRead(measurePin); // read the dust value
/*  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  */
  // 0 - 5.0V mapped to 0 - 1023 integer values 
  calcVoltage = voMeasured * (5.0 / 1024); 
  dustDensity = (0.17 * calcVoltage - 0.1)*1000; 
  Serial.println("Signal=" + (String)voMeasured + " ");
  Serial.println("Voltage=" + (String)calcVoltage + " ");
  Serial.println("Density=" + (String)dustDensity + "ug/m3 ");
  delay(3000);
}
