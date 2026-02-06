class Motor
{
    int on_pin;
    int reverse_pin;
    int speed_pin_1;
    int speed_pin_2;

public:
    // Constructor: receives the pins
    Motor(int on_pin, int reverse_pin, int speed_pin_1, int speed_pin_2)
        : on_pin(on_pin), reverse_pin(reverse_pin), speed_pin_1(speed_pin_1), speed_pin_2(speed_pin_2) {}

    // Initializes all pins as output
    void setup()
    {
        pinMode(on_pin, OUTPUT);
        pinMode(reverse_pin, OUTPUT);
        pinMode(speed_pin_1, OUTPUT);
        pinMode(speed_pin_2, OUTPUT);

        mySerial.write("Motor initialized");
    }

    // Sets speed: 1 to 4 -> 0,0; 0,1; 1,0; 1,1
    void setSpeed(int speed)
    {
        mySerial.write("Setting motor speed to " + String(speed));
        switch (speed)
        {
        case 1:
            digitalWrite(speed_pin_1, LOW);
            digitalWrite(speed_pin_2, LOW);
            break;
        case 2:
            digitalWrite(speed_pin_1, LOW);
            digitalWrite(speed_pin_2, HIGH);
            break;
        case 3:
            digitalWrite(speed_pin_1, HIGH);
            digitalWrite(speed_pin_2, LOW);
            break;
        case 4:
            digitalWrite(speed_pin_1, HIGH);
            digitalWrite(speed_pin_2, HIGH);
            break;
        default:
            digitalWrite(speed_pin_1, LOW);
            digitalWrite(speed_pin_2, LOW);
            break;
        }
    }

    // Sets motor ON/OFF
    void setOn(bool on, bool reverse = false)
    {
        mySerial.write("Setting motor ON to " + String(on) + ", reverse to " + String(reverse));
        digitalWrite(on_pin, on ? HIGH : LOW);
        digitalWrite(reverse_pin, reverse ? HIGH : LOW);
    }
};
