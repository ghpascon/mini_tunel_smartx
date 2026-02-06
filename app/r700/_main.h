class R700
{
    int read_pin;
    int ok_pin;
    int nok_pin;
    bool _is_reading = false;

public:
    // Constructor: receives the pins
    R700(int read_pin, int ok_pin, int nok_pin)
        : read_pin(read_pin), ok_pin(ok_pin), nok_pin(nok_pin) {}

    // Initializes the pins
    void setup()
    {
        pinMode(read_pin, OUTPUT);
        pinMode(ok_pin, INPUT_PULLUP);
        pinMode(nok_pin, INPUT_PULLUP);
        mySerial.write("R700 initialized");
    }

    // Reads the state of the read pin
    int get_read()
    {
        mySerial.write("Getting R700 read state: " + String(_is_reading));
        return _is_reading;
    }

    // Writes to the read pin
    void read(bool state)
    {
        digitalWrite(read_pin, state ? HIGH : LOW);
        _is_reading = state;
        mySerial.write("Setting R700 read to " + String(state));
    }

    // Returns the state of ok/nok pins: 0 = none, 1 = ok, 2 = nok
    int get_state()
    {
        if (digitalRead(ok_pin) == LOW)

            return 1;
        if (digitalRead(nok_pin) == LOW)
            return 2;
        return 0;
    }
};
