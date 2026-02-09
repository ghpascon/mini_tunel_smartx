

#include <Arduino.h>
#include <Preferences.h>

class Settings
{
public:
    void begin()
    {
        prefs.begin("settings", false);
        int ms = prefs.getInt("motor_speed", 1);
        int rc = prefs.getInt("retry_count", 0);
        last_motor_speed = ms;
        last_retry_count = rc;
        // O usuário deve aplicar ms e rc manualmente
    }

    void handle()
    {
        unsigned long now = millis();
        static unsigned long lastCheck = 0;
        if (now - lastCheck >= 10000)
        {
            lastCheck = now;
            if (motor.getSpeed() != last_motor_speed)
            {
                mySerial.write("Saving motor speed: " + String(motor.getSpeed()));
                prefs.putInt("motor_speed", motor.getSpeed());
                last_motor_speed = motor.getSpeed();
            }
            if (controller.retry_count != last_retry_count)
            {
                mySerial.write("Saving retry count: " + String(controller.retry_count));
                prefs.putInt("retry_count", controller.retry_count);
                last_retry_count = controller.retry_count;
            }
        }
    }

    int last_motor_speed = 1;
    int last_retry_count = 0;

private:
    Preferences prefs;
};
