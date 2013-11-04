#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char serverName[] = "lessismorecast.com";
EthernetClient client;

const int whitePin = 9;
const int redPin = 6;
const int bluePin = 5;

unsigned int i=0;
int period=1300;

const unsigned long requestInterval = 14400000;  // delay between requests
unsigned long lastAttemptTime = 0;               // last time connected to the server
String currentLine = "";                         // string to hold the text from server
String forecast = "";                            // string to hold the forecast
boolean readingforecast = false;                 // if you're currently reading the forecast
boolean requested;                               // whether you've made a request since connecting
boolean rise=true;

void setup() {

  pinMode(whitePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  

  currentLine.reserve(256);                      // reserve space for the strings
  forecast.reserve(150);                         // reserve space for forecast strings

  Serial.begin(9600);                            // Start up the ethernet shield
  if (Ethernet.begin(mac) == 0) {                // start ethernet using mac & IP address
    Serial.println("Failed to configure Ethernet using DHCP");  
    while(true);                                 // no point in carrying on, so stay in endless loop
  }

  // Initialize functions:
  getForecast();
  displayForecast();
  connectToServer();
}

void loop() {

  getForecast();
  displayForecast();

}

void displayForecast() {
 
  digitalWrite(whitePin, LOW);                   // Reset LED to off
  digitalWrite(redPin, LOW);                     // Reset LED to off
  digitalWrite(bluePin, LOW);                    // Reset LED to off
  
  if (forecast == ">S") {                        // same temperature - white LED
    digitalWrite(whitePin, HIGH);
  }
  
  if (forecast == ">W") {                        // warmer temperature - red LED
    digitalWrite(redPin, HIGH);
  }
  
  if (forecast == ">C") {                        // colder temperature - blue LED
    digitalWrite(bluePin, HIGH);
  }
  
  if (forecast == ">s") {                        // same temperature with precipitation - white fading LED
    if(i == period) {
      i=1;
      rise= !rise;
    }
    if(rise == false) {
      digitalWrite(whitePin, LOW);
      delayMicroseconds(i);
      digitalWrite(whitePin, HIGH);
      delayMicroseconds(period-i);
      i=i+1;
    }
    if(rise == true) {
      digitalWrite(whitePin, LOW);
      delayMicroseconds(period-i);
      digitalWrite(whitePin, HIGH);
      delayMicroseconds(i);
      i=i+1;
    }
  }
  
  if (forecast == ">w") {                        // warmer temperature with precipitation - red fading LED
    if(i == period) {
      i=1;
      rise= !rise;
    }
    if(rise == false) {
      digitalWrite(redPin, LOW);
      delayMicroseconds(i);
      digitalWrite(redPin, HIGH);
      delayMicroseconds(period-i);
      i=i+1;
    }
    if(rise == true) {
      digitalWrite(redPin, LOW);
      delayMicroseconds(period-i);
      digitalWrite(redPin, HIGH);
      delayMicroseconds(i);
      i=i+1;
    }
  }
  
  if (forecast == ">c") {                        // colder temperature with precipitation - blue fading LED
    if(i == period) {
      i=1;
      rise= !rise;
    }
    if(rise == false) {
      digitalWrite(bluePin, LOW);
      delayMicroseconds(i);
      digitalWrite(bluePin, HIGH);
      delayMicroseconds(period-i);
      i=i+1;
    }
    if(rise == true) {
      digitalWrite(bluePin, LOW);
      delayMicroseconds(period-i);
      digitalWrite(bluePin, HIGH);
      delayMicroseconds(i);
      i=i+1;
    }
  }

}

void getForecast() {

  if (client.connected()) {
    if (client.available()) {                    // read incoming bytes
      char inChar = client.read();
      currentLine += inChar;                     // add incoming byte to end of line
      if (inChar == '\n') {                      // if you get a newline, clear the line
        currentLine = "";
      } 
      
      if (currentLine.endsWith("<text>")) {      // if the current line ends with <text>, it will be followed by the forecast
        readingforecast = true;                  // Clear the forecast string
        forecast = "";
      }

      if (readingforecast) {
        if (inChar != '<') {
          forecast += inChar;
        } 
        else {
          readingforecast = false;               // End of forecast if "<" character is reached
          Serial.println(forecast);   
          Serial.println("disconnecting...");
          client.stop();                         // close the connection to the server
        }
      }
    }   
  }
  
  else if (millis() - lastAttemptTime > requestInterval) {
    connectToServer();  // if you're not connected, and two minutes have passed since your last connection, then attempt to connect again
  }

}

void connectToServer() {
 
  // attempt to connect, and wait a millisecond:
  delay(1000);                                   // give the Ethernet shield a second to initialize
  Serial.println("connecting...");
  if (client.connect(serverName, 80)) {          // Connect to server
    Serial.println("connected...");
    // make HTTP GET request to twitter:
    client.println("GET /oakland.php HTTP/1.0");   // Make a HTTP request
    client.println("HOST: lessismorecast.com");
    client.println("User-Agent: Arduino 1.0");
    client.println();
  }

  lastAttemptTime = millis();                    // note the time of this connect attempt
}   
