
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//WIFI CONNECTION
//const char *SSID     = "WIFI-SSID"; Enter your Wi-Fi name 
//const char *PASSWORD = "PASSWORD" ; Enter your Wi-Fi password
//SQL SERVER
//char* const host = "sql.sample.com"; 
//int hostPort = 80;

int valTemp;
int valPulse;
int tempPin = A1;
int pulsePin = A2;
int buttonPin = 7;
int buttonState = 0;
int counter = 0;
bool displayStandby = false;
bool scanning = false;
String seconds;

void setup() {	
  pinMode(buttonPin, INPUT);
  
  //wifi.init(SSID, PASSWORD);
  
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("WELCOME TO ");
  lcd.setCursor(0, 2);
  lcd.print("HEALTH MONITORING DEVICE");
  delay(2000);
  lcd.clear();
  lcd.print("INITIALIZING");
  for (int i=0; i < 4; i++){
      lcd.print(".");
      delay(1000);
  } 
  lcd.clear();
  displayStandby = true;
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == 1){
    scanning = true;
    ScanData();
  }
  else{
 	scanning = false;
    StandbyMessage();
  }
}

void StandbyMessage(){
  if (displayStandby == true){
    lcd.print("PRESS THE");
    lcd.setCursor(0, 2);
    lcd.print("BUTTON TO SCAN");
    displayStandby = false;
  }
 
}

void ScanData(){
  lcd.clear();
  for (int i=15; i >= 0; i--){
    seconds = String(i);
    lcd.print("PLEASE HOLD.." + seconds);
    lcd.setCursor(0, 2);
    lcd.print("TMP: ");
    valTemp = analogRead(tempPin);
    float mv = ( valTemp/1024.0)*5000; 
    float cel = mv/10;
    valTemp = cel;
    lcd.print(valTemp);
    lcd.print(" CELSIUS");
    lcd.print("          ");
    delay(1000);
    lcd.clear();
  } 
  for (int i=15; i >= 0; i--){
    seconds = String(i);
    lcd.print("PLEASE HOLD.." + seconds);
    lcd.setCursor(0, 2);
    lcd.print("BPM: ");
    valPulse = analogRead(pulsePin);
    valPulse = valPulse/6;
    lcd.print(valPulse);
    lcd.print(" pulse");
    lcd.print("          ");
    delay(1000);
    lcd.clear();
  } 
  lcd.print("SCAN SUCCESSFUL");
  lcd.setCursor(0, 2);
  lcd.print("TMP:");
  lcd.print(valTemp);
  lcd.print("C  ");
  lcd.print("BPM:");
  lcd.print(valPulse);
  delay(3000);
  lcd.clear();
  lcd.print("UPLOADING");
  
  //Send request for www.google.com at port 80
  //wifi.httpGet(host, hostPort);
  // get response buffer. Note that it is set to 250 bytes due to the Arduino low memory
  //char* wifiBuf = wifi.getBuffer();
  
  lcd.setCursor(0, 2);
  lcd.print("DATA");
  for (int i=5; i >= 0; i--){
    lcd.print(".");
    delay(1000);
  } 
  lcd.clear();
  lcd.print("THANK YOU FOR");
  lcd.setCursor(0, 2);
  lcd.print("USING");
  delay(3000);
}
 