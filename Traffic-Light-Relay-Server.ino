/* Create a WiFi access point and provide a web server on it. */
** For more details see http://42bots.com.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

IPAddress    apIP(10, 1, 1, 1);  // Defining a static IP address: local & gateway
                                    // Default IP in AP mode is 192.168.4.1

/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "JustATrafficLight";
const char *password = "";

// Define a web server at port 80 for HTTP
ESP8266WebServer server(80);


const int CH1 = D2;
const int CH2 = D1;
const int CH3 = D5;
const int CH4 = D0;

unsigned int numLights = 4;


void handleRoot() {
  digitalWrite (LED_BUILTIN, 0); //turn the built in LED on pin DO of NodeMCU on
  digitalWrite (ledPin, server.arg("led").toInt());
  ledState = digitalRead(ledPin);

 /* Dynamically generate the LED toggle link, based on its current state (on or off)*/
  char ledText[80];

  if (ledState) {
    strcpy(ledText, "LED is on. <a href=\"/?led=0\">Turn it OFF!</a>");
  }

  else {
    strcpy(ledText, "LED is OFF. <a href=\"/?led=1\">Turn it ON!</a>");
  }

  ledState = digitalRead(ledPin);

  char html[1000];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  int brightness = analogRead(A0);
  brightness = (int)(brightness + 5) / 10; //converting the 0-1024 value to a (approximately) percentage value

// Build an HTML page to display on the web-server root address
  snprintf ( html, 1000,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='10'/>\
    <title>ESP8266 WiFi Network</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: 1.5em; Color: #000000; }\
      h1 { Color: #AA0000; }\
    </style>\
  </head>\
  <body>\
    <h1>ESP8266 Wi-Fi Access Point and Web Server Demo</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Brightness: %d%</p>\
    <p>%s<p>\
    <p>This page refreshes every 10 seconds. Click <a href=\"javascript:window.location.reload();\">here</a> to refresh the page now.</p>\
  </body>\
</html>",

    hr, min % 60, sec % 60,
    brightness,
    ledText
  );
  server.send ( 200, "text/html", html );
  digitalWrite ( LED_BUILTIN, 1 );
}

void handleNotFound() {
  digitalWrite ( LED_BUILTIN, 0 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( LED_BUILTIN, 1 ); //turn the built in LED on pin DO of NodeMCU off
}

void allLightsOn() {
digitalWrite(CH1, HIGH);
digitalWrite(CH2, HIGH);
digitalWrite(CH3, HIGH);
if (numLights = 4) {
digitalWrite(CH4, HIGH);
  }
}

void allLightsOff() {
digitalWrite(CH1, LOW);
digitalWrite(CH2, LOW);
digitalWrite(CH3, LOW);
if (numLights = 4) {
digitalWrite(CH4, LOW);
  }
}

void scrollLights() {
  const int delayTimer = 1000;
  allLightsOff();
  digitalWrite(CH1, HIGH);
  delay(delayTimer);
  digitalWrite(CH1, LOW);
  digitalWrite(CH2, HIGH);
  delay(delayTimer);
  digitalWrite(CH2, LOW);
  digitalWrite(CH3, HIGH);
  delay(delayTimer);
  if (numLights = 4) {
    digitalWrite(CH3, LOW);
    digitalWrite(CH4, HIGH);
    delay(delayTimer);
  }
}

void setup() {


  pinMode(CH1, OUTPUT); //TOP
  pinMode(CH2, OUTPUT); //Middle
  pinMode(CH3, OUTPUT); //Bottom
  pinMode(CH4, OUTPUT); // Only used for 4 high lights

  allLightsOff();  // Start with everything off


	Serial.begin(115200);
	Serial.println();
	Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00

	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);

  server.on ( "/", handleRoot );
  server.on ( "/led=1", handleRoot);
  server.on ( "/led=0", handleRoot);
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );
  server.onNotFound ( handleNotFound );

	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
