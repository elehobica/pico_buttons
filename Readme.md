# Raspberry Pi Pico Buttons
## Overview
This project is a library for handling multiple buttons on Raspberry Pi Pico

This project features to detect:
* Single Push event
* Repeated Single Push event
* Multiple Push event (exclusive with Repeated Single)
* Long (Long) Push event (exclusive with Repeated Single)

## Supported Board and Device
* Raspberry Pi Pico
* 5 Way Switch + 2 Buttons

## Pin Assignment & Connection
### 5 Way Switch + 2 Buttons
| Pico Pin # | Pin Name | Function | Connection |
----|----|----|----
| 23 | GND | GND | COM |
| 24 | GP18 | GPIO Input | RESET |
| 25 | GP19 | GPIO Input | SET |
| 26 | GP20 | GPIO Input | CENTER |
| 27 | GP21 | GPIO Input | RIGHT |
| 29 | GP22 | GPIO Input | LEFT |
| 31 | GP26 | GPIO Input | DOWN |
| 32 | GP27 | GPIO Input | UP |

## How to build
* See ["Getting started with Raspberry Pi Pico"](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf)
* Put "pico-sdk" and "pico-examples" on the same level with this project folder.
* Build is confirmed in Developer Command Prompt for VS 2022 and Visual Studio Code on Windows enviroment
* Confirmed with Pico SDK 1.5.1, cmake-3.27.2-windows-x86_64 and gcc-arm-none-eabi-10.3-2021.10-win32
```
> git clone -b 1.5.1 https://github.com/raspberrypi/pico-sdk.git
> cd pico-sdk
> git submodule update -i
> cd ..
> git clone -b sdk-1.5.1 https://github.com/raspberrypi/pico-examples.git
>
> git clone -b sdk-1.5.1 https://github.com/raspberrypi/pico-extras.git
> 
> git clone -b main https://github.com/elehobica/pico_buttons.git
```
* Lanuch "Developer Command Prompt for VS 2022"
```
> cd pico_buttons\example
> mkdir build && cd build
> cmake -G "NMake Makefiles" ..
> nmake
```
* Put "pico_buttons.uf2" on RPI-RP2 drive

## Usage Guide
### Button Function Assignment
* Multiple / Long / LongLong detection for Center button
* Single / Repeated Single detection for Left/Right/Up/Down buttons
* Single detection for Set/Reset buttons

### Note
* If Multiple detection enabled, time lag defined by 'actFinishCnt' is needed to determine action
* Repeat count information of Repeated Single detection is served for UI items to accelerate something by continuous button push
* Use NewButtonConfig() for customizing button detection parameters other than default configurations
* Trriple clicks of Center button shows processing time of button scan function (in this example project)

### Log Example
```
==================
== pico_buttons ==
==================
center: 1
left: 1
right: 1
up: 1
up: 1
right: 1
down: 1
down: 1 (Repeated 1)
down: 1 (Repeated 2)
center: 2
set: 1
reset: 1
center: Long
center: LongLong
center: 3
time 19us (scan: 650)
```
