class Motor
{
public:
    int on_pin;
    int reverse_pin;
    int speed_pin_1;
    int speed_pin_2;
    bool _is_on = false;
    int _speed = 1;
    // Constructor: receives the pins
    Motor(int on_pin, int reverse_pin, int speed_pin_1, int speed_pin_2)
        : on_pin(on_pin), reverse_pin(reverse_pin), speed_pin_1(speed_pin_1), speed_pin_2(speed_pin_2) {}

    // Initializes all pins as output
    void setup()
    {
        pinMode(on_pin, INPUT_PULLUP);
        pinMode(reverse_pin, INPUT_PULLUP);
        pinMode(speed_pin_1, INPUT_PULLUP);
        pinMode(speed_pin_2, INPUT_PULLUP);

        mySerial.write("Motor initialized");
    }

    int getSpeed()
    {
        return _speed;
    }

    // Sets speed: 1 to 4 -> 0,0; 0,1; 1,0; 1,1
    void setSpeed(int speed)
    {
        if (speed < 1)
            speed = 1;
        if (speed > 4)
            speed = 4;
        _speed = speed;
        mySerial.write("Setting motor speed to " + String(speed));
        switch (speed)
        {
        case 1:
            pinMode(speed_pin_1, INPUT_PULLUP);
            pinMode(speed_pin_2, INPUT_PULLUP);
            break;
        case 2:
            pinMode(speed_pin_1, INPUT_PULLUP);
            pinMode(speed_pin_2, OUTPUT);
            break;
        case 3:
            pinMode(speed_pin_1, OUTPUT);
            pinMode(speed_pin_2, INPUT_PULLUP);
            break;
        case 4:
            pinMode(speed_pin_1, OUTPUT);
            pinMode(speed_pin_2, OUTPUT);
            break;
        default:
            pinMode(speed_pin_1, OUTPUT);
            pinMode(speed_pin_2, OUTPUT);
            break;
        }
    }

    // Sets motor ON/OFF
    void setOn(bool on, bool reverse = false)
    {
        pinMode(on_pin, on ? OUTPUT : INPUT_PULLUP);
        pinMode(reverse_pin, reverse ? OUTPUT : INPUT_PULLUP);
        _is_on = on;
    }

    // Returns if motor is ON
    bool getOn()
    {
        return _is_on;
    }
};
