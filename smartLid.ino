#include <SPI.h>
#include <MFRC522.h>
#include "pitches.h"

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above
#define NOTE_D6  1175
#define NOTE_D7  2349

const int pinLED = 33;
const int pinTrigger = 34;
const int pinEcho = 36;

long duration = 0;

MFRC522 mfrc522(

SS_PIN, RST_PIN);  // Create MFRC522 instance
int melody[] = {NOTE_D6, NOTE_D7};
int noteDurations[] = {4, 8};

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    delay(4);

    //Echo sensor
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinEcho, INPUT);
    pinMode(pinLED, OUTPUT);

}

void alert() {
    // iterate over the notes of the melody:

    for (int thisNote = 0; thisNote < 2; thisNote++) {

        // to calculate the note duration, take one second divided by the note type.

        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

        int noteDuration = 750 / noteDurations[thisNote];

        tone(11, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.

        // the note's duration + 30% seems to work well:

        int pauseBetweenNotes = noteDuration * 1.30;

        delay(pauseBetweenNotes);

        // stop the tone playing:

        noTone(11);

    }
}


long measureDistance() {
    digitalWrite(pinTrigger, LOW);
    delayMicroseconds(1000);
    noInterrupts();
    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(1000);
    digitalWrite(pinTrigger, LOW);
    duration = pulseIn(pinEcho, HIGH);
    interrupts();

    long durationOneWay = duration / 2;
    long distance = durationOneWay * 0.03435;
    return distance;
}


void loop() {

    long dis = measureDistance();
    Serial.println(dis);

    if (dis < 17) {
        // Turn on LED
        digitalWrite(pinLED, HIGH);


        while (true) {

            if (mfrc522.PICC_IsNewCardPresent()) {
                if (mfrc522.PICC_ReadCardSerial()) {
                    alert();
                    mfrc522.PICC_HaltA();
                    // Turn off LED
                    digitalWrite(pinLED, LOW);
                    break;
                }

            }

        }


    }


}
