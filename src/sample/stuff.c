#include "stuff.h"
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <nrfx_rtc.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern nrfx_uarte_t uarte_instance;
extern nrfx_rtc_t rtc_instance;

void init_stuff()
{
    nrfx_rtc_config_t rtc_config = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t errr = nrfx_rtc_init(&rtc_instance, &rtc_config, NULL);
    nrfx_rtc_enable(&rtc_instance);
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
    nrfx_systick_init();
    const nrfx_uarte_config_t uarte_config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    nrfx_uarte_init(&uarte_instance, &uarte_config, NULL);
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);
    nrf_gpio_pin_write(LED1, LED_OFF);
    nrf_gpio_pin_write(LED2, LED_OFF);
    nrf_gpio_pin_write(LED3, LED_OFF);
    nrf_gpio_pin_write(LED4, LED_OFF);
}

int read_int()
{
    char input[MAX_LENGTH];
    read_string(input);
    int number = atoi(input);
    return number;
}

void read_string(char *inputString)
{
    char bufferCharacter;
    int i = 0;

    while (i < MAX_LENGTH - 1)
    {
        nrfx_uarte_rx(&uarte_instance, &bufferCharacter, 1);

        if (bufferCharacter == '\r' || bufferCharacter == 13)
        {
            break;
        }

        inputString[i] = bufferCharacter;
        i++;
    }

    inputString[i] = '\0';
}

void uarte_write(char *data, int length)
{
    nrfx_uarte_tx(&uarte_instance, data, length, 0);
}

void send_int(int value)
{
    char buffer[MAX_LENGTH];
    sprintf(buffer, "%d", value);
    int length = strlen(buffer);
    uarte_write(buffer, length);
}
