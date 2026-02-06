
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
            motors.setSpeed(cmd.toInt());
        }
        else if (cmd.startsWith("#retry:"))
        {
            cmd.replace("#retry:", "");
            controller.setRetryCount(cmd.toInt());
        }
        else
            mySerial.write("Received command: " + cmd);
    }
};
