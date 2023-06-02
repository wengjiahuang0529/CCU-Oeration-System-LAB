/***************************************************************************//**
 * @file     targetdev.h
 * @brief    ISP support function header file
 * @version  0x31
 *
 * @note
 * Copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "NUC100Series.h"
#include "uart_transfer.h"
#include "ISP_USER.h"

/* rename for uart_transfer.c */
#define UART_N					UART0
#define UART_N_IRQHandler		UART02_IRQHandler
#define UART_N_IRQn				UART02_IRQn

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
