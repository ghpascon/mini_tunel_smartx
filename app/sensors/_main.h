#include <Bounce2.h>

class Sensors
{
    int in_pin;
    int out_pin;
    int emg_pin;
    int feeder_sensor_pin;
    Bounce deb_in;
    Bounce deb_out;
    Bounce deb_emg;
    Bounce deb_feeder;

public:
    // Constructor: receives the sensor pins and emergency pin
    Sensors(int in_pin, int out_pin, int emg_pin, int feeder_sensor_pin)
        : in_pin(in_pin), out_pin(out_pin), emg_pin(emg_pin), feeder_sensor_pin(feeder_sensor_pin) {}

    // Initializes the sensor pins and emergency pin as input
    void setup()
    {
        pinMode(in_pin, INPUT_PULLUP);
        pinMode(out_pin, INPUT_PULLUP);
        pinMode(emg_pin, INPUT_PULLUP);
        pinMode(feeder_sensor_pin, INPUT_PULLUP);
        deb_in.attach(in_pin);
        deb_in.interval(50);
        deb_out.attach(out_pin);
        deb_out.interval(50);
        deb_emg.attach(emg_pin);
        deb_emg.interval(50);
        deb_feeder.attach(feeder_sensor_pin);
        deb_feeder.interval(50);
    }

    // Atualiza o estado dos botões (deve ser chamado no loop)
    void update()
    {
        deb_in.update();
        deb_out.update();
        deb_emg.update();
        deb_feeder.update();
    }

    // Reads the state of the IN sensor
    bool readIn()
    {
        return deb_in.read() == LOW;
    }

    // Reads the state of the OUT sensor
    bool readOut()
    {
        return deb_out.read() == LOW;
    }

    // Checks the state of the emergency stop pin
    bool check_emergency_stop()
    {
        return deb_emg.read() == HIGH;
    }

    // Reads the state of the feeder sensor
    bool readFeederSensor()
    {
        return deb_feeder.read() == LOW;
    }
};
