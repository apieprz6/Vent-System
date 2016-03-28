#include <EEPROM.h>
/*   This program is used to run to setup all vents. Must be run on
 *   each new arduino before regular code can run. Program can also
 *   be used to reset the EEPROM if anything goes wrong
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  EEPROM.write(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x=0;
  x = EEPROM.read(0);
  Serial.println(x);
  delay(500);
}
