/*****************************************************************************/
/*
* OLED control in FSBL is only supported if both SPI and GPIO devices are PS.
*
* If that is the case, the user should define IS_PS_DEVICE as 1, otherwise
* IS_PS_DEVICE should be set to 0 so the FSBL will skip OLED loadup during boot.
*
* Make sure the SPI and GPIO constants are defined correctly based on pinout.
*
* SH1122 256x64 16-level grayscale driver.
* Framebuffer: 128 bytes/row * 64 rows = 8192 bytes.
* Each byte encodes two horizontally adjacent pixels (high nibble = left, low nibble = right).
*
******************************************************************************/
#ifndef MACHINE_OLED_H
#define MACHINE_OLED_H

#include <stdbool.h>
#include "xspips.h"
#include "xgpiops.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "sleep.h"
#include "sh1122.h"

// 0 by default - oled will NOT startup in the fsbl if 0
#define IS_PS_DEVICE 1  // 1 - oled gpio and spi pins connected to ps, 0 - connected to pl

// SPI Constant Definitions
#define SPI_DEVICE_ID        XPAR_XSPIPS_0_BASEADDR
#define SPI_CLOCK_PRESCALER  XSPIPS_CLK_PRESCALE_64  // keep SPI clock well under 16 MHz max
#define SPI_CHIP_SELECT      0

// GPIO Pin Definitions  (MIO pin numbers — adjust to match your board)
#define GPIO_RESETn          27   // SH1122 /RES
#define GPIO_DATA_CONTROL    28   // SH1122 D/C#

// SH1122 D/C# levels
#define COMMAND_WRITE        0
#define DATA_WRITE           1

// Function prototypes
void load_splash_screen(void);
bool gpio_init(void);
bool spi_init(void);
void gpio_write(int pin, int value);
int  sh1122_send_data(unsigned char *data, int ByteCnt);
int  sh1122_send_command(unsigned long cmd, int ByteCnt);

#endif /* MACHINE_OLED_H */
