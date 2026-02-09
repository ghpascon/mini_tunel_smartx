
class CheckSerial
{
public:
    String check_serial()
    {
        if (!Serial.available())
            return "";
        String cmd = Serial.readStringUntil('\n');
        return cmd;
    }

    void check()
    {
        String cmd = check_serial();
        if (cmd == "")
            return;
        else if (cmd.startsWith("#speed:"))
        {
            cmd.replace("#speed:", "");
            motor.setSpeed(cmd.toInt());
        }
        else if (cmd.startsWith("#retry:"))
        {
            cmd.replace("#retry:", "");
            controller.setRetryCount(cmd.toInt());
        }
        else if (cmd == "#approve")
            controller.approveBox();
        else if (cmd == "#reject")
            controller.rejectBox();
        else
            mySerial.write("Received command: " + cmd);
    }
};
