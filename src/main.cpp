#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ***** KONFIGURACJA SIECI WIFI *****
const char* ssid = "Odbiornik";  // Nazwa sieci WiFi
const char* password = "12345678";  // Hasło WiFi
IPAddress apIP(192, 168, 4, 2); // Ustawienie innego niż domyślne IP
WiFiServer server(20);  // Używamy serwera TCP na porcie 20

// ***** KONFIGURACJA LCD *****

LiquidCrystal_I2C lcd(0x27,16,2);

// ***** ZMIENNE DO PRZEWIJANIA LCD *****
String messageToScroll = "";
unsigned int scrollIndex = 0;
unsigned long previousMillis = 0;
const long scrollInterval = 450;

// ***** ZMIENNE DO DEKODOWANIA KODU MORSE'A *****
const int sensorPin = A0;  // Pin, do którego podłączony jest fototranzystor
const int threshold = 512; // Próg dla stanu wysokiego
bool isReceiving = false;
unsigned long signalStartTime = 0;
unsigned long signalDuration = 0;
String morseSymbol = ""; // Przechowuje aktualny symbol Morse'a
String decodedMessage = ""; // Ostateczna wiadomość

void setup()
{
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  server.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();
  
  lcd.print("IP: ");
  String ip = WiFi.softAPIP().toString();
  lcd.print(ip.substring(0, 11));
  
  lcd.setCursor(0, 1);
  lcd.print("Port: ");
  lcd.print(server.port());
}

void scrollText() 
{
  unsigned long currentMillis = millis();
  lcd.setCursor(0, 0);
  lcd.print("Message:");

  if (messageToScroll.length() <= 16) 
  {
    lcd.setCursor(0, 1);
    lcd.print(messageToScroll);
  } 
  else if (currentMillis - previousMillis >= scrollInterval) 
  { 
    previousMillis = currentMillis;
    
    lcd.setCursor(0, 1);
    lcd.print(messageToScroll.substring(scrollIndex, scrollIndex + 16));
    
    scrollIndex++;
    if (scrollIndex + 16 > messageToScroll.length()) 
    {
      scrollIndex = 0;
    }
  }
}

void loop()
{
  WiFiClient client = server.accept();
  if (client)
  {
    lcd.clear();
    messageToScroll = "Connected";
    scrollText();

    while (client.connected())
    {
      if(client.available())
      {

      }
      scrollText();
    }
    client.stop();
    messageToScroll = "Disconnected";
    scrollText();
  }
}
