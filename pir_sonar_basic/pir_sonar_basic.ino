//Sonar stuff
int sonarPin = 8; //the io pin we are using

//PIR stuff
int calibrationTime = 10; //seconds to allow calibration
//miliseconds sensor has to be low before all motion has stopped
long unsigned int pause = 100; 
int pirPin = 11;
int detectionWindow = 2; //seconds

void setup() {
  Serial.begin(9600);
  while(!Serial); //wait for serial monitor duh
  Serial.println("PIR-Sonar test. No network.");
  setupPir();
  setupSonar();
}

void setupPir() {
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  
  
  Serial.println("Allowing PIR to calibrate");
  for(int i=0; i<calibrationTime; i++) {
    delay(1000); //delay is in miliseconds
  }
  Serial.println("Calibration finished");
  
  delay(50);
}

void setupSonar() {
  //not too much to do here
  pinMode(sonarPin, OUTPUT);
}

void loop() {
  if(digitalRead(pirPin) == HIGH) {
    
    Serial.println("Motion detected @ " + String(millis()/1000) + " seconds");

    Serial.println("Distance detected: " + String(getMinRange(detectionWindow))
    + " cm");

    delay(pause);
  }
}

//returns minimum range found over the given interval
float getMinRange(int s) { 
  
  //if we get this number back, something is wrong
  float minRange = 10000.0; 
  
  unsigned long startTime = millis();
  //s is the length of time we will poll the range finder, in seconds
  while((millis() - startTime)/1000 < s) {
    turnOnRangeFinder();
    //the duration of the pulse is converted into the distance
    float duration = pulseIn(sonarPin, HIGH); 
    // convert to centimeters
    float range = ((duration / 29.412) / 2);
    
    if ((range < minRange))
      minRange = range;
      Serial.println(range);
      
    delay(30); //wait before turning the sensor back on
  }
   
  return minRange;
}

  
void turnOnRangeFinder() {
  pinMode(sonarPin, OUTPUT);
  //this sequences turns on the sonar range finder
  digitalWrite(sonarPin, LOW); //send low pulse
  delayMicroseconds(2); //wait 2 uS
  digitalWrite(sonarPin, HIGH); //high pulse brah
  delayMicroseconds(5); //5 uS wait
  digitalWrite(sonarPin, LOW); //back off again
  pinMode(sonarPin, INPUT); // make it an input pin
  digitalWrite(sonarPin, HIGH); //turn on pullup resistor
  //making pin active low
}
