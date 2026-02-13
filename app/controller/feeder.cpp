void Controller::feeder_loop()
{
    if (feeder_check_emg())
        return;
    feeder_timeout();
    if (feeder_check_rejected())
        return;
    feeder_check_sensor();
    feeder_check_box_inside();
}

bool Controller::feeder_check_emg()
{
    if (!sensors.check_emergency_stop())
        return false;
    box_on_feeder = false;
    is_rejecting = false;
    feeder_motor.setOn(false);
    return true;
}

bool Controller::feeder_check_rejected()
{
    int rejecting_time = 2000;
    // is rejecting
    static unsigned long prevent_time = 0;

    if (millis() - prevent_time < rejecting_time)
        return false;

    if (box_rejected)
        is_rejecting = true;
    if (!is_rejecting)
        return false;

    // feeder motor
    static unsigned long current_time = 0;
    bool sensor_state = sensors.readFeederSensor();
    if (sensor_state)
        current_time = millis();

    if (millis() - current_time < rejecting_time)
        feeder_motor.setOn(false);
    else
        feeder_motor.setOn(true, true);
    // when box out
    static bool last_feeder_sensor = false;
    if (sensor_state == last_feeder_sensor)
        return true;
    last_feeder_sensor = sensor_state;
    if (!sensor_state)
    {
        prevent_time = millis();
        is_rejecting = false;
        feeder_motor.setOn(false);
    }
    return true;
}

void Controller::feeder_check_sensor()
{
    // get feeder sensor state
    if (box_in_process)
    {
        feeder_motor.setOn(false);
        return;
    }
    static bool last_feeder_state = false;
    bool feeder_state = sensors.readFeederSensor();
    if (feeder_state == last_feeder_state)
        return;
    last_feeder_state = feeder_state;
    if (feeder_state)
    {
        feeder_motor.setOn(true);
        box_on_feeder = true;
    }
}

void Controller::feeder_check_box_inside()
{
    static bool last_sensor_in_state = false;
    bool sensor_in_state = sensors.readIn();
    if (sensor_in_state == last_sensor_in_state)
        return;
    last_sensor_in_state = sensor_in_state;
    if (box_on_feeder && !sensor_in_state)
    {
        feeder_motor.setOn(false);
        box_on_feeder = false;
    }
}

void Controller::feeder_timeout()
{
    static unsigned long current_time = 0;
    if (!feeder_motor.getOn())
        current_time = millis();
    if (millis() - current_time < (box_rejected ? 5000 : 2000))
        return;

    box_on_feeder = false;
    is_rejecting = false;
    feeder_motor.setOn(false);
}