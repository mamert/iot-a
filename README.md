# iot-a
Junk, mostly.

Group 1 of iot stuff.
Initial idea:
* 1 clock: arduino, connected esp8266 (no idea which one), leds, thermometer, some kinda input. Server allowing setup of alarms and thermostat.
* 1 relay node - 2x power, 2+ USB power ports, thermometer.
* 1 breadboard on battery. Initially: thermometer? Use different rasio?

TODO:
* WiFi end-user setup
* daylight savings and timezone, configurable in UI https://github.com/JChristensen/Timezone
* keypad? I2C send to the non-connected unit...
* command: turn off esp, or at least disable WiFi
* rather than buttons:
** knock (https://www.arduino.cc/en/Tutorial/Knock)
** reed (sth special, like store settings)
** touch sensor (light intensity levels? Fluid increase/decrease)
* switch between STA & AP. AP times out after 8 minutes (tmr) reset on every request made to server in that mode
* add a piezo buzzer (for tone generation, not alarm) to arduino, for tick sounds (PWM-capable pin, but use digital only: 1->0, 0->1, ...) and later use for tones





