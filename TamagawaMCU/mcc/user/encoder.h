#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "xc.h"
#include "libpic30.h"
#include "D:\Project Docs\VS Code PRJ\Git Remote Works\Encoder_Tamagawa-Dev\TamagawaMCU\mcc\mcc_generated_files\system\pins.h"

#define ENCODER_SET_CLOCK_PIN()   MA_SetHigh()
#define ENCODER_CLR_CLOCK_PIN()   MA_SetLow()
#define ENCODER_GET_DATA_FROM()   SLO_GetValue()

#define ENCODER_DATA_MAX_LENGTH 10

#define ENCODER_CLOCK_PRELOW_TIME 0
#define ENCODER_CLOCK_TIME 0
#define ENCODER_CLOCK_POSTHIGH_TIME 2


typedef struct ENCODER_T{
    struct ENCODER_T *addr; // 用于判断是否初始化

    uint8_t mtSize;              // 累计运行圈数 位长度
    uint8_t stSize;              // 绝对位置数据 位长度

    uint8_t data[ENCODER_DATA_MAX_LENGTH];             // 数据存储 最大长度为80位
} ENCODER; //编码器对象，参数和数据都在此存储


extern ENCODER encoder;

int Encoder_GetDataSize(ENCODER *encoder);

/**
 * @brief 读取编码器数据，返回值将会保存在encoder.data中，传入未初始化的encoder对象会直接返回
 * 
 * @param encoder 
 * @return 无
 */
void Encoder_Read_Data(ENCODER *encoder);



//////// 项目代码 ////////////
void Encoder_init(ENCODER *en);
//////////////////////////////

#endif // ENCODER_H
