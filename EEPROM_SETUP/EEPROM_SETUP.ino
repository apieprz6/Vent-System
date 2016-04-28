#include <EEPROM.h>
/*   This program is used to run to setup all vents. Must be run on
 *   each new arduino before regular code can run. Program can also
 *   be used to reset the EEPROM if anything goes wrong. Can also 
 *   be used to check the EEPROM.
 */

void setup() {
  Serial.begin(9600);
  //UNCOMMENT NEXT LINE TO RESET EEPROM/SET UP EEPROM
  //EEPROM.write(0,1);
}

void loop() {
  int x=0;
  x = EEPROM.read(0);
  Serial.println(x);
  delay(500);
}
