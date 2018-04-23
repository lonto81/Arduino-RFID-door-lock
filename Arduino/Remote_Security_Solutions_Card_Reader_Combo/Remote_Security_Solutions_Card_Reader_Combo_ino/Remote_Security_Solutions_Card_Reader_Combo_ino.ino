/**
 * Author: Tommy Long
 * Date 04/24/2013
 */
 
#define RFID_ENABLE 2   //to RFID ENABLE
#define CODE_LEN 10      //Max length of RFID tag
#define VALIDATE_TAG 1  //should we validate tag?
#define VALIDATE_LENGTH  200 //maximum reads b/w tag read and validate
#define ITERATION_LENGTH 200 //time, in ms, given to the user to move hand away
#define START_BYTE 0x0A 
#define STOP_BYTE 0x0D
#include <SPI.h>
#include <WiFi.h>

char ssid[] = "";      //  your network SSID (name) 
char pass[] = "";   // your network password
char tag[CODE_LEN];  
int readLed = 4;
int invalidCardled = 5;
int validCardled = 6;
int ipCam = 7;
int doorLock = 8;
int alarm    = 9;
int status = WL_IDLE_STATUS;
WiFiServer server(1025);

void setup() { 
  Serial.begin(2400);  
  Serial.println("Please Swipe Your Card.");
  pinMode(RFID_ENABLE,OUTPUT);  
  pinMode(readLed, OUTPUT);
  pinMode(validCardled , OUTPUT);
  pinMode(invalidCardled, OUTPUT);
  pinMode(ipCam, OUTPUT);
  pinMode(doorLock, OUTPUT);
  pinMode(alarm, OUTPUT);
    
  if (WiFi.status() == WL_NO_SHIELD) {// check for the presence of the shield:
    Serial.println("WiFi shield not present"); 
    while(true);        // don't continue
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
  printWifiStatus();                        // you're connected now, so print out the status
  }

}
 
void loop() {   //Start our main Arduino Loop
  enableRFID();   //Enable the RFID card
  getRFIDTag();   //Reads the tag
  if(isCodeValid()) {  //Validates that the tag is good
    disableRFID();  //Puts the RFID reader in to low power mode
    sendCode();     //Sends the code read to the serial port
    delay(ITERATION_LENGTH);  //Debounce?
  } else {
    disableRFID();  //Got a incomplete code.. 
  }
  
  Serial.flush();
  clearCode();
  
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
            client.println("<br>");
            client.println("<p><h1><b>ATTENTION!!! SOMEONE IS TRYING TO ACCESS A SECURED DOOR!!!</p></h1></b>");
            
            
            
            
            // the content of the HTTP response follows the header:
            client.println("<CENTER>");
            client.print("<H1><p>PLEASE CHOOSE FROM ONE OF THE FOLLOWING OPTIONS BELOW.</p></H1>");
            client.print("<H1><p><font color='#0404B4'>Unlock The Door:</font></H1>");
            client.print("<input type=submit value=ON style=width:100px;height:45px onClick=location.href='/1\'>");
            client.print("<input type=submit value=OFF style=width:100px;height:45px onClick=location.href='/2\'>");
            client.print("<br>");
            client.print("<H1><p><font color='#0404B4'>Lock The Door:</font></H1>");
            client.print("<input type=submit value=ON style=width:100px;height:45px onClick=location.href='/3\'>");
            client.print("<input type=submit value=OFF style=width:100px;height:45px onClick=location.href='/4\'>");
            client.print("<br>");
            client.print("<H1><p><font color='#0404B4'>Sound The Alarm:</font></H1>");
            client.print("<input type=submit value=ON style=width:100px;height:45px onClick=location.href='/5\'>");
            client.print("<input type=submit value=OFF style=width:100px;height:45px onClick=location.href='/6\'>");
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

        
        if (currentLine.endsWith("GET /1")) {
          digitalWrite(ipCam, HIGH);               
        }
        if (currentLine.endsWith("GET /2")) {
          digitalWrite(ipCam, LOW);                
        }
         if (currentLine.endsWith("GET /3")) {
          digitalWrite(doorLock, HIGH);               
        }
        if (currentLine.endsWith("GET /4")) {
          digitalWrite(doorLock, LOW);               
        }
         if (currentLine.endsWith("GET /5")) {
          digitalWrite(alarm, HIGH);               
        }
        if (currentLine.endsWith("GET /6")) {
          digitalWrite(alarm, LOW);                
        }
         
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

} 
 
/**
 * Clears out the memory space for the tag to 0s.
 */
void clearCode() {
  for(int i=0; i<CODE_LEN; i++) {
    tag[i] = 0; 
  }
}
 
/**
 * Sends the tag to the computer.
 */ 
void sendCode() {
  //This is where I would add a return value (the code) to "validiate" or whatever at.
    Serial.print("Valid Card ID Scanned:");  

    char full_tag[10];
    for(int i=0; i<CODE_LEN; i++) {

      if (i == 9)  //Edits by riley porter
        Serial.println(tag[i]);  //This checks to see if its the last byte
      else                       //If it is it will print a "new line" so that the codes to jumble together
        Serial.print(tag[i]);  
   
    } 
}
 
/**************************************************************/
/********************   RFID Functions  ***********************/
/**************************************************************/
 
void enableRFID() {
   digitalWrite(RFID_ENABLE, LOW);
   digitalWrite(validCardled, HIGH);

}
 
void disableRFID() {
   digitalWrite(RFID_ENABLE, HIGH); 

}
 
/**
 * Blocking function, waits for and gets the RFID tag.
 */
void getRFIDTag() {
   
  byte next_byte; 
  while(Serial.available() <= 0) {}
  if((next_byte = Serial.read()) == START_BYTE) {      
    byte bytesread = 0; 
    while(bytesread < CODE_LEN) {
      if(Serial.available() > 0) { //wait for the next byte
          if((next_byte = Serial.read()) == STOP_BYTE) break;       
          tag[bytesread++] = next_byte; 
      digitalWrite(readLed, HIGH);
      delay(25);
      digitalWrite(validCardled, HIGH);
      delay(25);
      digitalWrite(invalidCardled, HIGH);
      delay(25);
      digitalWrite(readLed, LOW);
      delay(25);
      digitalWrite(validCardled, LOW);
      delay(25);
      digitalWrite(invalidCardled, LOW);
      delay(25);
      digitalWrite(validCardled, HIGH);
      delay(25);
      digitalWrite(validCardled, LOW);
      delay(25);
      digitalWrite(readLed, HIGH);
    
      }
    }                
  }    
}
 
/**
 * Waits for the next incoming tag to see if it matches
 * the current tag.
 */
boolean isCodeValid() {
  byte next_byte; 
  int count = 0;
  while (Serial.available() < 2) {  //there is already a STOP_BYTE in buffer
    delay(1); //probably not a very pure millisecond
    if(count++ > VALIDATE_LENGTH) return false;
  }
  Serial.read(); //throw away extra STOP_BYTE
  if ((next_byte = Serial.read()) == START_BYTE) {  
    byte bytes_read = 0; 
    while (bytes_read < CODE_LEN) {
      if (Serial.available() > 0) { //wait for the next byte      
          if ((next_byte = Serial.read()) == STOP_BYTE) break;
          if (tag[bytes_read++] != next_byte) return false; 
         digitalWrite(invalidCardled, HIGH);
         delay(10);
         digitalWrite(invalidCardled, LOW);
                
      }
    }                
  }

  return true;   
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
