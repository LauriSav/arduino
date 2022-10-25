#include "DHT.h"
#include <SPI.h>

//WIFI
#include <WiFiNINA.h>
#include "wifi_credentials.h"

char ssid[] = SECRET_SSID;                // your network SSID (name)
char pass[] = SECRET_PASS;                // your network password (use for WPA, or use as key for WEP)

#include <Tiny4kOLED.h>
#include <Servo.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int status = WL_IDLE_STATUS;             // the Wi-Fi radio's status
int ledState = LOW;                       //ledState used to set the LED
unsigned long previousMillisInfo = 0;     //will store last time Wi-Fi information was updated
unsigned long previousMillisLED = 0;      // will store the last time LED was updated
const int intervalInfo = 5000;            // interval at which to update the board information

Servo servo;
int angle = 10;


#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    dht.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.clearDisplay();
    display.setCursor(0, 15);
    display.print("terve");

    display.display();
    delay(1);

    servo.attach(8);
    servo.write(angle);
    
    Serial.begin(9600);
    while (!Serial);

    // set the LED as output
    pinMode(LED_BUILTIN, OUTPUT);


    // attempt to connect to Wi-Fi network:
    while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
     Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);

}
}

void loop() {
  // put your main code here, to run repeatedly:
    float h = dht.readHumidity();
    float t = dht.readTemperature();


    //Servo liikkuu edestakaisin
    // scan from 0 to 180 degrees
    for(angle = 0; angle < 200; angle++) {                                  
      servo.write(angle);               
      delay(7);                   
    } 
     // now scan back from 180 to 0 degrees
    for(angle = 200; angle > 0; angle--) {                                
      servo.write(angle);           
      delay(2);       
  } 
    

    unsigned long currentMillisInfo = millis();

    if (currentMillisInfo - previousMillisInfo >= intervalInfo) {
      previousMillisInfo = currentMillisInfo;

      Serial.println("Board Information:");
      // print your board's IP address:
      IPAddress ip = WiFi.localIP();
      Serial.print("IP Address: ");
      Serial.println(ip);

      // print your network's SSID:
      Serial.println();
      Serial.println("Network Information:");
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID());

      // print the received signal strength:
      long rssi = WiFi.RSSI();
      Serial.print("signal strength (RSSI):");
      Serial.println(rssi);
      Serial.println("---------------------------------------");
  }

    unsigned long currentMillisLED = millis();

    // measure the signal strength and convert it into a time interval
    int intervalLED = WiFi.RSSI() * -10;

    // check if the time after the last blink is bigger the interval 
    if (currentMillisLED - previousMillisLED >= intervalLED) {
      previousMillisLED = currentMillisLED;

      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
    } else {
        ledState = LOW;
    }

      // set the LED with the ledState of the variable:
      digitalWrite(LED_BUILTIN, ledState);
  }
  
    if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
      return;
  } else {
    if ((t >= 20) || (t <= 24)) {
      //tyhjennä ruutu
      display.clearDisplay();
      //aseta teksti paikkaan
      display.setCursor(5,5);
      display.print("That's good!");
      display.setCursor(5,15);
      display.print("temp is " + String(t));

      //näytä näytöllä
      display.display();
    }     
    else if (t > 24) {
      
      //tyhjennä ruutu
      display.clearDisplay();
      //aseta teksti paikkaan
      display.setCursor(5,5);
      display.print("Too hot!");
      display.setCursor(5,15);
      display.print("temp is " + String(t));

      //näytä näytöllä
      display.display();
     }
     else {
            //tyhjennä ruutu
      display.clearDisplay();
      //aseta teksti paikkaan
      display.setCursor(5,5);
      display.print("Too cold!");
      display.setCursor(5,15);
      display.print("temp is " + String(t));

      //näytä näytöllä
      display.display();
     }
      Serial.print("Humidity: "); 
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: "); 
      Serial.print(t);
      Serial.println(" *C");
      delay(1000);
  }
 
 }
