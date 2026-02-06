#include "pins.h"
#include "vars.h"

// MySerial
#include "app/myserial/_main.h"
MySerial mySerial;

// LED RGB
#include "app/rgb/_main.h"
LED_RGB led_rgb;

// R700
#include "app/r700/_main.h"
R700 r700(R700_READ_PIN, R700_OK_PIN, R700_NOK_PIN);

// Motor
#include "app/motor/_main.h"
Motor motor(MOTOR_ON_PIN, MOTOR_REVERSE_PIN, MOTOR_SPEED_1_PIN, MOTOR_SPEED_2_PIN);

// SENSORS
#include "app/sensors/_main.h"
Sensors sensors(SENSOR_IN_PIN, SENSOR_OUT_PIN, EMG_PIN);

// CONTROLLER
#include "app/controller/_main.h"
Controller controller;

// checkSerial
#include "app/myserial/check_serial.h"
CheckSerial checkSerial;

void setup()
{
    mySerial.setup();
    led_rgb.setup();

    r700.setup();
    motor.setup();
    sensors.setup();
    led_rgb.state(0, 0, 255);
}

void loop()
{
    checkSerial.check();
    controller.loop();
}