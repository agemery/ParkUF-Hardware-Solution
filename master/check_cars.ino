//Sonar stuff
const int sonarPin; //the io pin we are using

//PIR stuff
int calibrationTime = 10; //seconds to allow calibration
//miliseconds sensor has to be low before all motion has stopped
long unsigned int pause = 100; 
const int pirPin = 11;
int detectionWindow = 2; //seconds

public void setupSensors(int pir, int sonar) {
  setupPir(pir);
  setupSonar(sonar);
}


private void setupSonar(sonar) {
  //not too much to do here
  sonarPin = sonar;
  pinMode(sonarPin, OUTPUT);
}

private void setupPir(pir) {
  pirPin = pir;
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  
  
  Serial.println("Allowing PIR to calibrate");
  for(int i=0; i<calibrationTime; i++) {
    delay(1000); //delay is in miliseconds
  }
  Serial.println("Calibration finished");
  delay(50);
}

public boolean isCarDetected() {
  //call this method to poll the PIR & check the range finder
  //if the range finder returns a value under the threshold, that is
  //interpreted as a car (when no car is present, range finder reads max dist)
  //return true if car is present.
  
  //if the PIR goes high, check the range finder
  if(digitalRead(pirPin) == HIGH) {
    
    Serial.println("Motion detected @ " + String(millis()/1000) + " seconds");
    float dist = getMinRange(10);
    Serial.print("Distance detected: " + String(dist) + " cm");
    if(dist < threshhold)
      return true;
    else
      return false;
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
    
    if (range < minRange)
      minRange = range;
      
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
