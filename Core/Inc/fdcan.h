#ifndef __FDCAN_H__
#define __FDCAN_H__

#include "main.h"

extern FDCAN_HandleTypeDef hfdcan1;

void MX_FDCAN1_Init(void);

void FDCAN1_FilterSet(uint32_t filterCAN_ID1,
	uint32_t filterCAN_ID2);
void FDCAN1_ReceiveMessage(uint8_t *RxData,
						   uint32_t *len);
void FDCAN1_TransmitMessage(uint32_t CAN_ID,
							uint8_t *data,
							uint32_t len);
 
#endif /* __FDCAN_H__ */
