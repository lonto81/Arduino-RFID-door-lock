#include <SPI.h>
#include <WiFi.h>

char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(1025);


int LEDpin = 5;
String readString = String(30);
String state = String(3);

void setup()
{
  // start the Ethernet connection and the server:
  WiFi.begin(ssid, pass);
  server.begin();
  
  //Sets the LEDpin as an output
  pinMode(LEDpin,OUTPUT);
  
  digitalWrite(LEDpin,LOW);
  state = "OFF";
}

void loop()
{
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        if (readString.length() < 30) {
          readString.concat(c);
        }

        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          int LED = readString.indexOf("LED=");

          if (readString.substring(LED,LED+5) == "LED=T") {
            digitalWrite(LEDpin,HIGH);
            state = "ON";
          }
          else if (readString.substring(LED,LED+5) == "LED=F") {
            digitalWrite(LEDpin,LOW);
            state = "OFF";
          } 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          client.print("LED is ");
          client.print(state);
          client.print("<br><br>");
          
          if (state == "ON") {
            client.println("<a href=\"./?LED=F\">Turn Off<a>");
          }
          else {
            client.println("<a href=\"./?LED=T\">Turn On<a>");
          }
          
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    readString = "";
    // close the connection:
    client.stop();
  }
}
