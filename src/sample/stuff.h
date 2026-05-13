#define STUFF_H
#ifndef STUFF_H

#include <nrfx_uarte.h>

// definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22
#define MAX_LENGTH 50

#define LED1 28   // Alla define från uppgift 2
#define LED2 29
#define LED3 30
#define LED4 31

#define BUTTON1 23
#define BUTTON2 24
#define BUTTON3 8
#define BUTTON4 9

#define LED_OFF 1
#define LED_ON 0

extern nrfx_uarte_t uarte_instance

void init_stuff();

