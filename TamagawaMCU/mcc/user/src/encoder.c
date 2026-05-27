#include "mcc\user\encoder.h"
#include "mcc\mcc_generated_files\timer\delay.h"
#include "mcc\mcc_generated_files\uart\uart2.h"


ENCODER encoder;

int Encoder_GetDataSize(ENCODER *encoder){
    if(encoder == NULL || encoder ->addr == 0x00)
        return -1;
    return encoder->mtSize + encoder->stSize + 2;//ERR+WARN+CRC = 8
}

#define DELAY_NOPS(n) \
    do { \
        for(int i = 0; i < (n); i++) { __asm("nop"); } \
    } while(0)

// 用法：DELAY_NOPS(8);  // 执行8次nop


/*static void Delay_xuS(uint16_t x)               //实现微秒延时
{
    DELAY_microseconds(x);
}*/



static uint32_t Encoder_ReadData_Bits(ENCODER *encoder, uint16_t length)
{
    uint32_t data = 0;
    

    for(int bitIndex = 0; bitIndex < length; bitIndex++){
        volatile uint8_t dataIn = 0;
        
        //给出读取时钟，采用SSI总线
        ENCODER_SET_CLOCK_PIN();
        DELAY_NOPS(10);
        //Delay_xuS(ENCODER_CLOCK_TIME);
        ENCODER_CLR_CLOCK_PIN();
        dataIn = (uint8_t) ENCODER_GET_DATA_FROM();
        DELAY_NOPS(10);
        //Delay_xuS(ENCODER_CLOCK_TIME);

        data = (data << 1) | dataIn;
    }
    //uint32_t data = Encoder_ReadData_Bits(&encoder, length);
    //UART2_Drv.Write((data >> 24) & 0xFF);
    //UART2_Drv.Write((data >> 16) & 0xFF);
    //UART2_Drv.Write((data >> 8) & 0xFF);
    //UART2_Drv.Write(data & 0xFF);

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
    DELAY_NOPS(15);
    //Delay_xuS(ENCODER_CLOCK_PRELOW_TIME);

    uint32_t mtData = Encoder_ReadData_Bits(encoder, encoder->mtSize*8);
    //uint32_t mtData = Encoder_ReadData_Bits(encoder, 12);
    uint32_t stData = Encoder_ReadData_Bits(encoder, encoder->stSize*8);
    //uint32_t stData = Encoder_ReadData_Bits(encoder, 19);
    uint8_t err_warn_crcData = Encoder_ReadData_Bits(encoder, 0);
    DELAY_NOPS(20);
    //Delay_xuS(ENCODER_CLOCK_POSTHIGH_TIME);

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
    en->addr = en;
}