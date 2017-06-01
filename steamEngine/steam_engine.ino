//Motor Control Constants
const int baudSpeed = 9600;
const int motorEnablePin = D0;
const int forward = D1;
const int reverse = D2;
const int encoderPin = D3;

boolean old_state = LOW;
boolean current_state = LOW;
int pulse_count;

/* the number of pulses that the sensor reads
 when the motor-controlled lego wheel completes
 one full turn at 20% speed. About 14.67 rotations (880/60)
 per one entire lego wheel rotation */
const int full_turn_count = 880;

// determines if we should move forward or reverse
boolean forward_rotation = TRUE;

void setup() {
  Serial.begin(baudSpeed);
  pinMode(motorEnablePin, OUTPUT);
  pinMode(forward, OUTPUT);
  pinMode(reverse, OUTPUT);

  analogWrite(motorEnablePin, 0);
  digitalWrite(forward, HIGH);
  digitalWrite(reverse, LOW);

  pinMode(encoderPin, INPUT);
}

void loop(){
  pulse_count = 0;
  //move the motor forward
  if (forward_rotation) {
    analogWrite(motorEnablePin, 51);
    digitalWrite(forward, 51);
    digitalWrite(reverse, LOW);
    analogWrite(motorEnablePin, 51);
  }
  //move the motor in reverse
  else {
    digitalWrite(motorEnablePin, LOW);
    digitalWrite(forward, LOW);
    analogWrite(reverse, 51);
    analogWrite(motorEnablePin, 51);
  }

  while (pulse_count < full_turn_count) {
    Particle.process();
    //read the sensor
    current_state = digitalRead(encoderPin);
    //if we switch from 0 to 1 or vice versa
    if (current_state != old_state) {
      //increment the pulse count
      pulse_count++;
    }
    old_state = current_state;
  }
  //turn off the motor when we have reached full_turn_count
  analogWrite(motorEnablePin, 0);
  //make sure we switch rotation on the next cycle
  forward_rotation = !forward_rotation;
  //delay 5 seconds
  delay(5000);
}
