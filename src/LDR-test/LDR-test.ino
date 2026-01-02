// Simple LDR (photoresistor) test for Wemos D1 Mini
// Reads A0, averages samples, prints brightness level

const int analogPin = A0;

// Adjust this threshold after observing your readings
int thresholdValue = 500;

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("LDR Test Starting...");
}

void loop() {
  const int samples = 20;
  long total = 0;

  // Take multiple samples for stability
  for (int i = 0; i < samples; i++) {
    total += analogRead(analogPin);
    delay(5);
  }

  int reading = total / samples;

  // Print raw reading
  Serial.print("ADC: ");
  Serial.print(reading);

  // Interpret brightness
  if (reading > thresholdValue) {
    Serial.println("  -> LIGHT");
  } else {
    Serial.println("  -> DARK");
  }

  delay(300);
}

