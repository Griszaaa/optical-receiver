#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MorseSensor.h>

// ***** KONFIGURACJA SIECI WIFI *****
const char* ssid = "Odbiornik";  // Nazwa sieci WiFi
const char* password = "12345678";  // Hasło WiFi
IPAddress apIP(192, 168, 4, 2); // Ustawienie innego niż domyślne IP
WiFiServer server(20);  // Używamy serwera TCP na porcie 20

// ***** KONFIGURACJA CZUJNIKA MORSE'A *****
#define sensorPin A0
#define threshold 450
MorseSensor morseSensor(sensorPin, threshold);

// ***** KONFIGURACJA LCD *****
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ***** ZMIENNE DO PRZEWIJANIA LCD *****
String messageToScroll = ""; // Przechowuje wiadomość do przewijania
unsigned int scrollIndex = 0; // Aktualny indeks przewijania
unsigned long previousMillis = 0; // Czas ostatniego przewinięcia
const long scrollInterval = 450; // Interwał przewijania (450 ms)

void setup() { 
    // Konfiguracja Wi-Fi
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    server.begin();

    // Serial.begin(9600);

    // Inicjalizacja LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.clear();
    // Wypisanie IP i portu na LCD
    lcd.print("IP: ");
    lcd.print(WiFi.softAPIP().toString().substring(0, 11));
    
    lcd.setCursor(0, 1);
    lcd.print("Port: ");
    lcd.print(server.port());

    // Inicjalizacja czujnika Morse'a
    morseSensor.begin();
}

void scrollText() {
    unsigned long currentMillis = millis(); // Aktualny czas
    lcd.setCursor(0, 0);
    lcd.print("Message:");

    if(messageToScroll.length() <= 16) {
        lcd.setCursor(0, 1);
        lcd.print(messageToScroll); // Jeżeli tekst krótszy niż 16 znaków - wyświetl bez przewijania
    } 
    else if(currentMillis - previousMillis >= scrollInterval) { 
        previousMillis = currentMillis; // Zapisz czas przewinięcia
    
        lcd.setCursor(0, 1);
        // Wyświetlanie odpowiedniego fragmentu wiadomości
        lcd.print(messageToScroll.substring(scrollIndex, scrollIndex + 16)); 
    
        scrollIndex++; // Przesuwanie indeksu
        if(scrollIndex + 16 > messageToScroll.length()) {
        scrollIndex = 0; // Wracamy na początek po dotarciu do końca
    }
  }
}

void loop() {
    WiFiClient client = server.accept();
    if (client) {
        lcd.clear();
        messageToScroll = "Connected";
        scrollText();

        while (client.connected()) {
            if (client.available()) {
                morseSensor.update();
                // Sprawdzanie, czy jest nowa wiadomość
                if (morseSensor.hasNewMessage()) {
                    lcd.clear();
                    messageToScroll = morseSensor.getMessage();
                    client.println(messageToScroll);
                    // Serial.println(messageToScroll);
                    scrollIndex = 0;
                    scrollText();
                }
                
            }
            scrollText();
            yield();
        }
        client.stop();
        messageToScroll = "Disconnected";
        scrollText();
    }
}
