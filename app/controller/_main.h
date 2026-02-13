class Controller
{
public:
    // Events
    void onEmergencyStop();
    void onBoxDetected();
    void onFinishRetry();
    void onBoxApproved();
    void onBoxRejected();
    void onTimeout();
    // Feeder
    void feeder_loop();
    void feeder_check_sensor();
    void feeder_check_box_inside();
    void feeder_timeout();
    bool feeder_check_rejected();
    bool feeder_check_emg();
    bool box_on_feeder = false;
    bool is_rejecting = false;

    // vars
    bool box_in_process = false;
    int retry_count = 0;
    int current_retry = 0;
    bool box_approved = false;
    bool box_rejected = false;
    bool retry_finished = false;
    bool box_out_detected = false;
    int current_speed = 1;

public:
    void loop()
    {
        feeder_loop();
        // check emergency stop
        if (check_emergency_stop())
            return;

        // check for timeout
        check_timeout();

        // verify if has an state for aproved or rejected box
        check_r700_state();
        delayed_stop();
        speed_up_on_state();
        if (check_state())
            return;

        // wait for box if not in process
        if (!box_in_process)
        {
            wait_for_box();
            return;
        }

        // check sensor out for retry logic
        checkSensorOut();

        // timeout for answer
        checkAnswerTimeout();
    }

    void setRetryCount(int count)
    {
        if (count < 0)
            count = 0;
        if (count > 10)
            count = 10;
        retry_count = count;
        mySerial.write("#retry_count_set:" + String(retry_count));
    }

    void approveBox()
    {
        box_approved = true;
        box_rejected = false;
        mySerial.write("#box_approved_command_received");
    }

    void rejectBox()
    {
        box_rejected = true;
        box_approved = false;
        mySerial.write("#box_rejected_command_received");
    }

private:
    bool check_emergency_stop()
    {
        static bool last_emg_state = false;
        bool state = sensors.check_emergency_stop();
        if (state != last_emg_state)
        {
            last_emg_state = state;
            mySerial.write("#EMERGENCY STOP:" + String(state));
        }
        if (state)
        {
            onEmergencyStop();
            return true;
        }
        return false;
    }

    void check_timeout()
    {
        int timeout = 5000;
        static unsigned long current_time = 0;
        if (!motor.getOn() || (sensors.readIn() || sensors.readOut()))
        {
            current_time = millis();
        }
        if (millis() - current_time < timeout)
            return;

        onTimeout();
    }

    void check_r700_state()
    {
        int state = r700.get_state();

        if (state == 1)
        {
            box_approved = true;
            box_rejected = false;
        }
        else if (state == 2)
        {
            box_rejected = true;
            box_approved = false;
        }
    }

    void delayed_stop()
    {
        static unsigned long current_time = 0;
        int timeout = box_rejected ? 2000 : 1000;
        if (!box_out_detected)
            current_time = millis();
        else if (sensors.readOut() || sensors.readIn())
            current_time = millis();
        if (millis() - current_time < timeout)
            return;
        if (box_approved)
            onBoxApproved();
        else if (box_rejected)
            onBoxRejected();
    }

    bool check_state()
    {
        if (box_approved && box_rejected)
        {
            // conflicting state, prioritize rejection
            box_approved = false;
        }

        // box approved
        if (box_approved)
        {
            led_rgb.state(0, 255, 0);

            // motor forward to release box
            motor.setOn(true);

            // Wait for box to be removed
            static bool last_out_state = false;
            bool out_state = sensors.readOut();
            if (out_state == last_out_state)
                return true;

            last_out_state = out_state;
            if (out_state)
                return true;

            // box removed
            box_out_detected = true;
            return true;
        }

        // box rejected
        if (box_rejected)
        {
            led_rgb.state(255, 0, 0);

            // motor reverse to reject box
            motor.setOn(true, true);

            // Wait for box to be removed
            static bool last_in_state = false;
            bool in_state = sensors.readIn();
            if (in_state == last_in_state)
                return true;

            last_in_state = in_state;
            if (in_state)
                return true;

            // box removed
            box_out_detected = true;
            return true;
        }

        // no state
        if (box_in_process)
            led_rgb.state(0, 0, 255);
        else
            led_rgb.state(0, 255, 255);

        return false;
    }

    void wait_for_box()
    {
        if (box_in_process)
            return;

        current_retry = 0;
        static bool last_in_state = false;
        bool in_state = sensors.readIn();

        motor.setOn(in_state);

        if (in_state == last_in_state)
            return;
        last_in_state = in_state;
        if (!in_state)
        {
            onBoxDetected();
        }
    }

    void checkSensorOut()
    {
        static bool last_out_state = false;
        if (retry_finished)
        {
            last_out_state = false;
            return;
        }
        bool out_state = sensors.readOut();
        if (out_state == last_out_state)
            return;

        last_out_state = out_state;

        // box out sensor detected
        if (!out_state)
        {
            motor.setOn(true);
            current_retry++;
            mySerial.write("#current_retry:" + String(current_retry) + "/" + String(retry_count));
            return;
        }

        // box in sensor detected
        // check if we reached retry limit
        if (current_retry >= retry_count)
        {
            onFinishRetry();
            return;
        }
        // reverse motor for retry
        motor.setOn(true, true);
    }

    void checkAnswerTimeout()
    {
        static unsigned long current_time = 0;
        int answer_timeout = 5000;
        if (!retry_finished)
            current_time = millis();
        if (millis() - current_time < answer_timeout)
            return;
        box_rejected = true;
        mySerial.write("#answer_timeout");
    }

    void speed_up_on_state()
    {
        static bool last_state = false;
        bool state = box_approved || box_rejected;
        if (state == last_state)
            return;
        last_state = state;
        if (!state)
        {
            motor.setSpeed(current_speed);
            return;
        }
        else
        {
            current_speed = motor.getSpeed();
            motor.setSpeed(4);
        }
    }
};
