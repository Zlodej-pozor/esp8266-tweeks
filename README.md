# esp8266-tweeks
esp8266 where external hosting is needed. 

package_esp8266com_index.json:
  Litle fix of a json file from https://github.com/earlephilhower/Arduino giving acess to ESP8266 building tools on Arm64 architecture
  If the file on  stated git is newer then this one, use thatone.
  I claim only an edit, not authorship
  
  To use it follow relevant ESP arduino guide substituting The adress with https://github.com/Zlodej-pozor/esp8266-tweeks/raw/main/package_esp8266com_index.json
 

Print.h:
litle edit to enable ussage of F() flash string helper with printf
  To use replace your Print.h inside the relevant core library (eg: ~/.arduino15/packages/esp8266/hardware/esp8266/6.6.10/cores/esp8266/ )
