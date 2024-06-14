#include <dummy.h>

// Header Files
#include "thingProperties.h"
#include <ESP32Servo.h>

// Digital Pins
const int Relay = 2;
const int Buzzer = 4;
const int ServoPin = 5;

// Analog Pin
const int LM35Pin = 36;

// Flag Pin
 int flag=1;

// Object create for Servo
Servo servo;

// Function to setup
void setup() {
  
  // Initialize serial and wait for port to open
  Serial.begin(9600);
  
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Pin Configuration
  pinMode(Relay,OUTPUT);   // Relay
  pinMode(Buzzer,OUTPUT);  // Buzzer
  servo.attach(ServoPin);  // Servo Motor

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

// Infinite Loop
void loop() {

  // Realtime Update in cloud
  ArduinoCloud.update();
  
  // Read the analog value from LM35
  int sensorValue = analogRead(LM35Pin);

  // Convert the analog value to voltage
  float voltage = sensorValue * (3.3 / 4095.0); // Assuming 12-bit ADC resolution

  // Convert voltage to temperature (in Celsius)
  float temperatureC = voltage * 100.0; // LM35 has a scale factor of 10 mV/°C

  // Print temperature
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  if(temperatureC>50  && flag)
  {
    // Make buzzer ON for 5 seconds
    digitalWrite(Buzzer,HIGH);
    delay(5000);
    // Turn OFF buzzer
    digitalWrite(Buzzer,LOW);
    // Turn OFF Relay
    relay=0;
    onRelayChange();
    
    // Setting Flag to zero
    flag=0;     
  }

  delay(1000); // Delay for a second
}

// Function to turn ON & OFF relay
void onRelayChange()  
{
   if(relay)
   {
     digitalWrite(Relay,HIGH);
   }
   else
   {
     digitalWrite(Relay,LOW);
   }
}

// Function to decide servo in Clockwise direction
void onForwardChange()  
{
  if(forward)
  {
    rotateServo(60);
  }
}

// Function to decide servo in Anti-Clockwise direction
void onReverseChange()  
{
  if(reverse)
  {
    rotateServo(-60);
  }
}

// Function to rotate servo in Clockwise and Anti-Clockwise direction
void rotateServo(int degrees) 
{
  int angle = map(degrees, -90, 90, 0, 180);
  servo.write(angle);
}
