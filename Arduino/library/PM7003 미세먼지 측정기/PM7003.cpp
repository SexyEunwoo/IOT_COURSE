#include "PM7003.h"

PM7003::PM7003(Stream& _stream)
{
    this->_stream = _stream;
    this->_stream.begin(BAUD);
    this->_status = ACTIVE;
    this->_bSuccess = false;
}

void PM7003::sleep(void)
{
    this->_arrCommand[IDX_CMD] = SLEEP_SET;
    this->_arrCommand[IDX_DATAL] = SLEEP;

    uint16_t verifyData = calc_verify_byte();
    this->_arrCommand[IDX_LRCH] = verifyData >> 8;
    this->_arrCommand[IDX_LRCL] = verifyData;

    this->_stream.write(this->_arrCommand, sizeof(this->_arrCommand));
}

void PM7003::wakeup(void)
{
    this->_arrCommand[IDX_CMD] = SLEEP_SET;
    this->_arrCommand[IDX_DATAL] = WAKEUP;

    uint16_t verifyData = calc_verify_byte();
    this->_arrCommand[IDX_LRCH] = verifyData >> 8;
    this->_arrCommand[IDX_LRCL] = verifyData;

    this->_stream.write(this->_arrCommand, sizeof(this->_arrCommand));
}

void PM7003::passive(void)
{
    this->_arrCommand[IDX_CMD] = CHANGE_MODE;
    this->_arrCommand[IDX_DATAL] = MODE_PASSIVE;

    uint16_t verifyData = calc_verify_byte();
    this->_arrCommand[IDX_LRCH] = verifyData >> 8;
    this->_arrCommand[IDX_LRCL] = verifyData;

    this->_stream.write(this->_arrCommand, sizeof(this->_arrCommand));
}

void PM7003::active(void)
{
    this->_arrCommand[IDX_CMD] = CHANGE_MODE;
    this->_arrCommand[IDX_DATAL] = MODE_ACTIVE;

    uint16_t verifyData = calc_verify_byte();
    this->_arrCommand[IDX_LRCH] = verifyData >> 8;
    this->_arrCommand[IDX_LRCL] = verifyData;

    this->_stream.write(this->_arrCommand, sizeof(this->_arrCommand));
}

bool PM7003::read(void)
{
    this->_bSuccessLast = this->_bSuccessCur;
    this->_bSuccessCur = false;
    bool ret = false;
    int idx = 0;
    int verifyData1 = -1, verifyData2 = -1;

    if (this->_stream.available())
    {
        while (this->_stream.available())
        {
            this->_arrData[idx] = this->_stream.read();

            switch (idx)
            {
            case IDX_HEADER1:
                if (this->_arrData[idx] == HEADER1)
                {
                    idx++;
                }
                break;
            case IDX_HEADER2:
                if (this->_arrData[idx] == HEADER2)
                {
                    idx++;
                }
                break;
            default:
                idx++;
                break;
            }

            if (idx >= DATA_LEN)
            {
                verifyData2 = (this->_data[DATA_LEN - 2] << 8) + this->_data[DATA_LEN - 1];
                break;
            }
        }

        for (int i = 0; i < DATA_LEN - 2; i++)
        {
            verifyData += this->_data[i];
        }

        if (verifyData1 == verifyData2)
        {
            this->_data._PM_SP_UG_1_0 = (this->_arrData[PM_SP_UG_1_0_HIGH] << 8) + (this->_arrData[PM_SP_UG_1_0_LOW]);
            this->_data._PM_SP_UG_2_5 = (this->_arrData[PM_SP_UG_2_5_HIGH] << 8) + (this->_arrData[PM_SP_UG_2_5_LOW]);
            this->_data._PM_SP_UG_10 = (this->_arrData[PM_SP_UG_10_HIGH] << 8) + (this->_arrData[PM_SP_UG_10_LOW]);

            this->_data._PM_AE_UG_1_0 = (this->_arrData[PM_AE_UG_1_0_HIGH << 8]) + (thus->_arrData[PM_AE_UG_1_0_LOW]);
            this->_data._PM_AE_UG_2_5 = (this->_arrData[PM_AE_UG_2_5_HIGH << 8]) + (thus->_arrData[PM_AE_UG_2_5_LOW]);
            this->_data._PM_AE_UG_10 = (this->_arrData[PM_AE_UG_10_HIGH << 8]) + (thus->_arrData[PM_AE_UG_10_LOW]);

            this->_bSuccessCur = true;
            return ret;
        }
        else
        {
            this->_stream.println("Verify Data Dismatch! \n");
            this->_bSuccessCur = false;
            return ret;
        }
    }
    else
    {
        this->_stream.println("No Available Data! \n");
        this->_bSuccessCur = false;
        return ret;
    }

}

bool PM7003::request_read()
{
    bool ret = false;
    if(this->_status == ACTIVE)
    {
        this->_arrCommand[IDX_CMD] = READ_IN_PASSIVE;
        this->_stream.write(this->_arrCommand, sizeof(this->_arrCommand));
        ret = read();
    }

    return ret;
}

uint16_t PM7003::calc_verify_byte(void)
{
    uint16_t verifyData = 0;

    if(this->_status == ACTIVE)
    {
        for(int i = 0; i < 30; i++)
        {
            verifyData += this->_arrData[i];
        }
    }
    else if(this->_status == PASSIVE)
    {
        for(int i = 0; i < 5; i++)
        {
            verifyData += this->_arrCommand[i];
        }
    }

    return verifyData;
}