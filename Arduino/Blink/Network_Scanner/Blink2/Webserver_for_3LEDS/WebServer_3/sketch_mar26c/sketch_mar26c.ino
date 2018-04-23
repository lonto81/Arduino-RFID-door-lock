#include <WiFi.h>
#include <SPI.h>
#include <avr/pgmspace.h>

prog_char string_0[] PROGMEM = "";
prog_char string_1[] PROGMEM = "<br><input type=submit name=b1 value=Led1>";
prog_char string_2[] PROGMEM = "<br><input type=submit name=b2 value=Led2>";
prog_char string_3[] PROGMEM = "<br><input type=submit name=b3 value=Led3>";
prog_char string_4[] PROGMEM = "<br><input type=submit name=b4 value=Led4>";
prog_char string_5[] PROGMEM = "";  //"<br>Insert your name here:";
prog_char string_6[] PROGMEM = "";  //"<input name=msg value=no_name MAXLENGTH=20>";
prog_char string_7[] PROGMEM = "";
prog_char string_8[] PROGMEM = "ON";
prog_char string_9[] PROGMEM = "OFF";

prog_char string_10[] PROGMEM = "<meta http-equiv=refresh content=30 > "; //for auto refresh

PROGMEM const char *string_table[] =     // change "string_table" name to suit
{  
  string_0,
  string_1,
  string_2,
  string_3,
  string_4,
  string_5,
  string_6,
  string_7,
  string_8,
  string_9,
  string_10
};

char buffer[85];    // make sure this is large enough for the largest string it must hold



char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;


String inString = String(35);

WiFiServer server(1025);


boolean led1 = false;
boolean led2 = false;
boolean led3 = false;
boolean led4 = false;

String msg="";
int tam=0;
int st1=9,st2=9,st3=9,st4=9;

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  server.begin();
  Serial.println("Serial READY");
  Serial.println("WiFi READY");
  Serial.println("Server READY");
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
}

void loop()
{

  WiFiClient client = server.available();
 
  int led=0;
  if (client) {
   
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
    
      if (client.available()) {
     
        char c = client.read();}
      
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (inString.length() < 35) {
            inString.concat(c);
           
         }
        if (c == '\n' && current_line_is_blank) {
         
          if(inString.indexOf("b1")>0){
            
             if(led1==false){
               st1=8;
               led1=true;
               digitalWrite(4,HIGH);
             }
             else{
               st1=9;
               led1=false;
               digitalWrite(4,LOW);
             }
             led=1;
            
           }
           if(inString.indexOf("b2")>0){
            
             if(led2==false){
               st2=8;
               led2=true;
               digitalWrite(5,HIGH);
             }
             else{
               st2=9;
               led2=false;
               digitalWrite(5,LOW);
             }
             led=2;
            
           }
           if(inString.indexOf("b3")>0){
            
             if(led3==false){
               st3=8;
               led3=true;
               digitalWrite(6,HIGH);
             }
             else{
               st3=9;
               led3=false;
               digitalWrite(6,LOW);
             }
             led=3;
            
           }
           if(inString.indexOf("b4")>0){
            
             if(led4==false){
               st4=8;
               led4=true;
               digitalWrite(7,HIGH);
             }
             else{
               st4=9;
               led4=false;
               digitalWrite(7,LOW);
             }
             led=4;
            
           }
           /*
           if(inString.indexOf("msg")>0){
              char charBuf1[50];
              char charBuf2[50];
              strcpy(msg,(char*)inString.substring(inString.indexOf("g")+2,inString.indexOf(" H")));                        
              //Serial.print("msg: ");
              Serial.println(msg);
           }
         */
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html><body>");
          client.println("<Center>");
          client.println("<div style='background: #FF8000'>");
          client.println("<div style='border: solid 4px #660000'><p><u><h1>Remote Door Control</p></u></h1>");
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table[0]))); // Necessary casts and dereferencing, just copy.
          client.println( buffer );
          for (int i = 1; i < 8; i++)
          {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i]))); // Necessary casts and dereferencing, just copy.
            client.println( buffer );
            switch(i){
             
              case 1: strcpy_P(buffer, (char*)pgm_read_word(&(string_table[st1]))); client.println( buffer ); break;
              case 2: strcpy_P(buffer, (char*)pgm_read_word(&(string_table[st2]))); client.println( buffer ); break;
              case 3: strcpy_P(buffer, (char*)pgm_read_word(&(string_table[st3]))); client.println( buffer ); break;
              case 4: strcpy_P(buffer, (char*)pgm_read_word(&(string_table[st4]))); client.println( buffer ); break;
             
            }
            delay(30);
          }
       
         
         //strcpy_P(buffer, (char*)pgm_read_word(&(string_table[10]))); client.println( buffer );
  
              
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    inString = "";
    client.stop();
  }
}
