#include "hx711.h"

HX711::HX711(void) { }

HX711::HX711(uint8_t dout, uint8_t sck)
{
    this->_dout = dout;
    this->_sck = sck;
}

void HX711::init(uint8_t dout, uint8_t sck, uint8_t gain, float scale)
{
    Serial.begin(115200);
    Serial.println("Serial begin 115200...");

    Serial.println("Pin mode set");
    this->_dout = dout;
    this->_sck = sck;
    pinMode(this->_dout, INPUT);
    pinMode(this->_sck, OUTPUT);

    Serial.println("Calculating average offest for 2sec...");
    long temp = 0;
    for(int i = 0; i < 10; i++)
    {
        temp += this->read();
        delay(200);
    }
    this->_offset = temp / 10;
    Serial.println("Offset setting is done");
    Serial.print("your offset is "); Serial.println(this->_offset);

    Serial.println("Setting Gain...");
    this->set_gain(gain);
    Serial.print("Setting Gain Done"); Serial.println(this->_gain);

    Serial.println("Setting Scale...");
    this->set_scale(scale);
    Serial.print("Setting Scale Done"); Serial.println(this->_scale);
}

long HX711::read(void)
{
    byte data[3];
    long ret;

    while(!is_ready()) { yield(); }

    for(int i = 3; i > 0; i--)
    {
        for(int j = 8; j > 0; j--)
        {
            digitalWrite(this->_sck, HIGH);
            bitWrite(data[i - 1], j, digitalRead(this->_dout));
            digitalWrite(this->_sck, LOW);
        }
    }

    for(int i = 0; i < this->_gain; i++)
    {
        digitalWrite(this->_sck, HIGH); 
        digitalWrite(this->_sck, LOW);
    }    

    if(data[2] == 0x80)
    {
        Serial.println("MIN DATA");
        ret = 0;
    }

    ret = ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];

    return ret;
}

float HX711::get_data(int times)
{
    long data = 0;
    float ret = 0.0f;

    for(int i = 0; i < times; i++)
    {
        data += this->read();
    }

    // Serial.print("data1 : "); Serial.println(data);
    data /= times;
    // Serial.print("data2 : "); Serial.println(data);
    data -= this->_offset;
    // Serial.print("data3 : "); Serial.println(data);
    ret = (float)data / this->_scale;

    return ret;
}

bool HX711::is_ready(void)
{
    return (digitalRead(this->_dout) == LOW);
}

void HX711::set_gain(uint8_t gain)
{
    switch(gain)
    {
        case 128:
            this->_gain = 1;
            break;
        case 32:
            this->_gain = 2;
            break;
        case 64:
            this->_gain = 3;
            break;
        default:
            char* temp;
            sprintf(temp, "%d gain isn't valid\nSetting default gain...", gain);
            Serial.println(temp);
            this->_gain = 1;
            break;
    }
}

void HX711::set_scale(float scale)
{
    this->_scale = scale;
}

void HX711::set_offset(long offset)
{
    this->_offset = offset;
}

float HX711::get_scale(void)
{
    return this->_scale;
}

long HX711::get_offset(void)
{
    return this->_offset;
}

void HX711::power_up(void)
{
    digitalWrite(this->_sck, LOW);
}

void HX711::power_down(void)
{
    digitalWrite(this->_sck, LOW);
    digitalWrite(this->_sck, HIGH);
}