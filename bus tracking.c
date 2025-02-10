#include <SPI.h> // Include the
RFID libraries
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <string.h>
#define SS_PIN 10 // Declare the
reset and SDA pins of RFID
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN,
RST_PIN); // Create MFRC522
instance
LiquidCrystal_I2C lcd(0x27, 16, 2);
// (I2C addr., rows, columns)
byte validUIDs[][4] = {
{0xA3, 0xA8, 0x7F, 0xFA}, // UID
of student 1 TAG
{0x9C, 0xD0, 0xB8, 0x03}, // UID
of student 2 TAG
{0xAC, 0x99, 0xE1, 0x03}, // UID
of student 3 TAG
};
const char* passNames[] = {
"CHIBUZE"
, // Name of student 1
"MIMO"
, // Name of student 2
"CHAKI"
, // Name of student 3
};
String currPass[sizeof(validUIDs)];
// Array to store currently
authorized UIDs
int bal[] = {100, 100, 50};
int dist[] = {1, 1, 1};
int rate = 10;
int total_dist = 0;
void setup() {
Serial.begin(9600); // Initiate SPI
bus
SPI.begin();
mfrc522.PCD_Init(); // Initiate
MFRC522
lcd.begin(16, 4); // Begin the
LCD
lcd.init();
lcd.backlight();
pinMode(A0, OUTPUT); //
Actuator buzzer pin
lcd.clear();
lcd.setCursor(5, 0);
lcd.print("WELCOME");
lcd.setCursor(7, 1);
lcd.print("TO");
delay(2500);
lcd.setCursor(0, 0);
lcd.print(" BUS TICKET ");
lcd.setCursor(0, 1);
lcd.print(" PAYMENT SYSTEM ");
delay(2500);
lcd.clear();
lcd.setCursor(2, 0);
lcd.print("Bus Ticketing");
Serial.print("Ready to read RFID
cards");
delay(2000);
lcd.setCursor(0, 0);
lcd.print("Bus Fare:$10 ");
lcd.print(rate);
lcd.setCursor(0, 1);
lcd.print("Dist. Covered:");
lcd.print(total_dist);
delay(2000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" SCAN TAG FOR");
lcd.setCursor(0, 1);
lcd.print(" BUS TICKET");
delay(1000);
}
void checkPass(String content) {
bool foundInBus = false;
for (int i = 0; i < sizeof(currPass);
i++) {
if (currPass[i] == content) { //
Check if UID is already authorized
(scanned once)
foundInBus = true;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("ThankYou ");
lcd.print(passNames[i]);
lcd.setCursor(0, 1);
lcd.print("Balance:$");
bal[i] = bal[i] - dist[i]*rate;
lcd.print(bal[i]); // Display
remaining balance
currPass[i] = ""; // Remove the
authorized UID from currPass (for
next scan)
tone(A0, 1000);
delay(1000);
noTone(A0);
delay(2000);
break;
}
else if(currPass[i+1] == content){
foundInBus = true;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("ThankYou ");
lcd.print(passNames[i+1]);
lcd.setCursor(0, 1);
lcd.print("Balance:$");
bal[i+1] = bal[i+1] -
dist[i+1]*rate;
lcd.print(bal[i+1]); // Display
remaining balance
currPass[i+1] = ""; // Remove
the authorized UID from currPass
(for next scan)
tone(A0, 1000);
delay(1000);
noTone(A0);
delay(2000);
break;
}
else if(currPass[i+2] == content){
foundInBus = true;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("ThankYou ");
lcd.print(passNames[i+2]);
lcd.setCursor(0, 1);
lcd.print("Balance:$");
bal[i+2] = bal[i+2] -
dist[i+2]*rate;
lcd.print(bal[i+2]); // Display
remaining balance
currPass[i+2] = ""; // Remove
the authorized UID from currPass
(for next scan)
tone(A0, 1000);
delay(1000);
noTone(A0);
delay(2000);
break;
}
if (!foundInBus){
for (int i = 0; i <
sizeof(validUIDs); i++) {
if
(memcmp(mfrc522.uid.uidByte,
validUIDs[i], mfrc522.uid.size) ==
0){
Serial.print(currPass[i]); //
Add the UID to currPass (for
subsequent scans)
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Welcome
Aboard");
lcd.setCursor(0, 1);
lcd.print(passNames[i]);
tone(A0, 1000);
delay(500);
noTone(A0); // Welcome
message with student name
delay(1000);
currPass[i] = content;
break;
}
else{
continue;
}
break;
}
}
break;
}
}
void unregistered() {
lcd.clear();
for (int i = 0; i < 3; i++) {
tone(A0, 1000);
lcd.print(" UNREGISTERED!");
delay(500);
noTone(A0);
lcd.clear();
delay(500);
}
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" UNREGISTERED!");
lcd.setCursor(0, 1);
lcd.print("PLS GET A VALID
CARD");
delay(700);
for (int posCounter = 0;
posCounter < 43; posCounter++) {
lcd.scrollDisplayLeft(); // Scroll
one position left
delay(200); // Wait a bit
}
}
void loop() {
SPI.begin(); // Initiate SPI bus
mfrc522.PCD_Init(); // Initiate
MFRC522
// Look for new cards
if
(mfrc522.PICC_IsNewCardPresent(
)){
if
(mfrc522.PICC_ReadCardSerial()) {
Serial.print("\nUID tag: \n");
for (byte i = 0; i <
mfrc522.uid.size; i++) {
mfrc522.uid.uidByte[i] =
toupper(mfrc522.uid.uidByte[i]);
Serial.print("0x");
if (mfrc522.uid.uidByte[i] <
0x10) Serial.print("0");
Serial.print(mfrc522.uid.uidByte[i]
, HEX);
if (i < mfrc522.uid.size - 1)
Serial.print("
,
");
}
String content = "";
for (byte i = 0; i <
mfrc522.uid.size; i++) {
content +=
String(mfrc522.uid.uidByte[i] <
0x10 ? "0" : " ") +
String(mfrc522.uid.uidByte[i],
HEX);
}
bool validUID = false;
bool foundInBus = false;
for (int i = 0; i <
sizeof(validUIDs); i++) {
if
(memcmp(mfrc522.uid.uidByte,
validUIDs[i], mfrc522.uid.size) ==
0) {
validUID = true;
break;
}
}
checkPass(content);
if (!validUID){
unregistered();
}
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Bus Fare:$10 ");
lcd.print(rate);
lcd.setCursor(0, 1);
lcd.print("Dist. Covered:");
total_dist += 10;
for (int i=0; i< sizeof(dist); i++){
dist[i] += 10;
}
lcd.print(total_dist);
delay(2000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" SCAN TAG FOR");
lcd.setCursor(0, 1);
lcd.print(" BUS TICKET");
delay(1000);
}
mfrc522.PICC_HaltA(); // Stop
reading
mfrc522.PCD_StopCrypto1();
}
}
