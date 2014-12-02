#include <Process.h>
  
  //Forgive me father for I have used Global Variables

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  Serial.println("Starting");
  //we will probably take this out when using in the field
  while(!Serial); //wait for serial monitor duh
  Serial.println("Serial up now");
  
  //setting up sensors
  setupSensors();
}

void loop() {
  Serial.println("new loop");
  int numCars = carsDetected(); 
  
  for (int i = 0; i <numCars;i++) {
    Serial.println("Updating server");
    updateServer();
  }
  /*if (isCarDetected()) { //if a car is detected update the server
    Serial.println("Updating server");
    updateServer();
  }*/

}


