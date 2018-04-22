#include <SPI.h>
#include <WiFi.h>
boolean reading = false;

char ssid[] = "LoTemp";      //  your network SSID (name)
char pass[] = "Mot_Catz23*$%";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(1025);



void setup(){
  //Pins 10,11,12 & 13 are used by the ethernet shield
  pinMode(7, OUTPUT);

  WiFi.begin(ssid, pass);
  server.begin();
}

void loop(){
  // listen for incoming clients, and process qequest.
  checkForClient();
}

void checkForClient(){
  WiFiClient client= server.available();

  if (client) {

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean sentHeader = false;

    while (client.connected()) {
      if (client.available()) {
        if(!sentHeader){
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("LED switch");
          
          client.println("<form METHOD=get action=\"");
         
          
          client.println("<input type=hidden name=LED1 value=0>");
          client.println("<input type=submit value=\"off\"></form>");
         
          
         
          client.println("<input type=hidden name=LED1 value=1>");
          client.println("<input type=submit value=\"on\"></form>");
         
          
          client.println("<input type=hidden name=LED2 value=0>");
          client.println("<input type=submit value=\"off\"></form>");
         
          
         
          client.println("<input type=hidden name=LED2 value=1>");
          client.println("<input type=submit value=\"on\"></form>");
             
          
          client.println("<input type=hidden name=LED3 value=0>");
          client.println("<input type=submit value=\"off\"></form>");
          
          
          
          client.println("<input type=hidden name=LED3 value=1>");
          client.println("<input type=submit value=\"on\"></form>");
         
          client.println("<input type=hidden name=LED3 value=0>");
          client.println("<input type=submit value=\"off\"></form>");
          client.println("</>");
          
          sentHeader = true;
        }
        char c = client.read();
        if(reading && c == ' ') reading = false;
        if(c == '?') reading = true; //found the ?, begin reading the info
        if(reading){
          Serial.print(c);
          if(c == '0') {
            digitalWrite(9, LOW);
          }
          if(c == '1') {
            digitalWrite(9, HIGH);
          }
        }
         if (c == '\n' && currentLineIsBlank)  break;
         if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1); // give the web browser time to receive the data
    client.stop(); // close the connection:
  } 
}
