#include <nrfx.h> 
#include <nrf5340_application.h> 
#include <nrfx_config.h> 
#include <nrf.h> 
#include <nrfx_uarte.h> 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char uarte_buffer;
char bufferCharacter;
int i = 0;

// Skapar en driver instance för UARTE: 1.3
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

void init_stuff
{
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    nrfx_uarte_init(&instance, &config, NULL);
    nrfx_systick_init();
    init_gpio();
}

// en funktion för att skicka strängen data av storlek length, över UARTE 1.1
void uarte_write(char *data, int length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
}

// Den här funktionen omvandlar en textsträng till ett heltal 1.2
int read_int(char *inputStringNumber)
{
    int number = atoi(inputStringNumber);
    return number;
}

// läser in en textsträng från UARTE och sparar den i en teckenarray 1.2
void read_string(char *inputString)
{
    char bufferCharacter;
    int i = 0;

    while (i < MAX_LENGTH - 1)
    {
        nrfx_uarte_rx(&instance, &bufferCharacter, 1);
        if (bufferCharacter == '\r' || bufferCharacter == 13)
        {
            break;
        }

        inputString[i] = bufferCharacter;
        i++;
    }
    inputString[i] = '\0';
}

// en funktion för att skicka strängen data av storlek length, över UARTE 1.2
void uarte_write(char *data, int length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
}

// skickar ett heltal via UARTE genom att först omvandla talet till en textsträng. 1.3
void send_int(int value)
{
    char buffer[MAX_LENGTH];
    sprintf(buffer, "%d", value);
    int length = strlen(buffer);
    uarte_write(buffer, length);
}

// Vänta på knapptryck
int wait_for_button_press(void)
{
    while (1) // kollar så att knapp har blivit intryckt, om ja --> returnar värde.
    {
        if (nrf_gpio_pin_read(BUTTON1) == 0)
            return 1;
        if (nrf_gpio_pin_read(BUTTON2) == 0)
            return 2;
        if (nrf_gpio_pin_read(BUTTON3) == 0)
            return 3;
        if (nrf_gpio_pin_read(BUTTON4) == 0)
            return 4;
    }
}

void button_and_led_stuff
{
    // GPIO LEDs, Från uppgfit 2
    nrf_gpio_cfg_output(LED1); // Alla fyra LED-pinnar kan syras av programmet som utgångar.
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);

    nrf_gpio_pin_write(LED1, LED_OFF); // Släcker alla LEDs direkt genom att sätta värdet till OFF,
    nrf_gpio_pin_write(LED2, LED_OFF);
    nrf_gpio_pin_write(LED3, LED_OFF);
    nrf_gpio_pin_write(LED4, LED_OFF);

    // GPIO buttons
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP); // BUTTON(X)  blir en utgång, och ska ha PULLUP aktiverat, dvs. Värde 1 när den inte är nertryckt, när tryckt --> Värde 0-
    nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);
}

void RTC
{
    /* Starta RTC */
    NRF_RTC1->PRESCALER = 0;
    NRF_RTC1->TASKS_START = 1;
}
