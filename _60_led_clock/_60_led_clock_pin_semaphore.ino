// What this is for:
// PIN_ESP_RCV goes low:
// * cmdBeginTime = now()
// * sync internal time, keep clock running on internal rather than RTC
// * change from i2c master to slave, to receive data from ESP8266.
// * PIN_ESP_ACK high to acknowledge and signal readiness
// wait for i2c data
// receive i2c command
// * if ntp data: add now() - cmdBeginTime, sync
// (optional) wait for PIN_ESP_RCV to go high
// PIN_ESP_ACK low
// restore normal operation


//semaphore pins
#define PIN_ESP_RCV 3
#define PIN_ESP_ACK 7

time_t cmdBeginTime = 0;
bool isNormalOperation = true;


void semaphoreSetup(){
	semaphoreNormal();
	
}

void semaphoreNormal(){
	pinMode(PIN_ESP_ACK, OUTPUT);
	digitalWrite(PIN_ESP_ACK, LOW);
	pinMode(PIN_ESP_RCV, OUTPUT); // OUTPUT for Interrupts. Logically, more INPUT
	digitalWrite(PIN_ESP_RCV, HIGH); // pull-up resistor enabled on input
	attachInterrupt(PIN_ESP_RCV, espPinToggle, CHANGE);//or FALLING?
	isNormalOperation = true;
}

void onRcvFalling(){
	if(!isNormalOperation)
		Serial.println("Warning: onRcvRising while still serving!");
		return;
	isNormalOperation = false;
	pinMode(PIN_ESP_ACK, OUTPUT);
	digitalWrite(PIN_ESP_ACK, LOW);
	pinMode(PIN_ESP_RCV, OUTPUT); // OUTPUT for Interrupts. Logically, more INPUT
	digitalWrite(PIN_ESP_RCV, HIGH); // pull-up resistor enabled on input
	attachInterrupt(PIN_ESP_RCV, espPinToggle, CHANGE);//or FALLING?
}



void espPinToggle(){
	read PIN_ESP_RCV (it's "output", tho?)
	toggle i2c master/slave
	
}
