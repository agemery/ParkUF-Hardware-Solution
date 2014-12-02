//Sonar stuff
const int sonarPin = 8; //the io pin we are using
const float threshold = 265; //distance threshold in centimeters 
//anything under threshold counts as a car
  
//PIR stuff
const int pirPin = 11;
const int calibrationTime = 10; //seconds to allow calibration
//miliseconds sensor has to be low before all motion has stopped

const int detectionWindow = 4; //duration to poll range finder

void setupSensors() {
  setupPir();
  setupSonar();
}


void setupSonar() {
  //not too much to do here
  pinMode(sonarPin, OUTPUT);
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

int carsDetected() {
  //instead of returning a boolena value indicating at maximum a single car,
  //count the number of cars that have passed in a timining interval 
  //based upon the spaces that exist between cars in a chain of cars
  //for example, if three cars pass in an interval, we count the number
  //of times the distance goes back to ~265(cm).
  //so something like [168,165,268,190,191,192,189,268,267,268,140,141..]
  //we can see that there are two breaks there, thus three cars
  
  if(digitalRead(pirPin == HIGH)) {
    Serial.println("Motion detected @ " + String(millis()/1000) + " seconds");
    int cars = countCars();
    Serial.println(String(cars) + " cars detected");
    return (cars); 
  }
  else {
    return 0;
  }
}

int expandWindow(int w, int start) {
  if ((w - (millis() - start)/1000) <= 2) {
    return (w +1); //extend the window so we do not miss a car
      }
  else
    return w;
}
      

int countCars() {
  unsigned long startTime = millis();
  boolean isFirstCarPassed = false;
  boolean isCarBreak = false;
  int numCars = 0;
  int window = detectionWindow; 
  //adjustable window extends when there is a car chain so we do not miss a 
  //car at the edge of the window 
  
  //s is the length of time we will poll the range finder, in seconds
  while((millis() - startTime)/1000 < window) {
    turnOnRangeFinder();
    //the duration of the pulse is converted into the distance
    float duration = pulseIn(sonarPin, HIGH); 
    // convert to centimeters
    float range = ((duration / 29.412) / 2);
    
    if (!isFirstCarPassed && (range < threshold)) {
      //if the first car has not passed previously, and range is
      //now below threshold, then this car is the first car in the chain
      isFirstCarPassed = true;
      numCars=1;
      window = expandWindow(window, startTime);
    }
    
    if(isFirstCarPassed && !isCarBreak && (range >= (threshold-5))) { 
      //setting it to 260 to be safe
      //if the first car has already passed, and we see the range go 
      //higher than 260, and we are not already on a car break, then
      //this is a break in the chain of cars
      isCarBreak = true;
      window = expandWindow(window, startTime);
    }
    
    if(isFirstCarPassed && isCarBreak && (range < threshold)) {
      //if the first car has already passed, and we have recorded that
      //we were on a break in the chain, and if the range is now below threshold
      //then this is a new car. increment numCars
      numCars++;
      isCarBreak = false; //not on a car break anymore
      window = expandWindow(window, startTime);
    }
    
    Serial.println(range);      
    delay(30); //wait before turning the sensor back on
  }
  return numCars;
}
    
boolean isCarDetected() {
  //call this method to poll the PIR & check the range finder
  //if the range finder returns a value under the threshold, that is
  //interpreted as a car (when no car is present, range finder reads max dist)
  //return true if car is present.
  
  //if the PIR goes high, check the range finder
  if(digitalRead(pirPin) == HIGH) {
    
    Serial.println("Motion detected @ " + String(millis()/1000) + " seconds");
    float dist = getMinRange();
    Serial.println("Distance detected: " + String(dist) + " cm");
    if(dist < threshold)
      return true;
    else
      return false;
  }
  return false;
}

//returns minimum range found over the given interval
float getMinRange() { 
  
  //if we get this number back, something is wrong
  float minRange = 10000.0; 
  
  unsigned long startTime = millis();
  //s is the length of time we will poll the range finder, in seconds
  while((millis() - startTime)/1000 < detectionWindow) {
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
