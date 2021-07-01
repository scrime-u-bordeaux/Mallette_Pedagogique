//#define DEBUG

// Attention l'un des theremin possède les trigPin et echoPin inversés (1,0 et 19,18);
int trigPin1 = 0;    // Trigger
int echoPin1 = 1;    // Echo
long duration1, cm1, inches1;

int trigPin2 = 18;    // Trigger
int echoPin2 = 19;    // Echo
long duration2, cm2, inches2;

int n = 0;
int refn = 0;
int m = 0;
int refm = 0;

int hauteur1;
int hauteur2;

const int numReadings  = 10;
int readings1 [numReadings];
int readIndex1  = 0;
long total1  = 0;
int readings2 [numReadings];
int readIndex2  = 0;
long total2  = 0;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  pinMode(echoPin1, INPUT);
  duration1 = pulseIn(echoPin1, HIGH, 4000);
  int duree1 = 2*smooth1(duration1);

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH, 4000);
  int duree2 = 2*smooth2(duration2);

#ifdef DEBUG
  Serial.print("Durée 1 smooth : ");
  Serial.print(duree1);
  Serial.print("\t");
  Serial.print("Durée 2 smooth : ");
  Serial.print(duree2);
  Serial.print("\t");
  delay(50);
#endif

  if (duree1 > 0) {
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

  int val1 = map(duree1, 130, 5500, 0, 127);
  val1 = constrain(val1, 0, 127);

  usbMIDI.sendControlChange(60, val1, 1);

  if (duree2 > 0) {
    m = 1;
  } else {
    m = 0;
  }

  // Envoi d'une note
  if ((m == 1) && (m != refm)) {
    //Serial.println("Note");
    usbMIDI.sendNoteOn(61, 127, 1);
    refm = m;
  }

  if ((m == 0) && (m != refm)) {
    //Serial.println("Pas note");
    usbMIDI.sendNoteOff(61, 127, 1);
    refm = m;
  }

  int val2 = map(duree2, 130, 5500, 0, 127);
  val2 = constrain(val2, 0, 127);

  usbMIDI.sendControlChange(61, val2, 1);

#ifdef DEBUG
  Serial.print("Val1 : ");
  Serial.print(val1);
  Serial.print("\t");
  Serial.print("Val2 : ");
  Serial.print(val2);
  Serial.println();
#endif

  delay(20);
}

long smooth1(int duration) { /* function smooth */
  long average1;
  total1 = total1 - readings1[readIndex1];
  readings1[readIndex1] = duration;
  total1 = total1 + readings1[readIndex1];
  readIndex1 = readIndex1 + 1;
  if (readIndex1 >= numReadings) {
    readIndex1 = 0;
  }
  average1 = total1 / numReadings;
  return average1;
}

long smooth2(int duration) { /* function smooth */
  long average2;
  total2 = total2 - readings2[readIndex2];
  readings2[readIndex2] = duration;
  total2 = total2 + readings2[readIndex2];
  readIndex2 = readIndex2 + 1;
  if (readIndex2 >= numReadings) {
    readIndex2 = 0;
  }
  average2 = total2 / numReadings;
  return average2;
}
