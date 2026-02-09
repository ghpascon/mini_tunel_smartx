#include "pins.h"
#include "credentials.h"

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
Motor feeder_motor(FEEDER_MOTOR_ON_PIN, FEEDER_MOTOR_REVERSE_PIN, -1, -1);

// SENSORS
#include "app/sensors/_main.h"
Sensors sensors(SENSOR_IN_PIN, SENSOR_OUT_PIN, EMG_PIN, FEEDER_SENSOR_PIN);

// CONTROLLER
#include "app/controller/_main.h"
#include "app/controller/events.cpp"
#include "app/controller/feeder.cpp"
Controller controller;

// checkSerial
#include "app/myserial/check_serial.h"
CheckSerial checkSerial;

// Display
#include "app/display/_main.h"
Display display;

// Network Manager
#include "app/network/_main.h"
MyNetwork my_network;

// Settings
#include "app/settings/_main.h"
Settings settings;

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void auxLoop(void *pvParameters) {
  while (true) {
    checkSerial.check();
    settings.handle();
    display.loop();
    vTaskDelay(1);
  }
}

void setup_parameters() {
  motor.setSpeed(settings.last_motor_speed);
  controller.setRetryCount(settings.last_retry_count);
}

void setup() {
  settings.begin();

  mySerial.setup();
  led_rgb.setup();

  r700.setup();
  motor.setup();
  sensors.setup();

  setup_parameters();

  display.setup();
  my_network.setup(ssid, password);

  xTaskCreatePinnedToCore(
    auxLoop,    // Função da task
    "auxLoop",  // Nome da task
    2048,       // Stack size
    NULL,       // Parametros
    1,          // Prioridade
    NULL,       // Handle
    1           // Core (0=PRO, 1=APP)
  );
}

void loop() {
  // update sensors
  sensors.update();
  r700.update();

  // controller loop
  controller.loop();

  // Handle my_network
  my_network.update();
}