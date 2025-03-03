# Raspberry Pi Pico Buttons example

## Pin Assignment
### 5 Way switch + 2 buttons
| Pico Pin # | Pin Name | Function | Connection |
----|----|----|----
| 23 | GND | GND | COM |
| 24 | GP18 | GPIO Input | UP |
| 25 | GP19 | GPIO Input | DOWN |
| 26 | GP20 | GPIO Input | LEFT |
| 27 | GP21 | GPIO Input | RIGHT |
| 29 | GP22 | GPIO Input | CENTER |
| 31 | GP26 | GPIO Input | SET |
| 32 | GP27 | GPIO Input | RESET |

## Usage Guide
### Button Function Assignment
* Multiple / Long / LongLong detection for Center button
* Single / Repeated Single detection for Left/Right/Up/Down buttons
* Single detection for Set/Reset buttons

### Note
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
