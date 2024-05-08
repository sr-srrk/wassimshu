#include <Wire.h>

const int xPin = A0; // Analog pin for X-axis
const int yPin = A1; // Analog pin for Y-axis
const int zPin = A2; // Analog pin for Z-axis

const int numReadings = 100; // Number of readings to average for initial calibration
int readingsX[numReadings], readingsY[numReadings], readingsZ[numReadings]; // Arrays to store readings
int index = 0; // Index for current reading
float totalX = 0, totalY = 0, totalZ = 0; // Variables to store the sum of readings
float offsetX, offsetY, offsetZ; // Offsets for each axis

void setup() {
  Serial.begin(9600);
  // Initialize arrays
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readingsX[thisReading] = 0;
    readingsY[thisReading] = 0;
    readingsZ[thisReading] = 0;
  }
}

void loop() {
  // Read accelerometer values
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int zValue = analogRead(zPin);

  // Calculate running total for each axis
  totalX -= readingsX[index];
  totalY -= readingsY[index];
  totalZ -= readingsZ[index];

  readingsX[index] = xValue;
  readingsY[index] = yValue;
  readingsZ[index] = zValue;

  totalX += xValue;
  totalY += yValue;
  totalZ += zValue;

  // Calculate the mean of the initial values
  if (index < numReadings - 1) {
    index++;
  } else {
    offsetX = totalX / numReadings;
    offsetY = totalY / numReadings;
    offsetZ = totalZ / numReadings;
  }

  // Calculate reference value
  float referenceValue = (offsetX + offsetY + offsetZ) / 3;

  // Adjust readings below reference value plus 2 to zero
  if (xValue < (offsetX + 5)) {
    xValue = 0;
  }
  if (yValue < (offsetY + 5)) {
    yValue = 0;
  }
  if (zValue < (offsetZ + 5)) {
    zValue = 0;
  }

  // Print the adjusted values
  //Serial.print("X: ");
  Serial.print(xValue);
  Serial.print("\t");
  //Serial.print(", Y: ");
  Serial.print(yValue);
  Serial.print("\t");
  //Serial.print(", Z: ");
  Serial.println(zValue);

  // Delay for stability
  delay(100);
}
