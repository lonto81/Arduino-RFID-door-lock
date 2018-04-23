//zoomkat 8-04-12
//simple button GET server code to control servo and arduino pins 5, 6, and 7
//for use with IDE 1.0
//open serial monitor to see what the arduino receives
//use ' instead of " in the html 
//address will look like http://192.168.1.102:84 when submited
//for use with W5100 based ethernet shields
///note that the below bug fix may be required
// http://code.google.com/p/arduino/issues/detail?id=605 

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(1025);

String readString; 

//////////////////////

void setup(){

  pinMode(5, OUTPUT); //pin selected to control
  pinMode(6, OUTPUT); //pin selected to control
  pinMode(7, OUTPUT); //pin selected to control
  pinMode(8, OUTPUT); //pin selected to control
  //start Ethernet
WiFi.begin(ssid, pass);
  server.begin();

  //enable serial data print 
  Serial.begin(9600); 
  Serial.println("server multi pin button test 1.0"); // so I can keep track of what is loaded
}

void loop(){
  // Create a client connection
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string 
          readString += c; 
          //Serial.print(c);
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          Serial.println(readString); //print to serial monitor for debuging 

          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino GET test page</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          client.println("<H1>Zoomkat's simple Arduino button</H1>");
          
          // For simple testing, pin 5, 6, 7, and 8 are used in buttons
          // DIY buttons
          client.println("<a href=/?on2 >ON</a>"); 
          client.println("<a href=/?off3 >OFF</a>"); 
          client.println("&nbsp;<a href=/?off357 >ALL OFF</a><br><br>"); 

          // mousedown buttons
          client.println("<input type=button value=ON onmousedown=location.href='/?on4;'>"); 
          client.println("<input type=button value=OFF onmousedown=location.href='/?off5;'>");        
          client.println("&nbsp;<input type=button value='ALL OFF' onmousedown=location.href='/?off3579;'><br><br>");        
                   
          // mousedown radio buttons
          client.println("<input type=radio onmousedown=location.href='/?on6;'>ON</>"); 
          client.println("<input type=radio onmousedown=location.href='/?off7;'>OFF</>"); 
          client.println("&nbsp;<input type=radio onmousedown=location.href='/?off3579;'>ALL OFF</><br><br>");    
   
          
          // custom buttons
          client.print("<input type=submit value=ON style=width:100px;height:45px onClick=location.href='/?on8;'>");
          client.print("<input type=submit value=OFF style=width:100px;height:45px onClick=location.href='/?off9;'>");
          client.print("&nbsp;<input type=submit value='ALL OFF' style=width:100px;height:45px onClick=location.href='/?off3579;'>");

          client.println("</BODY>");
          client.println("</HTML>");
 
          delay(1);
          //stopping client
          client.stop();

          ///////////////////// control arduino pin
          if(readString.indexOf('2') >0)//checks for 2
          {
            digitalWrite(5, HIGH);    // set pin 5 high
            Serial.println("Led 5 On");
          }
          if(readString.indexOf('3') >0)//checks for 3
          {
            digitalWrite(5, LOW);    // set pin 5 low
            Serial.println("Led 5 Off");
          }
          
          if(readString.indexOf('4') >0)//checks for 4
          {
            digitalWrite(6, HIGH);    // set pin 6 high
            Serial.println("Led 6 On");
          }
          if(readString.indexOf('5') >0)//checks for 5
          {
            digitalWrite(6, LOW);    // set pin 6 low
            Serial.println("Led 6 Off");
          }
          
           if(readString.indexOf('6') >0)//checks for 6
          {
            digitalWrite(7, HIGH);    // set pin 7 high
            Serial.println("Led 7 On");
          }
          if(readString.indexOf('7') >0)//checks for 7
          {
            digitalWrite(7, LOW);    // set pin 7 low
            Serial.println("Led 7 Off");
          }     
          
            if(readString.indexOf('8') >0)//checks for 8
          {
            digitalWrite(8, HIGH);    // set pin 8 high
            Serial.println("Led 8 On");
          }
          if(readString.indexOf('9') >0)//checks for 9
          {
            digitalWrite(8, LOW);    // set pin 8 low
            Serial.println("Led 8 Off");
          }         
             
          //clearing string for next read
          readString="";

        }
      }
    }
  }
} 
