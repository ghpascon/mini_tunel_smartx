#include <UnicViewAD.h>
LCM Lcm(Serial2);
// vars
LcmVar speed_button(0);
LcmString speed_label(100, 100);
LcmVar retry_button(1);
LcmString retry_label(200, 100);
LcmString status_label(300, 200);
LcmVar screen_button(2);

class Display
{
public:
    // screen
    byte load_screen = 2;
    byte main_screen = 0;
    byte emg_screen = 4;
    byte approved_screen = 5;
    byte rejected_screen = 6;

    void setup()
    {
        Serial2.begin(115200, SERIAL_8N1, DISPLAY_RX_PIN, DISPLAY_TX_PIN);
        Lcm.begin();
        Lcm.changeBacklight(100);
    }

    void loop()
    {
        load_screen_on_start();
        change_screen_on_state();
        check_buttons();
        update_labels();
    }

private:
    void load_screen_on_start()
    {
        static bool loaded = false;
        if (loaded)
            return;
        Lcm.changePicId(load_screen);
        if (millis() < 5000)
            return;
        mySerial.write("Changing to main screen...");
        Lcm.changePicId(main_screen);
        loaded = true;
    }

    void change_screen_on_state()
    {
        static unsigned long last_update = 0;
        if (millis() - last_update < 500 || millis() < 5000)
            return;
        last_update = millis();

        byte state = 0;
        if (sensors.check_emergency_stop())
            state = emg_screen;
        else if (controller.box_approved)
            state = approved_screen;
        else if (controller.box_rejected)
            state = rejected_screen;
        else
            state = main_screen;

        Lcm.changePicId(state);
    }

    void check_buttons()
    {
        // speed
        if (speed_button.available())
        {
            int value = speed_button.getData();
            int speed = motor.getSpeed() + (value == 0 ? -1 : +1);
            motor.setSpeed(speed);
        }

        // retry
        if (retry_button.available())
        {
            int value = retry_button.getData();
            int retry = controller.retry_count + (value == 0 ? -1 : +1);
            controller.setRetryCount(retry);
        }

        // screen
        if (screen_button.available())
        {
            int value = screen_button.getData();
            main_screen = value;
        }
    }

    void update_labels()
    {
        update_speed_label();
        update_retry_label();
        update_status_label();
    }

    void update_speed_label()
    {
        static int last_speed = 0;
        static unsigned long last_update = 0;
        int speed = motor.getSpeed();
        if (speed != last_speed || millis() - last_update >= 3000)
        {
            last_speed = speed;
            String speed_str = String(speed);
            speed_label.write(speed_str);
            last_update = millis();
        }
    }

    void update_retry_label()
    {
        static int last_retry = 0;
        static unsigned long last_update = 0;
        int retry = controller.retry_count;
        if (retry != last_retry || millis() - last_update >= 3000)
        {
            last_retry = retry;
            String retry_str = String(retry);
            retry_label.write(retry_str);
            last_update = millis();
        }
    }

    void update_status_label()
    {
        static String last_status = "";
        static unsigned long last_update = 0;
        String status =
            "MOTOR: " + String(motor.getOn() ? "ON" : "OFF") + "\n"
                                                               "READING: " +
            String(r700.get_read() ? "ON" : "OFF") + "\n"
                                                     "STATUS: " +
            String(controller.box_approved ? "APPROVED" : controller.box_rejected ? "REJECTED"
                                                                                  : "WAITING") +
            "\n"
            "RETRY: " +
            String(controller.retry_count) + "\n"
                                             "SPEED: " +
            String(motor.getSpeed()) + "\n"
                                       "EMG: " +
            String(sensors.check_emergency_stop() ? "ON" : "OFF") + "\n"
                                                                    "SENSOR IN: " +
            String(sensors.readIn() ? "ON" : "OFF") + "\n"
                                                      "SENSOR OUT: " +
            String(sensors.readOut() ? "ON" : "OFF") + "\n";
        if (status != last_status || millis() - last_update >= 3000)
        {
            last_status = status;
            status_label.write(status);
            last_update = millis();
        }
    }
};