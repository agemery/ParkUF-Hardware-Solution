//lets get some things straight
  //this never changes
  const String webAddress = "http://54.148.158.124/parking/"; 
  //this changes depending on the lot #
  const String lot = "35/";
  //this changes based on the direction (entry or exit)
  const String direct = "entry";


void updateServer() {
  Process p;
  p.begin("curl");
  p.addParameter("-X");
  p.addParameter("PUT");
  p.addParameter(webAddress + lot + direct);
  p.run();
  
  while(p.available()>0) {
    char c = p.read();
    Serial.print(c);
  }
  Serial.flush();
  Serial.print("\n");
}
