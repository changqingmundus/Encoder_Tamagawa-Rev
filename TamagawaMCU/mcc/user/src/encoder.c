#include "encoder.h"
#include "timer/delay.h"
#include "uart/uart1.h"
#include "system/pins.h"

ENCODER encoder;

uint32_t encoder_zero_ABS = 0; // 注意无static

int Encoder_GetDataSize(ENCODER *encoder){
    if(encoder == NULL || encoder ->addr == 0x00)
        return -1;
    return encoder->mtSize + encoder->stSize + 1;//ERR+WARN+CRC = 8
}

static void Delay_xuS(uint16_t x)               //实现微秒延时
{
    DELAY_microseconds(x);
}



static uint32_t Encoder_ReadData_Bits(ENCODER *encoder, uint16_t length)
{
    uint32_t data = 0;
    

    for(int bitIndex = 0; bitIndex < length; bitIndex++){
        volatile uint8_t dataIn = 0;
        
        //给出读取时钟，采用SSI总线
        ENCODER_SET_CLOCK_PIN();
        Delay_xuS(ENCODER_CLOCK_TIME);
        ENCODER_CLR_CLOCK_PIN();
        dataIn = (uint8_t) ENCODER_GET_DATA_FROM();
        Delay_xuS(ENCODER_CLOCK_TIME);

        data = (data << 1) | dataIn;
    }

    return data;
}


void Encoder_Read_Data(ENCODER *encoder)
{
    /*if(encoder->addr == 0x00)
        return;

    for (int i = 0; i < ENCODER_DATA_MAX_LENGTH; i++)
        encoder->data[i] = 0x00;*/

    //启动通讯
    ENCODER_CLR_CLOCK_PIN();
    Delay_xuS(ENCODER_CLOCK_PRELOW_TIME);

    uint32_t mtData = Encoder_ReadData_Bits(encoder, encoder->mtbit);
    uint32_t stData = Encoder_ReadData_Bits(encoder, encoder->stbit);
    uint8_t err_warn_crcData = Encoder_ReadData_Bits(encoder, 8);
    Delay_xuS(ENCODER_CLOCK_POSTHIGH_TIME);

    ENCODER_SET_CLOCK_PIN();

    for(int i = encoder->mtSize - 1; i >= 0; i--){
        encoder->data[i] = mtData & 0xFF;
        mtData >>= 8;
    }
    for(int i = encoder->stSize - 1; i >= 0; i--){
        encoder->data[encoder->mtSize + i] = stData & 0xFF;
        stData >>= 8;
    }
    encoder->data[encoder->mtSize + encoder->stSize] = err_warn_crcData;


    return;

}


void Encoder_init(ENCODER *en){
    //if (en == NULL) return;
    en->mtSize = 2;
    en->stSize = 3;
    
    en->mtbit = 12;
    en->stbit = 19;
    en->addr = en;

    Encoder_LoadZeroABS();
}

void Encoder_Clear_Data(void) {
    Encoder_Read_Data(&encoder); // 先读取一次数据，确保 encoder.data 中有实时的 ABS 数据
    // 先拆解当前 encoder.data[]，计算 ABS
    int32_t abs = 0;
    for (int i = 0; i < encoder.stSize; i++) {
        abs |= ((uint32_t)encoder.data[encoder.mtSize + i]) << (8 * (encoder.stSize - 1 - i));
    }
    encoder_zero_ABS = abs;
}

void Encoder_LoadZeroABS(void)
{
    uint16_t lowbyte;
    uint16_t highbyte;

    DEE_Read(DEE_ENCODER_ZERO_L, &lowbyte);
    DEE_Read(DEE_ENCODER_ZERO_H, &highbyte);
    
    encoder_zero_ABS = ((uint32_t)highbyte << 16) | lowbyte;
    if(encoder_zero_ABS == 0xFFFFFFFF)
    {
        encoder_zero_ABS = 0;
    }
}