#include <Process.h>

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
  p.addParameter("http://54.148.158.124/parking/35/entry");
  p.run();
  
  while(p.available()>0) {
    char c = p.read();
    Serial.print(c);
  }
  Serial.flush();
  Serial.print("\n");
}
