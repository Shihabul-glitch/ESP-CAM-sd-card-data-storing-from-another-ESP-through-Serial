#include <HardwareSerial.h>

HardwareSerial MySerial(2);

void setup() {
  Serial.begin(115200);
  MySerial.begin(921600, SERIAL_8N1, 16, 17);
  delay(200);
  MySerial.write("s44,33,22,11,10,9.9, y,y,y,y,shiahab,n");
}

int i = 0;
String val;

void loop() {
  val = "";
  val = val + "s";
  val = val + String(i);
  val = val + ",44,33,22,11,11.11,11.22,33.44,44.55,55.66,n";
  i = i + 1;

  // --- Convert String to char array ---
  char buffer[100];               // Make sure it's big enough
  val.toCharArray(buffer, sizeof(buffer));

  MySerial.write(buffer);          // Send char array

  delay(40);
}
