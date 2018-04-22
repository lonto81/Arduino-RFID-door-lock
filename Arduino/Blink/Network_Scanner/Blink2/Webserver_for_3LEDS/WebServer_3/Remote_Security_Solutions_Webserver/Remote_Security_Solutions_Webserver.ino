/*
   Remote Security Solutions Web Server
   
  This code creates a simple web page using HTML 5 code 
  and enables you to be able to control your Arduino board
  or components from anywhere in the world over the internet.
  
  created March 21, 2013
  by Tommy Long.
 */
 
#include <SPI.h>
#include <WiFi.h>


char ssid[] = "LoTemp";      //  your network SSID (name) 
char pass[] = "Mot_Catz23*$%";   // your network password


int status = WL_IDLE_STATUS;
WiFiServer server(1025);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    while(true);        // don't continue
  } 

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();                           // start the web server on port 80
  printWifiStatus();   // you're connected now, so print out the status
  
  
   
    }




void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {  
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:    
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            client.println("<HTML>");
            client.println("<HEAD>");
            client.println("<TITLE>Remote Security Solutions</TITLE>");
            client.println("</HEAD>");
            
            
            client.println("<BODY>");
            client.println("<CENTER>");
            
            client.println("<div style='background: #FF8000'>");
            client.println("<div style='border: solid 4px #660000'>");
            client.println("<p><u><H1>Remote Door Control</p></u></H1>");
            client.println("<iframe src=http://sbuy.ipcam.so height=500 width=750 seamless=seamless></iframe>");
            client.println("<br>");
            client.println("<p><h1><b>ATTENTION!!! SOMEONE IS TRYING TO ACCESS A SECURED DOOR!!!</p></h1></b>");
            
            
            
            
            // the content of the HTTP response follows the header:
            client.println("<CENTER>");
            client.print("<H1><p>PLEASE CHOOSE FROM ONE OF THE FOLLOWING OPTIONS BELOW.</p></H1>");
            client.print("<H1><p><font color='#0404B4'>Unlock The Door:</font></H1>");
            client.print("<input type=submit value=UNLOCK style=width:100px;height:45px onClick=location.href='/2\'>");
            client.print("<br>");
            client.print("<H1><p><font color='#0404B4'>Lock The Door:</font></H1>");
            client.print("<input type=submit value=LOCK style=width:100px;height:45px onClick=location.href='/3\'>");
            client.print("<br>");
            client.print("<H1><p><font color='#0404B4'>Sound The Alarm:</font></H1>");
            client.print("<input type=submit value=ALARM! style=width:100px;height:45px onClick=location.href='/5\'>");
            client.print("<input type=submit value=Reset Alarm style=width:100px;height:45px onClick=location.href='/6\'>");
            client.print("<br>");
            
            
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;         
          } 
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }     
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /1")) {
          digitalWrite(4, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /2")) {
          digitalWrite(4, LOW);                // GET /L turns the LED off
        }
         if (currentLine.endsWith("GET /3")) {
          digitalWrite(5, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /4")) {
          digitalWrite(5, LOW);                // GET /L turns the LED off
        }
         if (currentLine.endsWith("GET /5")) {
          digitalWrite(6, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /6")) {
          digitalWrite(6, LOW);                // GET /L turns the LED off
        }
         
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
