#ifndef STUFF_H
#define STUFF_H

#define PIN_TXD 20
#define PIN_RXD 22

#define MAX_LENGTH 50
#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31

#define BUTTON1 23

#define LED_ON 0
#define LED_OFF 1

void init_stuff();
void read_string(char *str);
int read_int();
void send_int(int value);
void uarte_write(char *data, int length);
#endif