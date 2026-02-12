void Controller::feeder_loop()
{
    feeder_timeout();
    if (feeder_check_rejected())
        return;
    feeder_check_sensor();
    feeder_check_box_inside();
}

bool Controller::feeder_check_rejected()
{
    // set reject var
    if (box_rejected)
    {
        feeder_rejected = true;
        box_on_feeder = true;
    }
    if (!feeder_rejected)
        return false;

    feeder_motor.setOn(true, true);

    // wait for sensor to be triggered
    static bool last_feeder_sensor = false;
    bool feeder_sensor = sensors.readFeederSensor();
    if (feeder_sensor == last_feeder_sensor)
        return true;
    last_feeder_sensor = feeder_sensor;

    // when box out
    if (!feeder_sensor)
    {
        feeder_rejected = false;
        box_on_feeder = false;
        return true;
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
    if (box_on_feeder)
        current_time = millis();
    if (millis() - current_time < (feeder_rejected ? 5000 : 2000))
        return;
    box_on_feeder = false;
    feeder_rejected = false;
    feeder_motor.setOn(false);
}