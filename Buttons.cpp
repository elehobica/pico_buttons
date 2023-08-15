/*------------------------------------------------------/
/ Copyright (c) 2023, Elehobica
/ Released under the BSD-2-Clause
/ refer to https://opensource.org/licenses/BSD-2-Clause
/------------------------------------------------------*/

#include "Buttons.h"

inline uint8_t trailing_zeros64(uint64_t u64)
{
    return (uint8_t) __builtin_ctzll(u64);
}

inline uint8_t ones_count64(uint64_t u64)
{
    return (uint8_t) __builtin_popcountll(u64);
}

Buttons::Buttons(button_t* buttons, int size, uint8_t scan_skip)
    : _buttons(buttons), _size(size), _scan_skip(scan_skip), _scan_count(0)
{
    // button event queue
    queue_init(&btn_evt_queue, sizeof(button_event_t), EVENT_QUEUE_LENGTH);
}

void Buttons::scan_periodic()
{
    if (_scan_count < _scan_skip) {
        _scan_count++;
        return;
    }
    for (int i = 0; i < _size; i++) {
        // what to get (default values)
        uint8_t repeat_cnt = 0;
        uint8_t count_rise = 0;
        bool detect_long = false;
        bool detect_long_long = false;
        // alias
        button_t* button = &_buttons[i];
        const button_config_t* config = button->config;
        // === get raw status of pin ===
        bool raw_sts = gpio_get(button->pin) == config->active_high;
        // === unshift history ===
        unshift_history(&button->history, raw_sts);
        uint8_t recent_stay_pushed_counts = get_recent_stay_pushed_counts(&button->history);
        uint8_t recent_stay_released_counts = get_recent_stay_released_counts(&button->history);
        // === Detect Repeated (by non-filtered) ===
        if (config->long_detect_cnt == 0 && config->long_long_detect_cnt == 0) {
            if ((_scan_count % (config->repeat_skip + 1)) == 0) {
                if (config->repeat_detect_cnt > 0 && recent_stay_pushed_counts >= config->repeat_detect_cnt) {
                    if (button->rpt_cnt < 255) {
                        button->rpt_cnt++;
                    }
                    repeat_cnt = button->rpt_cnt;
                } else {
                    button->rpt_cnt = 0;
                }
            }
        }
        // === Detect Long (by non-filtered) ===
        if (config->repeat_detect_cnt == 0) {
            if (recent_stay_pushed_counts > 0) {
                if (recent_stay_pushed_counts == config->long_detect_cnt) {
                    detect_long = true;
                } else if (recent_stay_pushed_counts == config->long_long_detect_cnt) {
                    detect_long_long = true;
                }
            }
        }
        // === unshift Filter ===
        if (recent_stay_pushed_counts >= config->filter_size) {
            unshift_history(&button->filtered, true);
        } else if (recent_stay_released_counts >= config->filter_size) {
            unshift_history(&button->filtered, false);
        } else {
            unshift_history(&button->filtered, get_pos_history(&button->filtered, 0));
        }
        uint8_t recent_stay_released_counts_filtered = get_recent_stay_released_counts(&button->filtered);
        // === Check Action finished (only if multiClicks) ===
        bool act_finished = recent_stay_released_counts_filtered >= config->act_finish_cnt;
        // === Then, Count rising edge ===
        if (repeat_cnt > 0) { // if repeatCnt,countRise could be 0
            count_rise = 1;
        } else if (act_finished) {
            count_rise = count_rising_edge(&button->filtered, !config->multi_clicks);
        }
        // Clear all once detected, initialize all as true to avoid repeated detection
        if (detect_long || count_rise > 0) {
            clear_history(&button->filtered, true);
        }
        // === Send event ===
        button_event_type_t event_type = EVT_NONE;
        if (count_rise > 1) {
            event_type = EVT_MULTI;
        } else if (count_rise > 0) {
            event_type = EVT_SINGLE;
        } else if (detect_long) {
            event_type = EVT_LONG;
        } else if (detect_long_long) {
            event_type = EVT_LONG_LONG;
        }
        if (event_type != EVT_NONE && queue_get_level(&btn_evt_queue) < EVENT_QUEUE_LENGTH) {
            button_event_t button_event = {
                button->name, // button_name
                event_type,   // event_type
                count_rise,   // click_count
                repeat_cnt    // repeat_count;
            };
            if (!queue_try_add(&btn_evt_queue, &button_event)) {
                //printf("Queue was full\n");
            }
        }
    }
    _scan_count++;
}

bool Buttons::get_button_event(button_event_t* button_event)
{
    int count = queue_get_level(&btn_evt_queue);
    if (count) {
        queue_remove_blocking(&btn_evt_queue, button_event);
        return true;
    } 
    return false;
}

void Buttons::clear_history(button_history_t *history, bool flag)
{
    if (flag) {
        *history = ~0LL;
    } else {
        *history = 0LL;
    }
}

bool Buttons::get_pos_history(button_history_t *history, int i)
{
    uint64_t mask = 1LL << i;
    return (((uint64_t) *history) & mask) != 0LL;
}

void Buttons::unshift_history(button_history_t* history, bool sts)
{
    *history = (*history << 1) | ((uint64_t) sts);
}

uint8_t Buttons::get_recent_stay_pushed_counts(button_history_t* history)
{
    // shortcut for very usual history case
    if (*history == 0) {
        return (uint8_t) 0;
    }
    return trailing_zeros64(~((uint64_t) *history));
}

uint8_t Buttons::get_recent_stay_released_counts(button_history_t* history)
{
    return trailing_zeros64((uint64_t) *history);
}

uint8_t Buttons::count_rising_edge(button_history_t* history, bool single)
{
    uint64_t u64 = (uint64_t) *history;
    const uint64_t even_mask = 0x5555555555555555LL;
    const uint64_t odd_mask  = 0xaaaaaaaaaaaaaaaaLL;
    // check rising at even column
    uint64_t u64e = u64 ^ even_mask;
    u64e = (u64e >> 1) | u64e;
    u64e = u64e & even_mask;
    // check rising at odd column
    uint64_t u64o = u64 ^ odd_mask;
    u64o = (u64o >> 1) | u64o;
    u64o = u64o & odd_mask;
    // merge even and odd (ignore MSB)
    u64 = ~(u64o | u64e | (1LL << 63));
    // shortcuts
    if (u64 == 0LL) {
        return 0;
    } else if (single) {
        return 1;
    }
    // now '1' indicates where rising edge is
    return ones_count64(u64);
}
