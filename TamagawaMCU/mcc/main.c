/*
? [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "dee/dee.h"
#include "system/system.h"
#include "encoder.h"
#include "tamagawa.h"
#include "cleardata.h"

/*
    Main application
*/

#include "timer/sccp1.h"
#include <stdint.h>

uint8_t test_triggered = 0;
uint8_t test_Data = 0b10101100;
uint8_t uartReceiveData = 0;

int main(void)
{
    SYSTEM_Initialize();
    DEE_Init();
    Encoder_init(&encoder);
    SET_SetInterruptHandler(ClearData_CN_Callback);
    Timer1_TimeoutCallbackRegister(ClearData_Timer_Callback);

    while(1)
    {
        if(isUartReceived){
            isUartReceived = 0;
            if(ti.status == 0){ // BUT only response when idle.
                // Ignore received data during transmit or processing
                Tamagawa_Receive(uartReceiveData);
            }
        }
        if(ti.status == 1){
            Encoder_Read_Data(&encoder);
            encoderData.ABS=0;
            encoderData.ABM=0;
            uint32_t abs_cur = 0;
            uint8_t dataIndex = 0;
            int32_t rel_abs = 0;
            for(dataIndex = 0; dataIndex<encoder.mtSize; dataIndex++){
                encoderData.ABM |= ((uint32_t)encoder.data[dataIndex]) << (8 * dataIndex);
            }
            if(encoder_zero_ABS == 0)
            {
                for(dataIndex = 0; dataIndex<encoder.stSize; dataIndex++){
                    rel_abs |= ((uint32_t)encoder.data[encoder.mtSize + dataIndex] << (8 * dataIndex));
                }
            }
            else
            {
            for (dataIndex = 0; dataIndex < encoder.stSize; dataIndex++) {
            abs_cur |= ((uint32_t)encoder.data[encoder.mtSize + dataIndex]) << (8 * (encoder.stSize - 1 - dataIndex));
            }
            rel_abs = abs_cur - encoder_zero_ABS;
            if (rel_abs < 0) rel_abs += ABS_MAX_VALUE + 1;
            rel_abs = ((rel_abs & 0x0000FF) << 16) | (rel_abs & 0x00FF00) | ((rel_abs & 0xFF0000) >> 16);
            }
            encoderData.ABS = rel_abs;
            Tamagawa_Process(&encoderData);
        }
        if(ti.status == 2){
            Tamagawa_Send();
        }
    }
}

//void SCCP1_TimeoutCallback (void) // For test purpose only
//{
//    if(ti.status == 0){ // BUT only response when idle.
//        // Ignore received data during transmit or processing
//        uint8_t readByte = 0b00011010;
//        isUartReceived = 1;
//        Tamagawa_Receive(readByte);
//    }
//}