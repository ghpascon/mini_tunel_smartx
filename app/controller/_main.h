class Controller
{
    bool box_in_process = false;
    int retry_count = 0;

public:
    void loop()
    {
        // check emergency stop
        if (!check_emergency_stop())
            return;

        // wait for box if not in process
        if (!box_in_process)
        {
            wait_for_box();
            return;
        }
    }

    void setRetryCount(int count)
    {
        retry_count = count;
        mySerial.write("Retry count set to " + String(retry_count));
    }

private:
    bool check_emergency_stop()
    {
        static bool last_emg_state = false;
        bool state = sensors.check_emergency_stop();
        if (state != last_emg_state)
        {
            last_emg_state = state;
            mySerial.write("#EMGERGENCY STOP: " + String(state));
        }
        if (state)
        {
            motors.setOn(false);
            r700.read(false);
            return false;
        }
        return true;
    }

    void wait_for_box()
    {
        if (box_in_process)
            return;

        if (sensors.readIn() == LOW)
        {
            box_in_process = true;
            motors.setOn(true);
            mySerial.write("Box IN Process");
        }
    }
};
