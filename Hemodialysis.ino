#include <SoftwareSerial.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 6

const int irSensorPin = A0;

// Variables for relays
int Relay1 = 4;
int Relay2 = 5;

unsigned char flowsensor1 = 2; // Sensor Input
unsigned char flowsensor2 = 3; // Sensor Input

int buzzer = 7;

void setup() {
  Serial.begin(9600);

  // Set up relays and sensors
  pinMode(Relay1, OUTPUT); // Relay for pump
  pinMode(flowsensor1, INPUT);
  
  pinMode(Relay2, OUTPUT); // Relay for pump
  pinMode(flowsensor2, INPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  // Turn off both relays for 5 seconds
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  delay(5000);
  
  // Turn on both relays for 5 seconds
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  delay(5000);

  // Read temperature from DHT11 sensor
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);

  // Activate buzzer if temperature is below 36.00°C
  if (DHT.temperature < 36.00) {
    tone(buzzer, 13500);
  }

  // Read flow sensor 1
  uint32_t pulse1 = pulseIn(flowsensor1, HIGH);
  float Hz1 = 1 / (2 * pulse1 * pow(10, -6));
  float flow1 = 0.0137 * (float)Hz1 + 8 * pow(10, -15);
  Serial.println("Flow1:"); // Print litres/hour
  Serial.print(flow1); // Print litres/hour
  Serial.println(" L/hour");
  delay(100);

  // Read flow sensor 2
  uint32_t pulse2 = pulseIn(flowsensor2, HIGH);
  float Hz2 = 1 / (2 * pulse2 * pow(10, -6));
  float flow2 = 0.0137 * (float)Hz2 + 8 * pow(10, -15);
  Serial.println("Flow2:"); // Print litres/hour
  Serial.print(flow2); // Print litres/hour
  Serial.println(" L/hour");
  delay(100);

  // Read IR sensor
  int sensorValue = analogRead(irSensorPin);
  if (sensorValue < 500) {
    Serial.print("Air in line detected!");
    Serial.println(sensorValue);
    digitalWrite(8, HIGH); // Turn on the LED
  } else {
    Serial.print("Fluid");
    Serial.println(sensorValue);
    digitalWrite(8, LOW); // Turn off the LED
  }
}
