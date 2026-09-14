/*
 * drv8323.h
 *
 *  Created on: 7 wrz 2026
 *      Author: LH213
 */

#ifndef APPLICATION_USER_DRV8323_H_
#define APPLICATION_USER_DRV8323_H_

#include <stdint.h>
// drv8323.h
#define DRV8323_REG_FAULT_STATUS1   0x00
#define DRV8323_REG_FAULT_STATUS2   0x01
#define DRV8323_REG_DRIVER_CTRL     0x02
#define DRV8323_REG_GATE_HS         0x03
#define DRV8323_REG_GATE_LS         0x04
#define DRV8323_REG_OCP_CTRL        0x05
#define DRV8323_REG_CSA_CTRL        0x06

// bit "R/W" = bit15 (1=read), adres = bit14:11, dane = bit10:0
uint16_t DRV8323_ReadReg(uint8_t addr);
void     DRV8323_WriteReg(uint8_t addr, uint16_t data);
void     DRV8323_Init(void);
uint16_t DRV8323_ClearFaults(void);

#endif /* APPLICATION_USER_DRV8323_H_ */
