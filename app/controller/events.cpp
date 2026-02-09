void Controller::onEmergencyStop()
{
    motor.setOn(false);
    r700.read(false);
    box_rejected = true;
    box_approved = false;
    box_in_process = false;
    led_rgb.state(255, 255, 0);
}

void Controller::onBoxDetected()
{
    box_in_process = true;
    motor.setOn(true);
    r700.read(true);
    current_retry = 0;
    retry_finished = false;
    mySerial.write("#box_detected");
}

void Controller::onTimeout()
{
    motor.setOn(false);
    r700.read(false);
    box_in_process = false;
    mySerial.write("#timeout");
    box_approved = false;
    box_rejected = false;
}

void Controller::onFinishRetry()
{
    motor.setOn(false);
    r700.read(false);
    retry_finished = true;
    mySerial.write("#retry_finished");
}

void Controller::onBoxApproved()
{
    box_in_process = false;
    box_approved = false;
    box_out_detected = false;
    motor.setOn(false);
    mySerial.write("#box_approved");
}

void Controller::onBoxRejected()
{
    box_in_process = false;
    box_rejected = false;
    box_out_detected = false;
    motor.setOn(false);
    mySerial.write("#box_rejected");
}