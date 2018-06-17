
#include <Wire.h>

#include "Adafruit_FONA.h"

#include <Wire.h>

const int buzzer = A1;

//Include LCD library
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 3, 2, 1, 0);

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI 7

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

// Variable to store text message
String textMessage;

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

// Create a variable to store Lamp state
String lampState = "HIGH";

// light connected to pin 6
#define rgbblue 5
#define rgbred 10
#define rgbgreen 6

int brightness = 255;

void setup() {

  // set up the LCD's number of columns and rows: 
  lcd.begin(20,4);

  pinMode(rgbblue, OUTPUT);
  pinMode(rgbred, OUTPUT);
  pinMode(rgbgreen, OUTPUT);

  digitalWrite(rgbblue, HIGH);
  digitalWrite(rgbred, HIGH);
  digitalWrite(rgbgreen, HIGH);

  brightness = 100;
  
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  
  while (!Serial);

  Serial.begin(115200);

  // Give time to your GSM shield log on to network
  delay(2000);
 
  Serial.println(F("FONA SMS caller ID test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  // make it slow so its easy to read!
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while(1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); Serial.println(imei);
  }

  fonaSerial->print("AT+CNMI=2,1\r\n");  //set up the FONA to send a +CMTI notification when an SMS is received

  Serial.println("FONA Ready");
  
  lcd.setCursor(0,0);
  lcd.print("- Aafje  Thuiszorg -");
  lcd.setCursor(2,1);
  lcd.print("De zorgverlener");
  lcd.setCursor(6,2);
  lcd.print("stuurt u");
  lcd.setCursor(2,3);
  lcd.print("snel een bericht");


char fonaNotificationBuffer[64];          //for notifications from the FONA
char smsBuffer[250];


}

void loop(){

 
  
  if(fona.available()>0){
    textMessage = fona.readString();
    Serial.print(textMessage);    
    delay(10);
  }
  
  if(textMessage.indexOf("Route")>=0){
    // Turn on light and save current state
    digitalWrite(rgbgreen, LOW);
    digitalWrite(rgbblue, HIGH);
    digitalWrite(rgbred, HIGH);
    lampState = "on";
    Serial.println("Bleu light set to ON");
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    lcd.begin(20, 4);
    lcd.setCursor(0, 0);
    lcd.print("U staat op de route!");
    lcd.setCursor(0, 1);
    lcd.print("Uw zorgverlener laat");
    lcd.setCursor(2, 2);
    lcd.print("het weten wanneer");
    lcd.setCursor(0, 3);
    lcd.print("het nog 1 uur duurt.");
    textMessage = "";  
  }
  if(textMessage.indexOf("Eenuur")>=0){
    // Turn on light and save current state
    digitalWrite(rgbgreen, LOW);
    digitalWrite(rgbblue, LOW);
    digitalWrite(rgbred, LOW);
    lampState = "on";
    Serial.println("Bleu, red and green light set to ON");
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    lcd.begin(20, 4);
    lcd.setCursor(2, 0);
    lcd.print("De zorg is binnen");
    lcd.setCursor(4, 1);
    lcd.print("1 uur bij u!");
    lcd.setCursor(0, 2);
    lcd.print("U hoort het wanneer");
    lcd.setCursor(2, 3);
    lcd.print("deze onderweg is!");
    textMessage = "";  
    }
  if(textMessage.indexOf("Vijftienmin")>=0){
    // Turn on light and save current state
    digitalWrite(rgbblue, HIGH);
    digitalWrite(rgbgreen, LOW);
    digitalWrite(rgbred, HIGH);
    lampState = "on";
    Serial.println("Green light set to ON");
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    lcd.begin(20, 4);
    lcd.setCursor(0, 0);
    lcd.print("De zorg is binnen 15");
    lcd.setCursor(3, 1);
    lcd.print("minuten bij u!");
    lcd.setCursor(0, 2);
    lcd.print("Het is erg fijn als");
    lcd.setCursor(1, 3);
    lcd.print("u ons binnen laat.");
    textMessage = "";  
  }
  if(textMessage.indexOf("Vertraging")>=0){
    // Turn off light and save current state
    digitalWrite(rgbblue, HIGH);
    digitalWrite(rgbred, LOW);
    digitalWrite(rgbgreen, HIGH);
    lampState = "on"; 
    Serial.println("Red light set to OFF");
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    lcd.begin(20,4);
    lcd.setCursor(1, 0);
    lcd.print("Er is vertraging...");
    lcd.setCursor(3, 1);
    lcd.print("Onze Excuses!");
    lcd.setCursor(1, 2);
    lcd.print("Wij bellen u zodra");
    lcd.setCursor(6, 3);
    lcd.print("dit kan!");
    textMessage = ""; 
  }

  if(textMessage.indexOf("Klaar")>=0){
    // Turn off light and save current state
    digitalWrite(rgbblue, HIGH);
    digitalWrite(rgbgreen, HIGH);
    digitalWrite(rgbred, HIGH);
    lampState = "off"; 
    Serial.println("All lights set to OFF");
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    lcd.begin(20, 4);
    lcd.setCursor(0, 0);
    lcd.print("Uw zorg is afgerond!");
    lcd.setCursor(1, 1);
    lcd.print("Was dit naar wens?");
    lcd.setCursor(1, 2);
    lcd.print("U kunt ons bereiken");
    lcd.setCursor(3, 3);
    lcd.print("op 06-46642500");
    textMessage = "";
    delay(120000);
    lcd.begin(20, 4);
    lcd.setCursor(5, 0);
    lcd.print("Tot ziens!");
    lcd.setCursor(5, 2);
    lcd.print("06-46642500");
    delay(5000);
    lcd.begin(20, 4);
  }


  //Check state of any light
    if(textMessage.indexOf("State")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
  
}  

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  fona.print("AT+CMGF=1\r"); 
  delay(100);
  fona.println("AT + CMGS = \"0031640595779\""); 
  delay(100);
  // Send the SMS
  fona.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  fona.println((char)26); 
  delay(100);
  fona.println();
  // Give module time to send SMS
  delay(5000);  

}

