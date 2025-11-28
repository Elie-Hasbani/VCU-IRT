

class VCU
{
public:
    void init();
    void Task10ms();
    void receiveCanCallback(uint32_t id, uint32_t data, uint8_t length);

private:
    void init_throttle_test_set_1();
    void init_utils();
};