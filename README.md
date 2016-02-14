# iot-a
Junk, mostly.

Group 1 of iot stuff.
Initial idea:
* 1 clock: arduino, connected esp8266 (no idea which one), leds, thermometer, some kinda input. Server allowing setup of alarms and thermostat.
* 1 relay node - 2x power, 2+ USB power ports, thermometer.
* 1 breadboard on battery. Initially: thermometer?

TODO:
* I2C connection
* WiFi, SNTP to arduino
* WiFi end-user setup
* daylight savings and timezone, configurable in UI https://github.com/JChristensen/Timezone
* disable the pattern cycling until real pattern setup is created (move to i2c command; disable saving)
* keypad? I2C send to the non-connected unit...
* command: turn off esp, or at least disable WiFi



