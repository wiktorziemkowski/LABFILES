#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <stdlib.h>   // TILLÄGG: behövs för rand/srand
#include <stdio.h>    // TILLÄGG: för sprintf
#include <nrfx_uarte.h>

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

#define PIN_TXD 20
#define PIN_RXD 22
#define MAX_LENGTH 50

nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

void uarte_write(char *data, int length) // skickar data till UART-Driver --> skickar tillbaka till pc
{
    nrfx_uarte_tx(&instance, data, length, 0);
}


int wait_for_button_press(void)
{
    while (1)  // kollar så att knapp har blivit intryckt, om ja --> returnar värde.
    {
        if (nrf_gpio_pin_read(BUTTON1) == 0) return 1;
        if (nrf_gpio_pin_read(BUTTON2) == 0) return 2;
        if (nrf_gpio_pin_read(BUTTON3) == 0) return 3; 
        if (nrf_gpio_pin_read(BUTTON4) == 0) return 4; 
    }
}


int main(void)
{
    // UART init
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    nrfx_uarte_init(&instance, &config, NULL);

    // GPIO LEDs, Från uppgfit 2
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);

    nrf_gpio_pin_write(LED1, LED_OFF);
    nrf_gpio_pin_write(LED2, LED_OFF);
    nrf_gpio_pin_write(LED3, LED_OFF);
    nrf_gpio_pin_write(LED4, LED_OFF);

    // GPIO buttons
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP); 
    nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

    // RTC Setup
    
    while (1)
    {
        (*NRF_RTC1).PRESCALER = 0; //sätter prescalen för RTC1 till 0, kör med ~33 Hz.
        (*NRF_RTC1).TASKS_START = 1; // startar RTC1 klockan.

    char msg[] = "\n\rTryck på en knapp för att starta slumpgenerator...\n\r";
    uarte_write(msg, sizeof(msg));

    // Vänta på knapptryckning
    int button = wait_for_button_press();

   
    uint32_t seed = NRF_RTC1->COUNTER;  //Seed generator

    
    srand(seed);  //Randomiserar seeden

    char msg2[100];
    sprintf(msg2, "\n\rSeed = %lu\n\r", seed);
    uarte_write(msg2, strlen(msg2));

    // Generear Random tal
    for (int i = 0; i < 5; i++)  // Bestämmer hur många Slupmtal vi får.
    {
        int r = rand() % 1000;    // Bestämmer hur stora slumptalen blir (nu är det 1-1000).

        char buf[50];
        sprintf(buf, "Slumptal %d: %d\n\r", i + 1, r); // tar int --> skriver om till string
        uarte_write(buf, strlen(buf));
    }

    }
}