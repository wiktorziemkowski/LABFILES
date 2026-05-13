#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31

#define BUTTON1 23

#define LED_OFF 1
#define LED_ON 0

#define PIN_TXD 20
#define PIN_RXD 22

#define MAX_LENGTH 50

nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

/* Delayfunktion */
void delay_ms(uint32_t ms)
{
    nrfx_systick_delay_ms(ms);
}

/* UART write */
void uarte_write(char *data, int length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
}

/* Läs in sträng från UART */
void read_string(char *inputString)
{
    char bufferCharacter;
    int i = 0;

    while (i < MAX_LENGTH - 1)
    {
        nrfx_uarte_rx(&instance, &bufferCharacter, 1);

        /* Enter avslutar */
        if (bufferCharacter == '\r' || bufferCharacter == 13)
        {
            break;
        }

        inputString[i] = bufferCharacter;
        i++;
    }

    inputString[i] = '\0';
}

/* Konvertera sträng till int */
int read_int(char *inputString)
{
    return atoi(inputString);
}

/* Vänta på knapptryck */
int wait_for_button_press(void)
{
    while (1)
    {
        if (nrf_gpio_pin_read(BUTTON1) == 0)
        {
            return 1;
        }
    }
}

int main(void)
{
    /* UART config */
    const nrfx_uarte_config_t config =
        NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

    nrfx_uarte_init(&instance, &config, NULL);

    /* Initiera systick */
    nrfx_systick_init();

    /* LEDs som output */
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);

    /* Släck LEDs */
    nrf_gpio_pin_write(LED1, LED_OFF);
    nrf_gpio_pin_write(LED2, LED_OFF);
    nrf_gpio_pin_write(LED3, LED_OFF);
    nrf_gpio_pin_write(LED4, LED_OFF);

    /* Button som input */
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);

    while (1)
    {
        char startMsg[] =
        "\n\rTryck på knappen för att starta slumpgeneratorn\n\r";

        uarte_write(startMsg, strlen(startMsg));

        /* Vänta på knapp */
        wait_for_button_press();

        /* Starta RTC */
        NRF_RTC1->PRESCALER = 0;
        NRF_RTC1->TASKS_START = 1;

        /* Seed till slumpgenerator */
        uint32_t seed = NRF_RTC1->COUNTER;

        srand(seed);

        /* Läs amount */
        char msg1[] = "\n\rMata in amount:\n\r";

        uarte_write(msg1, strlen(msg1));

        char amountString[MAX_LENGTH];

        read_string(amountString);

        int amount = read_int(amountString);

        /* Läs delay */
        char msg2[] = "\n\rMata in delay i ms:\n\r";

        uarte_write(msg2, strlen(msg2));

        char delayString[MAX_LENGTH];

        read_string(delayString);

        int delay = read_int(delayString);

        /* Blink-loop */
        for (int i = 0; i < amount; i++)
        {
            int randomLed = rand() % 4;

            /* Släck alla LEDs */
            nrf_gpio_pin_write(LED1, LED_OFF);
            nrf_gpio_pin_write(LED2, LED_OFF);
            nrf_gpio_pin_write(LED3, LED_OFF);
            nrf_gpio_pin_write(LED4, LED_OFF);

            /* Tänd slumpmässig LED */
            if (randomLed == 0)
            {
                nrf_gpio_pin_write(LED1, LED_ON);
            }

            if (randomLed == 1)
            {
                nrf_gpio_pin_write(LED2, LED_ON);
            }

            if (randomLed == 2)
            {
                nrf_gpio_pin_write(LED3, LED_ON);
            }

            if (randomLed == 3)
            {
                nrf_gpio_pin_write(LED4, LED_ON);
            }

            /* Vänta */
            delay_ms(delay);

            /* Släck LEDs */
            nrf_gpio_pin_write(LED1, LED_OFF);
            nrf_gpio_pin_write(LED2, LED_OFF);
            nrf_gpio_pin_write(LED3, LED_OFF);
            nrf_gpio_pin_write(LED4, LED_OFF);
        }

        /* Beräkna produkt */
        int product = amount * delay;

        char result[100];

        sprintf(result,
        "\n\rProdukten av amount och delay är: %d ms\n\r",
        product);

        uarte_write(result, strlen(result));
    }
}

