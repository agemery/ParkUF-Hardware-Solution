//Sonar stuff
int sonarPin = 8; //the io pin we are using
float threshhold = 200; //distance threshold in centimeters 
//anything under threshold counts as a car

//PIR stuff
int calibrationTime = 10; //seconds to allow calibration
int pirPin = 11;

void setupPir() {
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  
  
  Serial.println("Allowing PIR to calibrate");
  for(int i=0; i<calibrationTime; i++) {
    delay(1000); //delay is in miliseconds
  }
  
  delay(50);
}

void setupSonar() {
  //not too much to do here
  pinMode(sonarPin, OUTPUT);
}

boolean isCarDetected() {
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
      
    delay(50); //wait before turning the sensor back on
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
