
class MySerial
{
public:
    void setup()
    {
        Serial.begin(115200);
    }

    void write(const String &msg)
    {
        Serial.println(msg);
    }
};
