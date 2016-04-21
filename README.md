# iot-a
Junk, mostly.

Group 1 of iot stuff.
Initial idea:
* 1 clock: arduino, connected esp8266
* 1 relay node - 2x power, 2+ USB power ports, thermometer. Maybe other radio.
* 1 breadboard on battery. Initially: thermometer? Use different radio?

TODO:
* switch i2c mater/slave status based on pin from esp
** use different time method when arduino is slave
** verification line (another pin pair) to verify readiness
* alternative: software i2c

* reattach RTC
* WiFi end-user setup
* keypad? I2C send to the non-connected unit...
* command: turn off esp, or at least disable WiFi
* rather than buttons:
** knock (https://www.arduino.cc/en/Tutorial/Knock)
** reed (sth special, like store settings)
** touch sensor (light intensity levels? Fluid increase/decrease)
* switch between STA & AP. AP times out after 8 minutes (tmr) reset on every request made to server in that mode
* add a piezo buzzer (for tone generation, not alarm) to arduino, for tick sounds (PWM-capable pin, but use digital only: 1->0, 0->1, ...) and later use for tones





