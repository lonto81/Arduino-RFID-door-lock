#include <SPI.h>
#include <WiFi.h>

char ssid[] = "LoTemp";      //  your network SSID (name)
char pass[] = "Mot_Catz23*$%";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(1025);  //server port


byte sampledata=50;            //some sample data - outputs 2 (ascii = 50 DEC)
int ledPin = 4;  // LED pin
char link[]="http://www.scienceprog.com/"; //link data
String readString = String(30); //string for fetching data from address
boolean LEDON = false; //LED status flag
void setup(){
//start Ethernet
  WiFi.begin(ssid, pass);
//Set pin 4 to output
  pinMode(ledPin, OUTPUT);
//enable serial datada print
  Serial.begin(9600);
}
void loop(){
// Create a client connection
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
   if (client.available()) {
    char c = client.read();
     //read char by char HTTP request
    if (readString.length() < 100)
      {
        //store characters to string
        readString += c; //replaces readString.append(c);
      }
        //output chars to serial port
        Serial.print(c);
        //if HTTP request has ended
        if (c == '\n') {
          //dirty skip of "GET /favicon.ico HTTP/1.1"
          if (readString.indexOf("?") <0)
          {
            //skip everything
          }
          else
          //lets check if LED should be lighted
        if(readString.indexOf("L=1") >0)//replaces if(readString.contains("L=1"))
           {
             //led has to be turned ON
             digitalWrite(ledPin, HIGH);    // set the LED on
             LEDON = true;
           }else{
             //led has to be turned OFF
             digitalWrite(ledPin, LOW);    // set the LED OFF
             LEDON = false;
           }
          // now output HTML data starting with standart header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          //set background to yellow
          client.print("<body style=background-color:yellow>");
          //send first heading
          client.println("<font color='red'><h1>HTTP test routines</font></h1>");
          client.println("<hr />");
          client.println("<hr />");
          //output some sample data to browser
          client.println("<font color='blue' size='5'>Sample data: ");
          client.print(sampledata);//lets output some data
          client.println("<br />");//some space between lines
          client.println("<hr />");
          //drawing simple table
          client.println("<font color='green'>Simple table: ");
          client.println("<br />");
          client.println("<table border=1><tr><td>row 1, cell 1</td><td>row 1, cell 2</td></tr>");
          client.println("<tr><td>row 2, cell 1</td><td>row 2, cell 2</td></tr></table>");
          client.println("<br />");
          client.println("<hr />");
          //printing some link
          client.println("<font color='blue' size='5'>Link: ");
          client.print("<a href=");
          client.print(link);
          client.println(">Visit Scienceprog!</a>");
          client.println("<br />");
          client.println("<hr />");
          //controlling led via checkbox
          client.println("<h1>LED control</h1>");
          //address will look like http://192.168.1.110/?L=1 when submited
          if (LEDON)
          client.println("<form method=get name=LED><input type=checkbox name=L value=1 CHECKED>LED<br><input type=submit value=submit></form>");
          else
          client.println("<form method=get name=LED><input type=checkbox name=L value=1>LED<br><input type=submit value=submit></form>");
          client.println("<br />");
          //printing LED status
          client.print("<font size='5'>LED status: ");
          if (LEDON)
              client.println("<font color='green' size='5'>ON");
          else
              client.println("<font color='grey' size='5'>OFF");
          client.println("<hr />");
          client.println("<hr />");
          client.println("</body></html>");
          //clearing string for next read
          readString="";
          //stopping client
          client.stop();
            }
          }
        }
      }
 }
