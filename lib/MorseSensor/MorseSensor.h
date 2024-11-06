#ifndef MORSE_SENSOR_H
#define MORSE_SENSOR_H

#include <Arduino.h>

class MorseSensor {
public:
    MorseSensor(int sensorPin, int threshold, unsigned int dotLength = 53);
    void begin(); // Inicjalizacja
    void update(); // Aktualizacja odbioru sygnałów
    String getMessage(); // Pobierz zdekodowaną wiadomość
    bool hasNewMessage(); // Sprawdź, czy jest nowa wiadomość

private:
    int sensorPin; // Pin, do którego podłączony jest fototranzystor
    int threshold; // Próg dla stanu wysokiego
    bool isReceiving;
    unsigned long signalStartTime;
    unsigned long signalDuration;
    unsigned long signalEndTime;
    unsigned int _dotLength;
    bool newMessage; // Flaga informująca o nowej wiadomości
    String morseSymbol; // Przechowuje aktualny symbol Morse'a
    String decodedMessage; // Ostateczna wiadomość

    char decodeMorseSymbol(String symbol); // Dekodowanie symbolu Morse'a
};

#endif
