#include <SPI.h>
#include <WiFi.h>



char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);
String HTTP_req;          // stores the HTTP request
boolean LED_status = 0;   // state of LED, off by default
int LED1 = 10;
int LED2 = 11;
int LED3 = 12;

void setup() {
  
  // start serial port:
  
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:

}
void loop()
{
   WiFiClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                HTTP_req += c;  // save the HTTP request 1 char at a time
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
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
                    client.println("<h1>LED</h1>");
                    client.println("<p>Click to switch LED1 on and off.</p>");
                    client.println("<form method=\"get\">");
                    ProcessCheckbox(client);
                    client.println("<p>Click to switch LED2 on and off.</p>");
                    client.println("<form method=\"get\">");
                    ProcessCheckbox2(client);
                    client.println("<p>Click to switch LED3 on and off.</p>");
                    client.println("<form method=\"get\">");
                    ProcessCheckbox3(client);
                    client.println("</form>");
                    client.println("</body>");
                    client.println("</html>");
                    Serial.print(HTTP_req);
                    HTTP_req = "";    // finished with request, empty string
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

// switch LED and send back HTML for LED checkbox
void ProcessCheckbox(WiFiClient cl)
{
  
    if (HTTP_req.indexOf("LED1=1") > -1) {  // see if checkbox was clicked
        // the checkbox was clicked, toggle the LED
        if (LED_status) {
            LED_status = 0;
        }
        else {
            LED_status = 1;
        }
    }
    
    if (LED_status) {    // switch LED on
        digitalWrite(1, HIGH);
        // checkbox is checked
        cl.println("<input type=\"checkbox\" name=\"LED1\" value=\"1\" \
        onclick=\"submit();\" checked>LED1");
    }
    else {              // switch LED off
        digitalWrite(1, LOW);
        // checkbox is unchecked
        cl.println("<input type=\"checkbox\" name=\"LED1\" value=\"1\" \
        onclick=\"submit();\">LED1");}
    }

// switch LED and send back HTML for LED checkbox
void ProcessCheckbox2(WiFiClient cl)
{
  
    if (HTTP_req.indexOf("LED2=2") > -1) {  // see if checkbox was clicked
        // the checkbox was clicked, toggle the LED
        if (LED_status) {
            LED_status = 0;
        }
        else {
            LED_status = 1;
        }
    }
    
    if (LED_status) {    // switch LED on
        digitalWrite(2, HIGH);
        // checkbox is checked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\" checked>LED2");
    }
    else {              // switch LED off
        digitalWrite(2, LOW);
        // checkbox is unchecked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\">LED2");}
    }

// switch LED and send back HTML for LED checkbox
void ProcessCheckbox3(WiFiClient cl)
{
  
    if (HTTP_req.indexOf("LED3=3") > -1) {  // see if checkbox was clicked
        // the checkbox was clicked, toggle the LED
        if (LED_status) {
            LED_status = 0;
        }
        else {
            LED_status = 1;
        }
    }
    
    if (LED_status) {    // switch LED on
        digitalWrite(3, HIGH);
        // checkbox is checked
        cl.println("<input type=\"checkbox\" name=\"LED3\" value=\"3\" \
        onclick=\"submit();\" checked>LED3");
    }
     else {              // switch LED off
        digitalWrite(3, LOW);
        // checkbox is unchecked
        cl.println("<input type=\"checkbox\" name=\"LED3\" value=\"3\" \
        onclick=\"submit();\">LED3");}
    }
