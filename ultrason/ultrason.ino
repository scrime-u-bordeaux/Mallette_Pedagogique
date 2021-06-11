// #define DEBUG

int trigPin = 0;    // Trigger
int echoPin = 1;    // Echo
long duration, cm, inches;
int n = 0;
int refn = 0;

int hauteur;

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH, 4000);

  // Convert the time into a distance
  cm = (duration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
  inches = (duration / 2) / 74; // Divide by 74 or multiply by 0.0135

#ifdef DEBUG
  Serial.println(duration);
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
#endif

  if (duration > 0) {
    n = 1;
  } else {
    n = 0;
  }

  // Envoi d'une note
  if ((n == 1) && (n != refn)) {
    //Serial.println("Note");
    usbMIDI.sendNoteOn(60, 127, 1);
    refn = n;
  }

  if ((n == 0) && (n != refn)) {
    //Serial.println("Pas note");
    usbMIDI.sendNoteOff(60, 127, 1);
    refn = n;
  }
  
  int val1 = map(duration, 130, 3000, 0, 127);
  val1 = constrain(val1,0,127);

  Serial.println(val1);
  delay(20);

  usbMIDI.sendControlChange(60, val1, 1);

}
