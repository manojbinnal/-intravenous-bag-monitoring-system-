
#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
#include "HX711.h"
#include<WiFiServer.h>
#include<WiFiClient.h>
const int LOADCELL_DOUT_PIN = 10;
const int LOADCELL_SCK_PIN = 8;

HX711 scale;
// your network name also called SSID
char ssid[] = "Redmi Note 10 Pro";
// your network password
char password[] = "anmol@1234";
// your network key Index number (needed only for WEP)
int keyIndex = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);      // initialize serial communication
  pinMode(RED_LED, OUTPUT);      // set the LED pin mode

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  
  // you're connected now, so print out the status  
  printWifiStatus();

  Serial.println("Starting webserver on port 80");
  server.begin();                           // start the web server on port 80
  Serial.println("Webserver started!");




  //Serial.begin(57600);
  Serial.println("Load Cell Interfacing with ESP32 - DIY CHEAP PERFECT");
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(794.6);    // this value is obtained by calibrating the scale with known weights as in previous step **********************************************
  scale.tare();                 // reset the scale to 0



  
}

void loop() {

Serial.print("Weight: ");
   
  Serial.println(scale.get_units(10), 1);
   int   Value=scale.get_units(10);
   int Finalvalue =(Value/-186)*100;
   Serial.print("********");
    Serial.print(Finalvalue);
    
  scale.power_down();             // put the ADC in sleep mode
  delay(1000);
  scale.power_up();


  
  int i = 0;
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    char buffer[150] = {0};                 // make a buffer to hold incoming data
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (strlen(buffer) == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            
            //client.println("<html lang='en'><head><style> body{ background-image: url('https://i.pinimg.com/originals/8b/88/c8/8b88c8c0aaea4924c73db383851c0f67.png');background-repeat: no-repeat;background-attachment: fixed;background-size: cover;}</style><title>Document</title></head>  ");
            //client.println(" <body ><center><h1 style='color: #7575dc;text-align: left;font-size: 250%;'> Iot based Intravenous Bag Monitoring system using cc3200</h1><div id='jk' style='height: 25%; width: 50%; background-color: rgba(11, 22, 21, 0.85); border-radius: 10px; border:10px solid rgb(23, 19, 13);margin-top: 254px;margin-bottom: 61px;padding-bottom: 15px;padding-top: 61px;'> <h1 style='color: #b8fff8;' >The value of Bag 1 is <span id='name' >"value"</span>%</h1></div></center></body></html>");
              String response ="HTTP/1.1 200 OK\r\n";
              response += "<!DOCTYPE HTML>\r\n";
              response += "<html lang='en'><meta http-equiv='refresh' content='2'><head><style> body{ background-image: url('https://i.pinimg.com/originals/8b/88/c8/8b88c8c0aaea4924c73db383851c0f67.png');background-repeat: no-repeat;background-attachment: fixed;background-size: cover;}</style><title>Document</title></head>  ";
              response += " <body ><center><h1 style='color: #7575dc;text-align: left;font-size: 250%;'> Iot based Intravenous Bag Monitoring system using cc3200</h1><div id='jk' style='height: 25%; width: 50%; background-color: rgba(11, 22, 21, 0.85); border-radius: 10px; border:10px solid rgb(23, 19, 13);margin-top: 254px;margin-bottom: 61px;padding-bottom: 15px;padding-top: 61px;'> <h1 style='color: #b8fff8;' >The value of Bag 1 is <span id='name' >";
  
              response += Finalvalue;
              response += "%</span>";
              response += "</h1></div></center></body></html>";
              client.print(response);
            
            
            
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear the buffer:
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          buffer[i++] = c;      // add it to the end of the currentLine
        }

        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

//
//a way to check if one array ends with another array
//
boolean endsWith(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  
  //compare the last "compLength" values of the inString
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 1) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
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
