#include <Bounce2.h>

class R700
{
    int read_pin;
    int ok_pin;
    int nok_pin;
    bool _is_reading = false;
    Bounce deb_ok;
    Bounce deb_nok;

public:
    // Constructor: receives the pins
    R700(int read_pin, int ok_pin, int nok_pin)
        : read_pin(read_pin), ok_pin(ok_pin), nok_pin(nok_pin) {}

    // Initializes the pins
    void setup()
    {
        pinMode(read_pin, OUTPUT);
        pinMode(ok_pin, INPUT_PULLDOWN);
        pinMode(nok_pin, INPUT_PULLDOWN);
        deb_ok.attach(ok_pin);
        deb_ok.interval(50);
        deb_nok.attach(nok_pin);
        deb_nok.interval(50);
        mySerial.write("R700 initialized");
    }

    // Atualiza o estado dos botões (deve ser chamado no loop)
    void update()
    {
        deb_ok.update();
        deb_nok.update();
    }

    // Reads the state of the read pin
    int get_read()
    {
        return _is_reading;
    }

    // Writes to the read pin
    void read(bool state)
    {
        digitalWrite(read_pin, state ? LOW : HIGH);
        _is_reading = state;
    }

    // Returns the state of ok/nok pins: 0 = none, 1 = ok, 2 = nok
    int get_state()
    {
        if (deb_ok.read() == HIGH)
            return 1;
        if (deb_nok.read() == HIGH)
            return 2;
        return 0;
    }
};
