#include <SPI.h>
#include <WiFi.h>
#define MaxHeaderLength 16    //maximum length of http header required
char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);
String HttpHeader = String(MaxHeaderLength); 


void setup(){
  // start serial port:
  
  Serial.begin(9600);
 
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    HttpHeader="";
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:

}
 



void loop(){
  // Create a client connection
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
         char c = client.read();
         //read MaxHeaderLength number of characters in the HTTP header
         //discard the rest until \n
         if (HttpHeader.length() < MaxHeaderLength)
         {
           //store characters to string
           HttpHeader = HttpHeader + c; 
         }
         //if HTTP request has ended
         if (c == '\n') { 
           // show the string on the monitor
           Serial.println(HttpHeader);
          // start of web page
           client.println("HTTP/1.1 200 OK");
           client.println("Content-Type: text/html");
           client.println("Connection: close");
           client.println();
           // send web page
           client.println("<!DOCTYPE html>");
           client.println("<html>");
           client.println("<head>");
           client.println("<title>Arduino LED Control</title>");
           client.println("</head>");
           client.println("<body>");
           client.println("<h1>LED Control</h1>");
           client.println("<p>Click to switch LEDs on and off.</p>");
           client.print("<form method=get>");
           client.print("<input type='radio' name=r value='1'>LED One<br>");
           client.print("<input type='radio' name=r value='2'>LED Two<br>");
           client.print("<input type='radio' name=r value='3'>LED Three<br>");
           client.print("<input type=submit value=submit></form>");
           client.print("</body></html>");
           //clearing string for next read
           HttpHeader="";
           //stopping client
            client.stop();
         }
       }
     }
   }

}
