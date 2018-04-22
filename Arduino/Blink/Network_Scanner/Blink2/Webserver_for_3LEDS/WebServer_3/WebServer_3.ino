#include <SPI.h>
#include <WiFi.h>



char ssid[] = "LoTemp";      //  your network SSID (name)
char pass[] = "Mot_Catz23*$%";   // your network password

int status = WL_IDLE_STATUS;
WiFiServer server(1025);
String HTTP_req;          // stores the HTTP request
boolean LED_status = 0;   // state of LED, off by default

int numPins = 5;
int pins [] = {3, 4, 5, 6, 7};
int pinState[] = {0, 0, 0, 0, 0};
char line1[100];

void setup() 
{
  for (int i = 0; i < numPins; i++)
  
  {
      pinMode(pins[i], OUTPUT);
  }
  // start serial port:
  
  Serial.begin(9600);

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
  void loop ()
  {
    WiFiClient client = server.available();
    if (client)
    {
      while (client.connected())
      {
        readHeader(client);
        if (! pageNameIs("/"))
        {
          client.stop();
          return;
        }
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        
        // send the body
        client.println("<html><body>");
        client.println("<Center>");
        client.println("<div style='background: #FF8000'>");
        client.println("<div style='border: solid 4px #660000'><p><u><h1>Remote Door Control</p></u></h1>");
        client.println("<form method= 'GET'>");
        setValuesFromParams();
        setPinStates();
        for (int i = 0; i < numPins; i++)
        {
          writeHTMLforPin(client, i);
        }
        client.println("<input type='submit' value='Submit'/>");
        client.println("</form>");
        client.println("</body></html>");
        client.println("</div>");
        client.stop();
     
      }
    }
  }
  void writeHTMLforPin(WiFiClient client, int i)
  {
        
        client.println("<CENTER>");
        client.println("<p><font color='#0404B4'>Option</font>");
        client.print(pins[i]);
        client.print("<select name='");
        client.print(i);
        client.println("'>");
        client.println("</CENTER>");
        client.print("<option value='0'");
      
        
        if (pinState[i] == 0)
        {
          client.println("selected");
        }
        client.println(">Lock</option>");
        client.print("<option value='1'");
        if (pinState[i] ==1
        )
        {
          client.print("selected");
        }
        client.println(">Unlock</option>");
        client.println("<select></p>");
   
       
  }
  void setPinStates()
  {
    for (int i = 0; i < numPins; i++)
    {
      digitalWrite(pins[i], pinState[i]);
    }
  }
  void setValuesFromParams()
  {
    for (int i = 0; i < numPins; i++)
    {
      pinState[i] = valueOfParam(i + '0');
    }
  }
  void readHeader(WiFiClient client)
  {
    // read first line of header
    char ch;
    int i = 0;
    while (ch != '\n')
    {
      if (client.available())
      {
        ch = client.read();
        line1 [i] = ch;
        i ++;
      }
    }
    line1[i] = '\0';
    Serial.println(line1);
  }
  boolean pageNameIs(char* name)
  {
    // page name starts at char  pos 4
    // ends with space
    int i = 4;
    char ch = line1[i];
    while (ch != ' ' && ch != '\n' && ch != '?')
    {
      if (name[i-4] != line1[i])
      {
        return false;
      }
      i++;
      ch = line1[i];
    }
    return true;
  }
  int valueOfParam(char param)
  {
    for (int i = 0; i < strlen(line1); i++)
    {
      if (line1[i] == param && line1[i+1] == '=')
      {
        return (line1[i+2] - '0');
      }
    }
    return 0;
  }
  
