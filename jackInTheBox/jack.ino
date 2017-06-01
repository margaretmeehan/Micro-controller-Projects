Servo myservo;
const int baudSpeed = 9600;

const int servo_pin = D0;
const int tonePin = D3;
const int distancePin = A0;
const int buttonPin = A1;

// Light Pins
const int bluePin = D1;
const int greenPin = D2;
const int redPin = D4;

float handDistance;
// Range hand should be in to turn off alarm
float maxHand = 600;
float minHand = 200;

// Time from now you want alarm to go off. Short for testing.
int inputTime = 10000;

// State variables
bool handSwipe = false;
bool buttonPress = false;
bool timerDone = false;

// current state of button
byte current = LOW;
// Previous state of the button
byte old = LOW;

void setup() {
  Serial.begin(baudSpeed);
  myservo.attach(servo_pin);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(tonePin, OUTPUT);
  pinMode(distancePin, INPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  if (Serial.available()>0){

    // Get state of button
    current = digitalRead(buttonPin);
    if ((old == LOW) && (current == HIGH)){
      // Button being pressed

      //User pressed the button, initialize alarm state variables appropriately.
      buttonPress = true;
      handSwipe = false;
    }
    // Remember the current state of the button
    old = current;

    // If the button was pressed, figure out what state the alarm should be in.
    if (buttonPress){

      //if the timer isn't up, delay inputTime seconds for nap duration
      if (!timerDone && !handSwipe) {
        delay(inputTime);

        timerDone = true;
      }

      // Check if the hand has swiped the alarm off
      if (handSwipe) {
        // Turn alarm off


        // Close the box, turn on the light, off the alarm
        // (one of the D pins wasn't working so just used Red)
        myservo.write(90);
        setColor(0, 0, 0);
        noTone(tonePin);
        delay(1000);

        //reset variables
        buttonPress = false;
        handSwipe = false;
      }

      // If hand hasn't silenced alarm yet
      if (!handSwipe && buttonPress) {
        // Open the box, turn on the light, turn on the sound
        myservo.write(180);
        setColor(0, 255, 255);
        tone(tonePin, 500);
        delay(1000);

        // Check if the hand has swiped in this loop
        handDistance = getDistance();
        if (handDistance >= minHand && handDistance <= maxHand){
          // If hand is within a reasonable range, user turned the alarm off.
          handSwipe = true;
          timerDone = false;
        }
      }
    }
    delay(500);
  }
}

/* Set color of the RBG light */
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

float getDistance(){
  float distance = analogRead(distancePin);
  distance = constrain(distance, 0, 4095);
  distance = map(distance, 0, 4095, 0, 3300);
  distance /= 1000.0;
  //convert to distance from voltage
  int physicalDistance = (int) (70.0/distance) - 6.0;
  return physicalDistance * 10;
}
