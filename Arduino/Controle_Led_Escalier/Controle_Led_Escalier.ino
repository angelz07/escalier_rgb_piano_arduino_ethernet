#include "simtronyx_RGB_LED.h"
#include <Ethernet.h>
#include "format_html.h"
#include "decode_request.h"

int marchesBas[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38};
int marchesHaut[] = {38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22};
int nbMarche = 17;
int delayMarche = 200;
int delayOff = 5000;

IPAddress ip(192, 168, 1, 150);
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
int portWebserver = 8282;

#define REDPIN 4
#define GREENPIN 2
#define BLUEPIN 6

String status_led = "0";
simtronyx_RGB_LED strip(REDPIN, GREENPIN, BLUEPIN);

// overall brightness
int brightness = 50;
// speed for animation / color transitions
int fadingDelay = 100;

#define FADING_STEPS 200
#define COLOR_HOLD 20

EthernetServer server(portWebserver);

// Your pet's tricks
#define turn_On 0
#define turn_Off 1


void setup() {
  for (int i = 0; i < nbMarche ; i++) {
      pinMode(marchesBas[i],OUTPUT);
      // Serial.write(marches[i]);
     digitalWrite(marchesBas[i],turn_Off); 
      delay(10);
    }
    
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //init webserveur et connexion lan
  Serial.println("Connexion au réseaux en cours ...");
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  server.begin();
  Serial.print("server connecté!!  adresse :  ");
  Serial.print(Ethernet.localIP());
  Serial.print(" port : ");
  Serial.println(portWebserver);

  strip.animateColorTypeSet(ANIMATETYPE_RGB);
}

void loop() {
  // put your main code here, to run repeatedly:
  strip.loop();
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);


        String req = client.readStringUntil('\r');
        // Match the request
        if (req.indexOf("") != -10) {  //checks if you're on the main page

          String reponse_decodage = decode_request_http(req);
          Serial.println("");
          if (reponse_decodage == "reboot") {
            String repStatus = "HTTP/1.1 200 OK\r\n";
            repStatus += "Content-Type: text/html\r\n\r\n";
            repStatus += "<!DOCTYPE HTML>\r\n<html>\r\n";
            repStatus += "<body>\n";
            repStatus += "<span class='status_reboot'>";
            repStatus += "1";
            repStatus += "</span>\n";


            repStatus += "</body>\n";
            repStatus += "</html>\n";

            client.print(repStatus);

            delay(5000);


            break;
          }
          if (reponse_decodage == "status") {

            String repStatus = "HTTP/1.1 200 OK\r\n";
            repStatus += "Content-Type: text/html\r\n\r\n";
            repStatus += "<!DOCTYPE HTML>\r\n<html>\r\n";
            repStatus += "<body>\n";
            repStatus += "<span class='status_led'>";
            repStatus += status_led;
            repStatus += "</span>\n";


            repStatus += "</body>\n";
            repStatus += "</html>\n";

            // client.print("status_led = ");
            //client.println("Status: 200");
            //client.println("Content-Type: text/html");
            client.print(repStatus);
            break;
          }

          else {
            String retour = action(reponse_decodage);
            /*
              if(retour == "1"){
                 String repStatus = "HTTP/1.1 200 OK\r\n";
                 repStatus += "Content-Type: text/html\r\n\r\n";
                 repStatus += "<!DOCTYPE HTML>\r\n<html>\r\n";
                 repStatus += "<body>\n";
                 repStatus += "<span class='status_led'>";
                 repStatus += retour;
                 repStatus += "</span>\n";


                repStatus += "</body>\n";
                repStatus += "</html>\n";

               // client.print("status_led = ");
                //client.println("Status: 200");
                //client.println("Content-Type: text/html");
                client.print(repStatus);
                break;
              }
              if(retour == "0"){
                String repStatus = "HTTP/1.1 200 OK\r\n";
               repStatus += "Content-Type: text/html\r\n\r\n";
               repStatus += "<!DOCTYPE HTML>\r\n<html>\r\n";
               repStatus += "<body>\n";
               repStatus += "<span class='status_led'>";
               repStatus += 0;
               repStatus += "</span>\n";


              repStatus += "</body>\n";
              repStatus += "</html>\n";

              // client.print("status_led = ");
              //client.println("Status: 200");
              //client.println("Content-Type: text/html");
              client.print(repStatus);
              break;
              }
            */
          }

        }


        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // Prepare the response
          String s = format_page_web();

          client.println(s);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");

  }
  // strip.loop();
}

/*
  etat = 1 = action ON bas vers le haut ordre croissant
  etat = 2 = action OFF bas vers le haut ordre croissant

  etat = 3 = action ON haut vers le bas ordre decroissant
  etat = 4 = action OFF  haut vers le bas ordre decroissant

*/
String action(String req) {

  String retour = "0";
  if (req == "0xF740BF") { //action OFF bas vers le haut ordre croissant
    status_led = "0";
    strip.animateStop();
    action_marche(2);
    
    Serial.println("OFF Bas");

    retour = "1";
  }
  if (req == "0xF7C03F") { //action ON bas vers le haut ordre croissant
    status_led = "1";
    strip.animateStop();
    action_marche(1);
    delay(delayOff);
    action_marche(2);
    Serial.println("ON Bas");

    retour = "1";
  }

  if (req == "0xF7T53R") { //action OFF  haut vers le bas ordre decroissant
    status_led = "1";
    strip.animateStop();
    action_marche(4);

    Serial.println("OFF Haut");

    retour = "1";
  }

  if (req == "0xZ7Z93R") { //action ON haut vers le bas ordre decroissant
    status_led = "1";
    strip.animateStop();
    action_marche(3);
    delay(delayOff);
    action_marche(4);
    Serial.println("ON Haut");

    retour = "1";
  }


  if (req == "0xtest03F") { //Test
    status_led = "1";
    strip.animateStop();
    action_marche(1);
    
    Serial.println("Test");

    retour = "1";
  }
  


  if (req == "0xF700FF") { // brighter
    status_led="1";
      strip.useBrightnessAdjust( true );
      brightness = inc(brightness,10,100);
      strip.setBrightnessRGB(brightness);
      delay(1000);
      Serial.println(brightness);
      retour = "1";
    }

  if (req == "0xF7807F") { // darker
    status_led="1";
     strip.useBrightnessAdjust( true );
     brightness = dec(brightness,10);
      strip.setBrightnessRGB(brightness);
      delay(1000);
      Serial.println(brightness);
      retour = "1";
    }


  if (req == "0xF7D02F") { // slower
    status_led="1";
      fadingDelay = inc(fadingDelay,100,5000);
      strip.animateSpeedSet(fadingDelay);
      delay(1000);
      Serial.println("You clicked 9");
      retour = "1";
    }

  if (req == "0xF7F00F") { // faster
    status_led="1";
      fadingDelay = dec(fadingDelay, 100 );
      strip.animateSpeedSet(fadingDelay);
      delay(1000);
      Serial.println("You clicked 9");
      retour = "1";
    }

   if (req == "0xF7E817") { // fading Massage
    status_led="1";
      strip.animateStop();
      delay(50);
      //digitalWrite(pinRelais,HIGH); // on commande l'allumage
      delay(50);
       strip.animateColorsClear();
       delay(50);
       strip.animateColorAdd(255,47,152,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(175,0,90,FADING_STEPS);  
       delay(50);
       strip.animateColorAdd(145,14,178,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(224,100,255,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(211,36,255,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(108,178,0,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(178,255,62,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(51,72,178,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(98,124,255,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(112,4,178,FADING_STEPS);
       delay(50);
       strip.animateStart();

      Serial.println("fading massage");
      retour = "1";
    }

    if (req == "0xD9ER18") { // fading Massage
      status_led="1";
      strip.animateStop();
      delay(50);
    //  digitalWrite(pinRelais,HIGH); // on commande l'allumage
      delay(50);
       strip.animateColorsClear();
       delay(50);
       strip.animateColorAdd(13,178,61,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(62,255,178,FADING_STEPS); 
       delay(50); 
       strip.animateColorAdd(36,255,100,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(178,1,0,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(255,37,36,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(178,13,136,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(255,91,213,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(255,36,199,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(178,140,15,FADING_STEPS);
       delay(50);
       strip.animateColorAdd(36,65,255,FADING_STEPS);
       delay(50);
       strip.animateStart();
       Serial.println("fading relax");
       retour = "1";
    }

    if (req.indexOf("rgb:") != -1) { // add color to list RGB
        int startRgbRetour = req.indexOf("rgb:");
        String rgb = req.substring(startRgbRetour+4);
       
        if (getValue(rgb,'.',0) != -1 && getValue(rgb,'.',1) != -1 && getValue(rgb,'.',2) != -1 ) {
          String r = getValue(rgb,'.',0);
          String g = getValue(rgb,'.',1);
          String b = getValue(rgb,'.',2);

          int r_int = r.toInt();
          int g_int = g.toInt();
          int b_int = b.toInt();
          
  
          delay(100);
          strip.animateColorAdd(r_int,g_int,b_int,FADING_STEPS);
          retour = "1";
          Serial.print("Add Color : ");
          Serial.print(r_int);
          Serial.print(".");
          Serial.print(g_int);
          Serial.print(".");
          Serial.print(b_int);
          Serial.print(" To list Fading.");
          
        }
      }

      if (req.indexOf("color:") != -1) { // set Color
        
        int startRgbRetour = req.indexOf("color:");
        String rgb = req.substring(startRgbRetour+6);
      
        if (getValue(rgb,'.',0) != -1 && getValue(rgb,'.',1) != -1 && getValue(rgb,'.',2) != -1 ) {
          String r = getValue(rgb,'.',0);
          String g = getValue(rgb,'.',1);
          String b = getValue(rgb,'.',2);

          int r_int = r.toInt();
          int g_int = g.toInt();
          int b_int = b.toInt();
          
         // digitalWrite(pinRelais,HIGH); // on commande l'allumage
          delay(100);
          status_led="1";
          strip.animateStop();
          delay(100);
          strip.setRGB(r_int,g_int,b_int);
          delay(1000);
         
          retour = "1";
          Serial.print("Set Color : ");
          Serial.print(r_int);
          Serial.print(".");
          Serial.print(g_int);
          Serial.print(".");
          Serial.println(b_int);
          
        }
      }
      if (req == "1xclear") { // Clear List Fadding
        
        
        strip.animateStop();
        delay(50); 
        strip.animateColorsClear();
        
        delay(1000);
        Serial.println("Clear LIst Fadding");
        retour = "1";
      }

      if (req == "1xstopfade") { // Stop Fadding
        status_led="0";
        //digitalWrite(pinRelais,LOW); // off commande l'extinction
        delay(50);
        strip.animateStop();
        
        delay(1000);
        Serial.println("Stop Fadding");
        retour = "1";
      }

      if (req == "1xstartfade") { // Start Fadding
        status_led="0";
        //digitalWrite(pinRelais,HIGH); // on commande l'allumage
        delay(50);
        strip.animateStart();
        
        delay(1000);
        Serial.println("Start Fadding");
        retour = "1";
      }





  return retour;
}


void action_marche(int etat) {
  /*
    etat = 1 = action ON bas vers le haut ordre croissant
    etat = 2 = action OFF bas vers le haut ordre croissant

    etat = 3 = action ON haut vers le bas ordre decroissant
    etat = 4 = action OFF  haut vers le bas ordre decroissant

  */
  if (etat == 1) { //etat = 1 = action ON bas vers le haut ordre croissant
    Serial.println("action ON bas vers le haut ordre croissant");
    for (int i = 0; i < nbMarche ; i++) {
      Serial.print(marchesBas[i]);
      Serial.print(" = ON");
      digitalWrite(marchesBas[i],turn_On);
      // Serial.write(marches[i]);
      Serial.println();
      delay(delayMarche);
    }
  }


  if (etat == 2) { //etat = 2 = action OFF bas vers le haut ordre croissant
    Serial.println("action OFF bas vers le haut ordre croissant");
    for (int i = 0; i < nbMarche ; i++) {
      Serial.print(marchesBas[i]);
      Serial.print(" = OFF");
      // Serial.write(marches[i]);
      digitalWrite(marchesBas[i],turn_Off);
      Serial.println();
      delay(delayMarche);
    }
  }


  if (etat == 3) { //action ON haut vers le bas ordre decroissant
    Serial.println("action ON haut vers le bas ordre decroissant");
    for (int i = 0; i < nbMarche ; i++) {
      Serial.print(marchesHaut[i]);
      Serial.print(" = ON");
      digitalWrite(marchesHaut[i],turn_On);
      Serial.println();
      delay(delayMarche);
    }
  }

  if (etat == 4) { //action OFF haut vers le bas ordre decroissant
    Serial.println("action OFF haut vers le bas ordre decroissant");
    for (int i = 0; i < nbMarche ; i++) {
      Serial.print(marchesHaut[i]);
      Serial.print(" = OFF");
      digitalWrite(marchesHaut[i],turn_Off);
      Serial.println();
      delay(delayMarche);
    }
  }


}




int inc( int in, int delta, int limit ) {
  int r = in + delta;
  if( r > limit ) r = limit;
  return r;
}

int dec( int in, int delta ) {
  int r = in - delta;
  if( r < 0 ) r = 0;
  return r;
}
