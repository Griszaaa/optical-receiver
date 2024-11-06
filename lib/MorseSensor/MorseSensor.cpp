#include "MorseSensor.h"

// Konstruktor inicjujący pin fototranzysotra i próg wyzwalania stanu wysokiego
MorseSensor::MorseSensor(int pin, int thresh, unsigned int dotLength) 
    : sensorPin(pin), threshold(thresh), isReceiving(false), signalStartTime(0),
      signalDuration(0), signalEndTime(0), _dotLength(dotLength), newMessage(false), 
      morseSymbol(""), decodedMessage("") {}

// Inicjalizacja
void MorseSensor::begin() {
    pinMode(sensorPin, INPUT);
}

// Aktualizacja odbioru sygnałów
void MorseSensor::update() {
    int sensorValue = analogRead(sensorPin);
    bool currentState = sensorValue > threshold;
    unsigned long currentTime = millis();

    if (currentState) {
        // Gdy wykrywamy sygnał (kropka lub kreska)
        if (!isReceiving) {
            // Rozpoczynamy odbiór sygnału
            isReceiving = true;
            signalStartTime = currentTime;
        }
    } else {
        // Gdy nie ma sygnału, przetwarzamy odebrany symbol
        if (isReceiving) {
            isReceiving = false;
            signalDuration = currentTime - signalStartTime;

            // Rozpoznanie kropki lub kreski na podstawie czasu trwania sygnału
            if (signalDuration <= _dotLength) {
                morseSymbol += ".";  // Dodajemy kropkę
            } else if (signalDuration <= 3 * _dotLength) {
                morseSymbol += "-";  // Dodajemy kreskę
            }

            // Ustawiamy czas zakończenia sygnału
            signalEndTime = currentTime;
        } else if ( (currentTime - signalEndTime >= 3 * _dotLength) && (!morseSymbol.isEmpty()) ) {
            // W przypadku długiej przerwy przetwarzamy kompletny symbol Morse'a
            char decodedChar = decodeMorseSymbol(morseSymbol);
            if (decodedChar != ' ') {
                decodedMessage += decodedChar; // Dodajemy symbol do wiadomości
            }
            morseSymbol = ""; // Reset symbolu po jego przetworzeniu
        } else if (currentTime - signalEndTime >= 7 * _dotLength && !decodedMessage.isEmpty()) {
            // Długa przerwa oznacza koniec wiadomości
            newMessage = true; // Flaga, że mamy nową wiadomość
        }
    }
}


// Pobranie zdekodowanej wiadomości
String MorseSensor::getMessage() {
    newMessage = false;
    String message = decodedMessage;
    decodedMessage = "";
    return message;
}

// Sprawdź czy jest nowa wiadomość
bool MorseSensor::hasNewMessage() {
    return newMessage;
}

// Dekodowanie symbolu Morse'a
char MorseSensor::decodeMorseSymbol(String symbol) {
    if (symbol == ".-")     return 'A';
    if (symbol == "-...")   return 'B';
    if (symbol == "-.-.")   return 'C';
    if (symbol == "-..")    return 'D';
    if (symbol == ".")      return 'E';
    if (symbol == "..-.")   return 'F';
    if (symbol == "--.")    return 'G';
    if (symbol == "....")   return 'H';
    if (symbol == "..")     return 'I';
    if (symbol == ".---")   return 'J';
    if (symbol == "-.-")    return 'K';
    if (symbol == ".-..")   return 'L';
    if (symbol == "--")     return 'M';
    if (symbol == "-.")     return 'N';
    if (symbol == "---")    return 'O';
    if (symbol == ".--.")   return 'P';
    if (symbol == "--.-")   return 'Q';
    if (symbol == ".-.")    return 'R';
    if (symbol == "...")    return 'S';
    if (symbol == "-")      return 'T';
    if (symbol == "..-")    return 'U';
    if (symbol == "...-")   return 'V';
    if (symbol == ".--")    return 'W';
    if (symbol == "-..-")   return 'X';
    if (symbol == "-.--")   return 'Y';
    if (symbol == "--..")   return 'Z';
    if (symbol == "-----")  return '0';
    if (symbol == ".----")  return '1';
    if (symbol == "..---")  return '2';
    if (symbol == "...--")  return '3';
    if (symbol == "....-")  return '4';
    if (symbol == ".....")  return '5';
    if (symbol == "-....")  return '6';
    if (symbol == "--...")  return '7';
    if (symbol == "---..")  return '8';
    if (symbol == "----.")  return '9';
    return ' '; // Zwróć spację w przypadku nieznanego symbolu
}
