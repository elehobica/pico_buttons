# Raspberry Pi Pico Buttons
## Overview
This project is a library for handling multiple buttons on Raspberry Pi Pico

This project features to detect:
* Single Push event
* Repeated Single Push event
* Multiple Push event (exclusive with Repeated Single)
* Long (Long) Push event (exclusive with Repeated Single)
* Switch On/Off events

## Supported Board and Device
* Raspberry Pi Pico

## How to build
* See [example project](example/Readme.md)

### Note
* If Multiple detection enabled, time lag defined by 'actFinishCnt' is needed to determine action
* Repeat count information of Repeated Single detection is served for UI items to accelerate something by continuous button push
* Use NewButtonConfig() for customizing button detection parameters other than default configurations