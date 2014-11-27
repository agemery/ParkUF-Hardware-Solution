#include <Process.h>
  
  //Forgive me father for I have used Global Variables

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  
  //we will probably take this out when using in the field
  while(!Serial); //wait for serial monitor duh
  Serial.println("Serial up now");
  
  //setting up sensors
  setupSensors();
}

void loop() {
  Serial.println("Polling for cars.");
  if (isCarDetected()) { //if a car is detected update the server
    updateServer();
  }

}


