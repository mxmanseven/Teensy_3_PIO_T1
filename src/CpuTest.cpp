#include <Arduino.h>

    // with 
    // board_f_cpu = 24000000
    // k: 125001752576.00
    // duration 4532
    // with out cpu setting:
    // duration 2174
    // k: 125001752576.00
void CpuTest()
{
    for (int ii = 0; ii < 5; ii++)
    {
        uint32_t s = millis();

        double d = 2;
        double j = 2;
        double k = 0;
        for (int i = 0; i < 500000; i++)
        {
            k = d * j + i + k; 
        }


        uint32_t e = millis();
        uint32_t t = e - s;

        Serial.println("duration "  + String(t));
        Serial.println("k: " + String(k));
    }
}