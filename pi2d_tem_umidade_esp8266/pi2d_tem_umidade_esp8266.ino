#include <Ubidots.h>
#include <AHT10.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define UBIDOTS_TOKEN "BBFF-THEyBBLGfTxR4xaGrar3EraxqDNO6A"  // Put here your Ubidots TOKEN
#define DEVICE_LABEL "Monitor_TEMP_UMID"  // Put here your Ubidots device label

// variables
#define LED  "blink"
#define TEMP  "var_temp"
#define UMID  "var_umid"

char const * WIFI_SSID = "CLARO_2GC4D901"; // Put here your Wi-Fi SSID
char const * WIFI_PASS = "vaitomarnocu123"; // Put here your Wi-Fi password

Ubidots controle(UBIDOTS_TOKEN, UBI_HTTP);

uint8_t readStatus = 0;

AHT10 myAHT10(0X38);

bool led = 0;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

float Val_Temp = 0;
float Val_Umid = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Monitor Temperatura e Umidade");
  delay(1000);

  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Monitor de T e U");

  while (myAHT10.begin() != true) {
    Serial.println(F("AHT10 not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
    delay(5000);
  }

  Serial.println("AHT10 OK");

  Serial.println("Conectando na Rede");
  controle.wifiConnect(WIFI_SSID, WIFI_PASS);
}


void loop() {
  controle.add(LED, led);
  Val_Temp = myAHT10.readTemperature();
  Val_Umid = myAHT10.readHumidity();

  controle.add(TEMP, Val_Temp);
  controle.add(UMID, Val_Umid);

  lcd.setCursor(0, 0);
  lcd.print("TEMP     UMID   
  ");

  lcd.setCursor(0, 1);
  lcd.print(Val_Temp);
  lcd.setCursor(5, 1);
  lcd.print("C");
  
  lcd.setCursor(9, 1);
  lcd.print(Val_Umid);
  lcd.setCursor(15, 1);
  lcd.print("%");

  controle.send(DEVICE_LABEL);

  delay(2000);
  led = !led;
}
