/*
 * drv8323.c
 *
 *  Created on: 7 wrz 2026
 *      Author: LH213
 */
// drv8323.c
#include "drv8323.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2;

static uint16_t DRV8323_Transfer(uint16_t txFrame)
{
    uint16_t rxFrame = 0;
    uint8_t tx[2] = { (uint8_t)(txFrame >> 8), (uint8_t)(txFrame & 0xFF) };
    uint8_t rx[2] = {0};
    HAL_Delay(1);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    volatile int status= HAL_SPI_TransmitReceive(&hspi2, tx, rx, 2, 100);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    rxFrame = ((uint16_t)rx[0] << 8) | rx[1];
    return rxFrame;
}

void DRV8323_WriteReg(uint8_t addr, uint16_t data)
{
    uint16_t frame = ((uint16_t)0 << 15) | ((uint16_t)(addr & 0x0F) << 11) | (data & 0x07FF);
    (void)DRV8323_Transfer(frame);
}

uint16_t DRV8323_ReadReg(uint8_t addr)
{
    uint16_t cmdFrame = ((uint16_t)1 << 15) | ((uint16_t)(addr & 0x0F) << 11);

   // uint16_t dummyFrame = 0; // może być 0, albo np. write do jakiegoś bezpiecznego rejestru
   // uint8_t tx[2] = { (uint8_t)(cmdFrame >> 8), (uint8_t)(cmdFrame & 0xFF) };

    // 2. Wyślij ramkę „dummy” – odpowiedź zawiera dane z żądanego rejestru
    uint16_t rx = DRV8323_Transfer(cmdFrame);

    return (rx & 0x07FF); // tylko 11 bitów danych
}

void DRV8323_Init(void)
{
    HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, GPIO_PIN_SET);
    HAL_Delay(2); /* czas budzenia driver'a (>1ms wg datasheet) */

    /* Driver Control reg 0x02: tryb 6xPWM, dead-time z DRV, brak OTW enable itd. */
    DRV8323_WriteReg(DRV8323_REG_DRIVER_CTRL,
                      (0U << 5)   /* PWM_MODE = 00 -> 6x PWM (INHx/INLx niezależne) */
                    | (1U << 2)  /* DEAD_TIME wybór - patrz pkt 3 niżej          */
                    );

    /* OCP_CTRL, CSA_CTRL wg wymaganego trybu prądowego/VDS itd. */
}

