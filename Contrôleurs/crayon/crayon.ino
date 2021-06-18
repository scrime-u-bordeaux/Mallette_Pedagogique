int pinCrayon = A9;

const int aisPin  = A0;
const int numReadings  = 10;
int readings [numReadings];
int readIndex  = 0;
long total  = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinCrayon, INPUT);
}

void loop() {
  int valCrayon = analogRead(pinCrayon);
  // on obtient une valeur sur 10 bits soit 1024
  int resCrayon = smooth(map(valCrayon, 350, 800, 0, 127));
  if (resCrayon < 0){
    resCrayon = 0;
  }
  usbMIDI.sendControlChange(60, resCrayon, 1);
  //Serial.println(valCrayon/10);
  Serial.println(resCrayon);
  delay(20);
}

long smooth(int var) { /* function smooth */
  ////Perform average on sensor readings
  long average;
  // subtract the last reading:
  total = total - readings[readIndex];
  // read the sensor:
  readings[readIndex] = var;
  // add value to total:
  total = total + readings[readIndex];
  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;

  return average;
}
