/*------------------------------------------------------/
/ Copyright (c) 2023, Elehobica
/ Released under the BSD-2-Clause
/ refer to https://opensource.org/licenses/BSD-2-Clause
/------------------------------------------------------*/

#pragma once

#include "pico/stdlib.h"
#include "pico/util/queue.h"

typedef enum _button_event_type_t {
    EVT_NONE,
    EVT_SINGLE,
    EVT_MULTI,
    EVT_LONG,
    EVT_LONG_LONG,
    EVT_ON,
    EVT_OFF
} button_event_type_t;

typedef struct _button_event_t {
    const char*         button_name;
    button_event_type_t type;
    uint8_t             click_count;
    uint8_t             repeat_count;
} button_event_t;

typedef struct _button_config_t {
    bool    is_button;             // Set true if it's a button or set false if it's a switch
    bool    active_high;           // Set false if button is connected between GND and pin with pull-up
    bool    multi_clicks;          // Detect multiple clicks if true, detect single click if false
    uint8_t filter_size;           // Filter size to process raw status
    uint8_t act_finish_cnt;        // Button action detection starts when status keeps false at latest continuous actFinishCnt times (only if multiClicks)
    uint8_t repeat_detect_cnt;     // Continuous counts to detect Repeat click when continuous push (only if !multiClicks. ignored if 0. if defined Long/LongLong detect disabled)
    uint8_t repeat_skip;           // Skip count for Repeat click detection (every scan if 0)
    uint8_t long_detect_cnt;       // Continuous counts to detect Long Push (ignored if 0)
    uint8_t long_long_detect_cnt;  // Continuous counts to detect LongLong Push (ignored if 0)
} button_config_t;

typedef uint64_t button_history_t;

typedef struct _button_t {
    const char* name;
    const uint pin;
    const button_config_t* config;
    button_history_t history;
    button_history_t filtered;
    uint8_t rpt_cnt;
} button_t;

class Buttons {
    public:
    static constexpr int EVENT_QUEUE_LENGTH = 1;
    static constexpr button_config_t DEFAULT_BUTTON_SINGLE_CONFIG = {
        true,   // is_button
        false,  // active_high
        false,  // multi_clicks
        1,      // filter_size
        0,      // act_finish_cnt
        0,      // repeat_detect_cnt
        0,      // repeat_skip
        0,      // long_detect_cnt
        0       // long_long_detect_cnt
    };
    static constexpr button_config_t DEFAULT_BUTTON_SINGLE_REPEAT_CONFIG = {
        true,   // is_button
        false,  // active_high
        false,  // multi_clicks
        1,      // filter_size
        0,      // act_finish_cnt
        10,     // repeat_detect_cnt
        2,      // repeat_skip
        0,      // long_detect_cnt
        0       // long_long_detect_cnt
    };
    static constexpr button_config_t DEFAULT_BUTTON_MULTI_CONFIG = {
        true,   // is_button
        false,  // active_high
        true,   // multi_clicks
        1,      // filter_size
        5,      // act_finish_cnt
        0,      // repeat_detect_cnt
        2,      // repeat_skip
        15,     // long_detect_cnt
        39      // long_long_detect_cnt
    };
    static constexpr button_config_t DEFAULT_SWITCH_CONFIG = {
        false,  // is_button
        false,  // active_high
        false,  // multi_clicks
        1,      // filter_size
        0,      // act_finish_cnt
        0,      // repeat_detect_cnt
        0,      // repeat_skip
        0,      // long_detect_cnt
        0       // long_long_detect_cnt
    };

    Buttons(button_t* buttons, int size, uint8_t scan_skip = 0);
    virtual ~Buttons() {};
    void scan_periodic();
    bool get_button_event(button_event_t& button_event);

    private:
    button_t* _buttons;
    int _size;
    uint8_t _scan_skip;
    uint32_t _scan_count;
    queue_t btn_evt_queue;

    void clear_history(button_history_t& history, bool flag);
    bool get_pos_history(button_history_t& history, int i);
    void unshift_history(button_history_t& history, bool sts);
    uint8_t get_recent_stay_pushed_counts(button_history_t& history);
    uint8_t get_recent_stay_released_counts(button_history_t& history);
    uint8_t count_rising_edge_core(uint64_t u64, bool single);
    uint8_t count_rising_edge(button_history_t& history, bool single);
    uint8_t count_falling_edge(button_history_t& history, bool single);
};