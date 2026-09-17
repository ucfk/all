
const int ledPin = 23;      // the PWM pin the LED is attached to
const int freq = 1793;      // set the frequency for 5kHz
const int ledChannel = 0;   // set the PWM channel
const int resolution = 8;   // set PWM resolution
const int dutyCycle = 255 * 0.75;

void setup() {
  ledcSetup(ledChannel, freq, resolution);  // define the PWM Setup
  ledcAttachPin(ledPin, ledChannel);
}

void loop() {
  ledcWrite(ledChannel, dutyCycle);        // set the Duty cycle to 50 out of 255
  delay(15);                       // Wait for 15 mS
}
