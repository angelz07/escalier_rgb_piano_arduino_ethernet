#include "Arduino.h"
#include <WString.h>
#include "decode_request.h"


// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


String decode_request_http(String req) {
  String retour = "";
     
    Serial.println(req);
    if (req.indexOf("/0xF740BF") != -1) { //checks if you clicked OFF
      retour = "0xF740BF";
    }
    if (req.indexOf("/0xF7C03F") != -1) { //checks if you clicked ON
      retour = "0xF7C03F";
    }
   
     if (req.indexOf("/0xF7T53R") != -1) { //checks if you clicked OFF
      retour = "0xF7T53R";
    }
    if (req.indexOf("/0xZ7Z93R") != -1) { //checks if you clicked ON
      retour = "0xZ7Z93R";
    }

    if (req.indexOf("/0xF700FF") != -1) { // brighter
      retour = "0xF700FF";
    }

    if (req.indexOf("/0xF7807F") != -1) { // darker
      retour = "0xF7807F";
    }


    if (req.indexOf("/0xF7D02F") != -1) { // slower
      retour = "0xF7D02F";;
    }

    if (req.indexOf("/0xF7F00F") != -1) { // faster
      retour = "0xF7F00F";
    }

    if (req.indexOf("/0xF7E817") != -1) { // fading Massage
      retour = "0xF7E817";
    }

    if (req.indexOf("/0xD9ER18") != -1) { // fading Massage
      retour = "0xD9ER18";
    }

    if (req.indexOf("/status") != -1) { // fading Massage
      retour = "status";
    }

    
    if (req.indexOf("/1xrgb") != -1) { // add color to list fadding
      int startRgb = req.indexOf("/1xrgb");
      int endRgb = req.indexOf("-END");
     
      String RGB = req.substring(startRgb +7 , endRgb);
      if (RGB.indexOf("r") != -1 && RGB.indexOf("g") != -1 && RGB.indexOf("b") != -1 ) { 
        String r = getValue(RGB,'.',0);
        String g = getValue(RGB,'.',1);
        String b = getValue(RGB,'.',2);
        r = r.substring(1);
        g = g.substring(1);
        b = b.substring(1);
      
        retour = "rgb:";
        retour += r;
        retour += ".";
        retour += g;
        retour += ".";
        retour += b;

      }
      else{
         Serial.println("Erreur RGB");
         retour = "error";
        }
      
    }

    if (req.indexOf("/color") != -1) { // set color
      Serial.print("req");
      Serial.println(req);

      // /color-r1.g255.b12-END
      int startRgb = req.indexOf("/color");
      int endRgb = req.indexOf("-END");
     
      String colorRgb = req.substring(startRgb +7 , endRgb);
       if (colorRgb.indexOf("r") != -1 && colorRgb.indexOf("g") != -1 && colorRgb.indexOf("b") != -1 ) { 
        String r = getValue(colorRgb,'.',0);
        String g = getValue(colorRgb,'.',1);
        String b = getValue(colorRgb,'.',2);
        r = r.substring(1);
        g = g.substring(1);
        b = b.substring(1);
      
        retour = "color:";
        retour += r;
        retour += ".";
        retour += g;
        retour += ".";
        retour += b;


      }
      else{
         Serial.println("Erreur Set Color RGB");
         retour = "error";
        }
      
    }

    if (req.indexOf("/1xclear") != -1) { // darker
      retour = "1xclear";
    }

    if (req.indexOf("/0xtest03F") != -1) { // darker
      retour = "0xtest03F";
    }

    if (req.indexOf("/1xstartfade") != -1) { // darker
      retour = "1xstartfade";
    }

    if (req.indexOf("/1xstopfade") != -1) { // darker
      retour = "1xstopfade";
    }

    if (req.indexOf("/reboot") != -1) { // darker
      retour = "reboot";
    }

  return retour;

}


