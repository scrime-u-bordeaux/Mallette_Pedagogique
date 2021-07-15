int pin[8] = {0, 1, 3, 4, 15, 16, 17, 18};
int touch[8] = {0};
int lastTouch[8] = {0};
int calib[8] = {0};
int midiCC[8] = {0};

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  calibration();
}

void loop() {
  for (int i = 0; i < 8; i++) {
    touch[i] = touchRead(pin[i]);
    touch[i] = touch[i] * 0.9 + lastTouch[i] * 0.1;
    lastTouch[i] = touch[i];
  }
  for (int i = 0; i < 8; i++) {
    midiCC[i] = (touch[i] - 1.1 * calib[i]) * 255 / calib[i];
    midiCC[i] = constrain( midiCC[i], 0, 127);
    usbMIDI.sendControlChange(101 + i, midiCC[i], 1); 
    Serial.print(midiCC[i]);
    Serial.print("\t");
  }
  Serial.println();
  
  delay(20);
}

void calibration() {
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 8; j++) {
      calib[j] = calib[j] + touchRead(pin[j]);
    }
    delay(10);
  }
  for (int i = 0; i < 8; i++) {
    calib[i] = calib[i] / 100;
  }
}
