class Sensors
{
    int in_pin;
    int out_pin;
    int emg_pin;

public:
    // Constructor: receives the sensor pins and emergency pin
    Sensors(int in_pin, int out_pin, int emg_pin)
        : in_pin(in_pin), out_pin(out_pin), emg_pin(emg_pin) {}

    // Initializes the sensor pins and emergency pin as input
    void setup()
    {
        pinMode(in_pin, INPUT_PULLUP);
        pinMode(out_pin, INPUT_PULLUP);
        pinMode(emg_pin, INPUT_PULLUP);
    }

    // Reads the state of the IN sensor
    int readIn()
    {
        return digitalRead(in_pin) == LOW;
    }

    // Reads the state of the OUT sensor
    int readOut()
    {
        return digitalRead(out_pin) == LOW;
    }

    // Checks the state of the emergency stop pin
    bool check_emergency_stop()
    {
        return digitalRead(emg_pin) == LOW;
    }
};
