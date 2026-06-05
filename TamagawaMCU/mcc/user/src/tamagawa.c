#include "tamagawa.h"
#include "uart/uart1.h"
#include "encoder.h"
#include <stdint.h>

TamagawaInterface ti;
EncoderData encoderData;
uint8_t isUartReceived = 0;

void UART1_RxCompleteCallback(void)
{ 
    isUartReceived = 1;
    uartReceiveData = UART1_Drv.Read();
}


static void Tamagawa_WriteB(uint8_t byte){
    UART1_Drv.Write(byte);
}


static void CRC(uint8_t *crc, uint8_t data)
{
    *crc^=data;
    uint8_t ploy=0x01;
    for(uint8_t i=0;i<8;i++){
        if(*crc & 0x80)
            *crc = (*crc<<1) ^ ploy;
        else
            *crc <<=1;
    }

    return;
}

static void Tamagawa_WriteB_CRC(uint8_t byte, uint8_t *crc){
    Tamagawa_WriteB(byte);
    CRC(crc, byte);
    
    return;
}

static void ResetReceive()
{
    ti.status = 0;
    ti.receiveSeq = 0;
    for(uint8_t i=0; i<sizeof(ti.rx); i++){
        ((uint8_t*)&ti.rx)[i] = 0x00;
    }
}

void Tamagawa_Receive(uint8_t frameData)
{
    if(ti.status != 0){
        return;
    }

    switch(ti.receiveSeq)
    {
        case 0:
            if((frameData & 0b111) != 0b010){
                ResetReceive();
            }else{
                ti.rx.cf = frameData;
                ti.data_id = (frameData & 0b11111000) >> 3;
            }

            if((ti.data_id == DATA_ID_0) 
            || (ti.data_id == DATA_ID_1)
            || (ti.data_id == DATA_ID_2)
            || (ti.data_id == DATA_ID_3)){
                ti.status = 1;
            }else{
                ti.receiveSeq = 1;
            }
            break;
        case 1:
            ti.rx.adf = frameData;
            ti.receiveSeq = 2;
            break;
        case 2:
            if(ti.data_id == DATA_ID_D){
                ti.rx.crc = frameData;
                ti.status = 1;
            }else if(ti.data_id == DATA_ID_6){
                ti.rx.edf = frameData;
                ti.receiveSeq = 3;
            }else{
                ResetReceive();
            }
            break;
        case 3:
            if(ti.data_id == DATA_ID_6){
                ti.rx.crc = frameData;
                ti.status = 1;
            }else{
                ResetReceive();
            }
            break;
    }

}

void Tamagawa_Process(EncoderData *encoderData)
{
    if(ti.status != 1){
        return;
    }

    ti.tx.cf = ti.rx.cf;

    // reset receive flags
    ResetReceive();

    switch (ti.data_id)
    {
        case DATA_ID_0:
        case DATA_ID_7:
        case DATA_ID_8:
        case DATA_ID_C:
            ti.tx.abs[2] = 0;

            ti.tx.abs[0] = encoderData->ABS & 0xFF;
            ti.tx.abs[1] = (encoderData->ABS >> 8) & 0xFF;
            ti.tx.abs[2] = (encoderData->ABS >> 16) & 0xFF;
            break;
        case DATA_ID_1: 
            ti.tx.abm[0] = encoderData->ABM & 0xFF;
            ti.tx.abm[1] = (encoderData->ABM >> 8) & 0xFF;
            ti.tx.abm[2] = 0;
            break;
        case DATA_ID_2:
            ti.tx.enid = 0x11;
            break;
        case DATA_ID_3:
            ti.tx.abs[2] = 0;

            ti.tx.abs[0] = encoderData->ABS & 0xFF;
            ti.tx.abs[1] = (encoderData->ABS >> 8) & 0xFF;
            ti.tx.abs[2] = (encoderData->ABS >> 16) & 0xFF;
            ti.tx.enid = 0x01;
            ti.tx.abm[0] = encoderData->ABM & 0xFF;
            ti.tx.abm[1] = (encoderData->ABM >> 8) & 0xFF;
            ti.tx.abm[2] = 0;
            break;        
        default:
            break;
    }
    
    ti.status = 2;
}

void Tamagawa_Send()
{
    if(ti.status != 2){
        return;
    }
    
    ti.tx.crc = 0;
    
    Tamagawa_WriteB_CRC(ti.tx.cf, &ti.tx.crc);
    switch(ti.data_id)
    {
        case DATA_ID_0:
            Tamagawa_WriteB_CRC(ti.tx.sf, &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[0], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[1], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[2], &ti.tx.crc);
            Tamagawa_WriteB(ti.tx.crc);
            break;
        case DATA_ID_1:
            Tamagawa_WriteB_CRC(ti.tx.sf, &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[0], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[1], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[2], &ti.tx.crc);
            Tamagawa_WriteB(ti.tx.crc);
            break;
        case DATA_ID_2:
            Tamagawa_WriteB_CRC(ti.tx.sf, &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.enid, &ti.tx.crc);
            Tamagawa_WriteB(ti.tx.crc);
            break;
        case DATA_ID_3:
            Tamagawa_WriteB_CRC(ti.tx.sf, &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[0], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[1], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[2], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.enid, &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[0], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[1], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[2], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.almc, &ti.tx.crc);
            Tamagawa_WriteB(ti.tx.crc);
            break;
        case DATA_ID_7:
        case DATA_ID_8:
        case DATA_ID_C:
            Tamagawa_WriteB_CRC(ti.tx.sf, &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[0], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[1], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[2], &ti.tx.crc);
            Tamagawa_WriteB(ti.tx.crc);
            break;
        default:
            Tamagawa_WriteB_CRC(ti.tx.sf, &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[0], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[1], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abs[2], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.enid, &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[0], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[1], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.abm[2], &ti.tx.crc);
            Tamagawa_WriteB_CRC(ti.tx.almc, &ti.tx.crc);
            Tamagawa_WriteB(ti.tx.crc);
            break;
    }
    // reset transmit flags
    ti.data_id = 0;
    ti.status = 0;
    for(uint8_t i=0; i<sizeof(ti.tx); i++){
        ((uint8_t*)&ti.tx)[i] = 0x00;//test code only, should be 0x00 in production
    }
}
