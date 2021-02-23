#ifndef __HX711_H_
#define __HX711_H_

#include "Arduino.h"
#include <stdint.h>

#define HIGH    1
#define LOW     0
#define BITWRITE(n, x, y) (n |= (y << x))

class HX711
{
    private:
        uint8_t _sck;
        uint8_t _dout;
        uint8_t _gain;
        long _offset;
        float _scale;
    public:
        HX711(void);
        HX711(uint8_t, uint8_t);
        void init(uint8_t dout, uint8_t sck, uint8_t gain = 128, float scale = 2500.f);
        long read(void);
        float get_data(int);
        bool is_ready(void);
        void power_up(void);
        void power_down(void);
        void set_gain(uint8_t times = 10);
        void set_scale(float);
        void set_offset(long);
        uint8_t get_gain(void);
        long get_offset(void);
        float get_scale(void);
};

#endif