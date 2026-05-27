#ifndef TAMAGAWA_H
#define TAMAGAWA_H

#include "xc.h"
#include <stdint.h>

/**
 *    @brief    Data ID codes
 */
enum DATA_ID
{
    DATA_ID_0 = 0b0,  /**< Data readout data in one revolution */
    DATA_ID_1 = 0b10001,  /**< Data readout multi-turn data */
    DATA_ID_2 = 0b10010,  /**< Data readout encoder ID */
    DATA_ID_3 = 0b00011,  /**< Data readout data in one revolution, encoder ID, multi-turn, encoder error */
    DATA_ID_6 = 0b00110,  /**< EEPROM write */
    DATA_ID_7 = 0b10111,  /**< Reset */
    DATA_ID_8 = 0b11000,  /**< Reset */
    DATA_ID_C = 0b01100,  /**< Reset */
    DATA_ID_D = 0b11101,  /**< EEPROM read */
    DATA_ID_SINKCODE = 0b010
};


/**
 * @brief Tamagawa Interface Transmit data
 */
struct rx
{
    uint8_t  cf;    /**< Control Frame */
    uint8_t  adf;   /**< EEPROM address */
    uint8_t  edf;   /**< EEPROM data */
    uint8_t  crc;   /**< CRC */
};

/**
 * @brief Tamagawa Interface Received data
 */
struct tx
{
    uint8_t abs[3];   /**< Data in one revolution */
    uint8_t abm[3];   /**< Multi-turn Data */
    uint8_t  cf;    /**< Control Frame */
    uint8_t  sf;    /**< Status Frame */
    uint8_t  enid;  /**< Encoder ID */
    uint8_t  almc;  /**< Encoder error */
    uint8_t  adf;   /**< EEPROM address */
    uint8_t  edf;   /**< EEPROM data */
    uint8_t  crc;   /**< CRC */
};

typedef struct TamagawaInterface_T
{
    uint8_t status;   /**< Status 0:Idle, 1:Receive 2:Ready-to-send */
    uint8_t data_id;  /**< Data ID code */
    uint8_t receiveSeq;
    struct tx tx;      /**< Transmit data */
    struct rx rx;      /**< Received data */
}TamagawaInterface;

typedef struct EncoderData_T
{
    uint8_t ALMC;    /**< Error code */
    uint32_t ABS;  /**< Data in one revolution; */
    uint32_t ABM;  /**< Multi-turn data; */
}EncoderData;

// ALMC reg error bit masks
#define SET_ENCODER_ERROR_OS(code) code|(1<<0)
#define SET_ENCODER_ERROR_FS(code) code|(1<<1)
#define SET_ENCODER_ERROR_CE(code) code|(1<<2)
#define SET_ENCODER_ERROR_OF(code) code|(1<<3)
#define SET_ENCODER_ERROR_ME(code) code|(1<<5)
#define SET_ENCODER_ERROR_BE(code) code|(1<<6)
#define SET_ENCODER_ERROR_BA(code) code|(1<<7)

#define CLEAR_ENCODER_ERROR_OS(code) code&(~(1<<0))
#define CLEAR_ENCODER_ERROR_FS(code) code&(~(1<<1))
#define CLEAR_ENCODER_ERROR_CE(code) code&(~(1<<2))
#define CLEAR_ENCODER_ERROR_OF(code) code&(~(1<<3))
#define CLEAR_ENCODER_ERROR_ME(code) code&(~(1<<5))
#define CLEAR_ENCODER_ERROR_BE(code) code&(~(1<<6))
#define CLEAR_ENCODER_ERROR_BA(code) code&(~(1<<7))
#define CLEAR_ENCODER_ERROR(code) code&0



extern TamagawaInterface ti;
extern EncoderData encoderData;
extern uint8_t isUartReceived;
extern uint8_t uartReceiveData;

void Tamagawa_Receive(uint8_t frameData);
void Tamagawa_Process();
void Tamagawa_Send();

#endif