#include <Process.h>

  //lets get some things straight
  //this never changes
  const char webAddress[] = "http://54.148.158.124/parking/"; 
  //this changes depending on the lot #
  const char lot[] = "35/";
  //this changes based on the direction (entry or exit)
  const char direct[] = "entry";

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  while(!Serial); //wait for serial monitor duh
  Serial.println("Serial up now");
  
  
}

void loop() {
  delay(5000);
  Serial.println("Running curl.");
  runCurl();

}

void runCurl() {
  Process p;
  p.begin("curl");
  p.addParameter("-X");
  p.addParameter("PUT");
  //p.addParameter(webAddress + lot + direct);
  p.run();
  
  while(p.available()>0) {
    char c = p.read();
    Serial.print(c);
  }
  Serial.flush();
  Serial.print("\n");
}
