/*------------------------------------------------------/
/ Copyright (c) 2023, Elehobica
/ Released under the BSD-2-Clause
/ refer to https://opensource.org/licenses/BSD-2-Clause
/------------------------------------------------------*/

#include <stdio.h>
#include <cstring>

#include "pico/stdlib.h"

#include "Buttons.h"

uint32_t scan_cnt = 0;
uint32_t t = 0;

static constexpr uint PIN_LED           = PICO_DEFAULT_LED_PIN;
static constexpr uint PIN_UP_BUTTON     = 18;
static constexpr uint PIN_DOWN_BUTTON   = 19;
static constexpr uint PIN_LEFT_BUTTON   = 20;
static constexpr uint PIN_RIGHT_BUTTON  = 21;
static constexpr uint PIN_CENTER_BUTTON = 22;
static constexpr uint PIN_SET_BUTTON    = 26;
static constexpr uint PIN_RESET_BUTTON  = 27;

// ADC Timer & frequency
static repeating_timer_t timer;
static constexpr int INTERVAL_BUTTONS_CHECK_MS = 50;

typedef enum _button_id_t {
    ID_UP_BUTTON = 0,
    ID_DOWN_BUTTON,
    ID_LEFT_BUTTON,
    ID_RIGHT_BUTTON,
    ID_CENTER_BUTTON,
    ID_SET_BUTTON,
    ID_RESET_BUTTON
} button_id_t;

static button_t btns_5way_tactile_plus2[] = {
    {ID_UP_BUTTON,     "up",     PIN_UP_BUTTON,     &Buttons::DEFAULT_BUTTON_SINGLE_REPEAT_CONFIG},
    {ID_DOWN_BUTTON,   "down",   PIN_DOWN_BUTTON,   &Buttons::DEFAULT_BUTTON_SINGLE_REPEAT_CONFIG},
    {ID_LEFT_BUTTON,   "left",   PIN_LEFT_BUTTON,   &Buttons::DEFAULT_BUTTON_SINGLE_REPEAT_CONFIG},
    {ID_RIGHT_BUTTON,  "right",  PIN_RIGHT_BUTTON,  &Buttons::DEFAULT_BUTTON_SINGLE_REPEAT_CONFIG},
    {ID_CENTER_BUTTON, "center", PIN_CENTER_BUTTON, &Buttons::DEFAULT_BUTTON_MULTI_CONFIG},
    {ID_SET_BUTTON,    "set",    PIN_SET_BUTTON,    &Buttons::DEFAULT_BUTTON_SINGLE_CONFIG},
    {ID_RESET_BUTTON,  "reset",  PIN_RESET_BUTTON,  &Buttons::DEFAULT_SWITCH_CONFIG}
};

Buttons* buttons = nullptr;

static inline uint64_t _micros(void)
{
    return to_us_since_boot(get_absolute_time());
}

static bool scan_buttons(repeating_timer_t *rt) {
    gpio_put(PIN_LED, !gpio_get(PIN_LED));
    if (buttons != nullptr) {
        uint64_t t0 = _micros();
        buttons->scan_periodic();
        t = (uint32_t) (_micros() - t0);
    }
    scan_cnt++;
    return true; // keep repeating
}

int main()
{
    stdio_init_all();

    // GPIO settings
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_put(PIN_LED, 0);

    for (int i = 0; i < sizeof(btns_5way_tactile_plus2) / sizeof(button_t); i++) {
        button_t* button = &btns_5way_tactile_plus2[i];
        gpio_init(button->pin);
        gpio_set_dir(button->pin, GPIO_IN);
        gpio_pull_up(button->pin);
    }

    buttons = new Buttons(btns_5way_tactile_plus2, sizeof(btns_5way_tactile_plus2) / sizeof(button_t));

    // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_us(-INTERVAL_BUTTONS_CHECK_MS * 1000, scan_buttons, nullptr, &timer)) {
        printf("Failed to add timer\n");
        return 0;
    }

    printf("==================\r\n");
    printf("== pico_buttons ==\r\n");
    printf("==================\r\n");

    while (true) {
        button_event_t event;
        if (buttons->get_button_event(event)) {
            switch (event.type) {
            case EVT_SINGLE:
                if (event.repeat_count > 0) {
                    printf("%s(%d): 1 (Repeated %d)\r\n", event.button_name, event.button_id, event.repeat_count);
                } else {
                    printf("%s(%d): 1\r\n", event.button_name, event.button_id);
                }
                break;
            case EVT_MULTI:
                printf("%s(%d): %d\r\n", event.button_name, event.button_id, event.click_count);
                if (strncmp(event.button_name, "center", 6) == 0 && event.click_count == 3) {
                    printf("time %dus (scan: %d)\r\n", t, scan_cnt);
                }
                break;
            case EVT_LONG:
                printf("%s(%d): Long\r\n", event.button_name, event.button_id);
                break;
            case EVT_LONG_LONG:
                printf("%s(%d): LongLong\r\n", event.button_name, event.button_id);
                break;
            case EVT_ON:
                printf("%s(%d): switch on\r\n", event.button_name, event.button_id);
                break;
            case EVT_OFF:
                printf("%s(%d): switch off\r\n", event.button_name, event.button_id);
                break;
            default:
                break;
            }
        }
    }

    return 0;
}
