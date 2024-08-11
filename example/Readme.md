# Raspberry Pi Pico Buttons example

## How to build
* See ["Getting started with Raspberry Pi Pico"](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf)
* Put "pico-sdk", "pico-examples" and "pico-extras" on the same level with this project folder.
* Set environmental variables for PICO_SDK_PATH, PICO_EXTRAS_PATH and PICO_EXAMPLES_PATH
* Confirmed with Pico SDK 2.0.0
```
> git clone -b 2.0.0 https://github.com/raspberrypi/pico-sdk.git
> cd pico-sdk
> git submodule update -i
> cd ..
> git clone -b sdk-2.0.0 https://github.com/raspberrypi/pico-examples.git
>
> git clone -b sdk-2.0.0 https://github.com/raspberrypi/pico-extras.git
> 
> git clone -b main https://github.com/elehobica/pico_buttons.git
```
### Windows
* Build is confirmed with Developer Command Prompt for VS 2022 and Visual Studio Code on Windows environment
* Confirmed with cmake-3.27.2-windows-x86_64 and gcc-arm-none-eabi-10.3-2021.10-win32
* Lanuch "Developer Command Prompt for VS 2022"
```
> cd pico_buttons\example
> mkdir build && cd build
> cmake -G "NMake Makefiles" ..
> nmake
```
* Put "pico_buttons_example.uf2" on RPI-RP2 drive
### Linux
* Build is confirmed with [pico-sdk-dev-docker:sdk-2.0.0-1.0.0]( https://hub.docker.com/r/elehobica/pico-sdk-dev-docker)
* Confirmed with cmake-3.22.1 and arm-none-eabi-gcc (15:10.3-2021.07-4) 10.3.1
```
$ cd pico_buttons/example
$ mkdir build && cd build
$ cmake ..
$ make -j4
```
* Download "pico_buttons_example.uf2" on RPI-RP2 drive

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
