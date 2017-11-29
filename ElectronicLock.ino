#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LOCK_PIN 8

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
	Serial.begin(9600);
	SPI.begin();
	rfid.PCD_Init();

	pinMode(LOCK_PIN, OUTPUT);
	digitalWrite(LOCK_PIN, LOW);
}

void loop() {
	if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
		return;

	String strID = "";
	for (byte i = 0; i < 4; i++) {
		strID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) + (i!=3 ? ":" : "");
	}
	strID.toUpperCase();

	Serial.print("Key: ");
	Serial.println(strID);

	if (strID.indexOf("D0:E1:0B:18") >= 0) {
		Serial.println("true key, unlock");
		digitalWrite(LOCK_PIN, HIGH);
		delay(10000);
		Serial.println("time out, lock");
		digitalWrite(LOCK_PIN, LOW);
	} else {
		Serial.println("wrong key, lock");
	}
 
	rfid.PICC_HaltA();
	rfid.PCD_StopCrypto1();
}
