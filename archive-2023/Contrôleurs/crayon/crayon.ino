int pinCrayon = A0;

const int numReadings  = 10;
int readings [numReadings];
int readIndex  = 0;
long total  = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinCrayon, INPUT);
  analogReadResolution(16);
}

void loop() {
  int valCrayon = analogRead(pinCrayon);
  int resCrayon = smooth(map(valCrayon, 2000, 9000, 0, 127));
  resCrayon = constrain(resCrayon, 0, 127);
  if (resCrayon < 0){
    resCrayon = 0;
  }
  usbMIDI.sendControlChange(60, resCrayon, 1);
  Serial.println(valCrayon);
  //Serial.println(resCrayon);
  delay(20);
}

long smooth(int var) { /* function smooth */
  long average;
  total = total - readings[readIndex];
  readings[readIndex] = var;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;

  return average;
}
