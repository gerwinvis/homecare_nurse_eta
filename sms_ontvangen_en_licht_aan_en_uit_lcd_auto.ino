
// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>

//Include LCD library
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Configure software serial port
SoftwareSerial SIM900(7, 8);

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "LOW";

// Relay connected to pin 6
const int relayb = 10;
const int relayg = 6;
const int relayr = 9;

void setup() {

  // set up the LCD's number of columns and rows: 
  lcd.begin(20,4);


  // Set relay as OUTPUT
  pinMode(relayb, OUTPUT);
  pinMode(relayg, OUTPUT);
  pinMode(relayr, OUTPUT);

  // By default the relay is off
  digitalWrite(relayb, HIGH);
  delay(100);
  digitalWrite(relayb, LOW);
  delay(100);
  digitalWrite(relayg, HIGH);
  delay(100);
  digitalWrite(relayg, LOW);
  delay(100);
  digitalWrite(relayr, HIGH);
  delay(100);
  digitalWrite(relayr, LOW);
  
  // Initializing serial communication
  Serial.begin(19200); 
  SIM900.begin(19200);

  // Give time to your GSM shield log on to network
  delay(20000);
  
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100); 
  
  lcd.setCursor(0,0);
  lcd.print("- Aafje  Thuiszorg -");
  lcd.setCursor(2,1);
  lcd.print("De zorgverlener");
  lcd.setCursor(6,2);
  lcd.print("stuurt u");
  lcd.setCursor(2,3);
  lcd.print("snel een bericht");
}

void loop(){
  
  
  if(SIM900.available()>0){
    textMessage = SIM900.readString();
    Serial.print(textMessage);    
    delay(10);
  }
  
  //Blue led goes on and off
  if(textMessage.indexOf("Route")>=0){
    // Turn on relay and save current state
    digitalWrite(relayb, HIGH);
    digitalWrite(relayr, LOW);
    digitalWrite(relayg, LOW);
    lampState = "on";
    Serial.println("Bleu relay set to ON");
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
    // Turn on relay and save current state
    digitalWrite(relayb, HIGH);
    digitalWrite(relayg, HIGH);
    digitalWrite(relayr, HIGH);
    lampState = "on";
    Serial.println("Bleu, red and green relay set to ON");
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
    // Turn on relay and save current state
    digitalWrite(relayb, LOW);
    digitalWrite(relayg, HIGH);
    digitalWrite(relayr, LOW);
    lampState = "on";
    Serial.println("Green relay set to ON");
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
    // Turn off relay and save current state
    digitalWrite(relayb, LOW);
    digitalWrite(relayr, HIGH);
    digitalWrite(relayg, LOW);
    lampState = "on"; 
    Serial.println("Bleu relay set to OFF");
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
    // Turn off relay and save current state
    digitalWrite(relayb, LOW);
    digitalWrite(relayr, LOW);
    digitalWrite(relayg, LOW);
    lampState = "off"; 
    Serial.println("All relays set to OFF");
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
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"0031640595779\""); 
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}

