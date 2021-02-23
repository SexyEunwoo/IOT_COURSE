#ifndef __PM7003_H_
#define __PM7003_H_

#include <stdint.h>

typedef struct _SData
{
    // Standard Particles, CF=1
    uint16_t _PM_SP_UG_1_0;
    uint16_t _PM_SP_UG_2_5;
    uint16_t _PM_SP_UG_10_0;

    // Atmospheric environment
    uint16_t _PM_AE_UG_1_0;
    uint16_t _PM_AE_UG_2_5;
    uint16_t _PM_AE_UG_10_0;
} SData;

class PM7003
{
public:
    PM7003(Stream&);
    void sleep(void);
    void wakeup(void);
    void passive(void);
    void active(void);
    void read(void);
    void request_read(void);
    uint16_t calc_verify_byte(void);

private:
    enum HEADER {
                    HEADER1,
                    HEADER2,
                    FRAME_LEN_HIGH,
                    FRAME_LEN_LOW
    };
    enum CMD {
                    CHANGE_MODE = 0xe1,
                    READ_IN_PASSIVE = 0xe2,
                    SLEEP_SET = 0xe4
    };
    enum CMD_DATAL {
                    SLEEP, MODE_PASSIVE = 0,
                    WAKEUP,
                    MODE_ACTIVE = 1
    };
    enum CMD_IDX {
                    IDX_HEADER1,
                    IDX_HEADER2,
                    IDX_CMD,
                    IDX_DATAH,
                    IDX_DATAL ,
                    IDX_LRCH,
                    IDX_LRCL
    };
    enum DATA_IDX {
        PM_SP_UG_1_0_HIGH = 4,
        PM_SP_UG_1_0_LOW,
        PM_SP_UG_2_5_HIGH,
        PM_SP_UG_2_5_LOW,
        PM_SP_UG_10_HIGH,
        PM_SP_UG_10_LOW,
        PM_AE_UG_1_0_HIGH,
        PM_AE_UG_1_0_LOW,
        PM_AE_UG_2_5_HIGH,
        PM_AE_UG_2_5_LOW,
        PM_AE_UG_10_HIGH,
        PM_AE_UG_10_LOW
    };
    enum STATUS {
        PASSIVE,
        ACTIVE
    };
    STATUS _status;
    const int DATA_LEN = 32;
    const int BAUD = 9600;

    struct _SData _data;
    Stream* _stream;

    uint8_t _arrCommand[7];
    uint8_t _arrData[32];
    bool _bSuccessLast;
    bool _bSuccessCur;
};

#endif