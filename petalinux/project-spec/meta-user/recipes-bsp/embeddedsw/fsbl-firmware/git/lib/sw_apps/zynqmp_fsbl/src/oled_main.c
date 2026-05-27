/*****************************************************************************/
/*
* This file controls an SH1122 256x64 OLED during ZynqMP FSBL startup.
*
* GPIO and SPI pin configurations are in oled_main.h.
* Custom splash screen content goes in sh1122.c (splash_buffer).
*
* Standard setup sequence:
*
*   sh1122Init();                   // Sets all SH1122 basic registers
*   sh1122FlipScreen(1);            // Sets proper screen orientation
*   sh1122TurnDisplayOn(1);         // Turns on the display
*   sh1122ClearScreen();            // Clears screen
*   sh1122LoadSplashScreen();       // Loads splash screen into staging buffer
*   sh1122UpdateStagingScreen();    // Renders staging buffer onto OLED
*
* oled_main.c       // platform: ZynqMP SPI/GPIO
*       ↑
* sh1122_internal.c // raw register commands
*       ↑
* sh1122.c          // buffers, fonts, rendering
*       ↑
* load_splash_screen() in oled_main.c
*
******************************************************************************/
#include "oled_main.h"

// Variable definitions
XGpioPs Gpio;
XSpiPs  SpiInstance;

void load_splash_screen()
{
    if (!(gpio_init() && spi_init())) {
        return;
    }

    // SH1122 setup
    sh1122Init();
    //sh1122FlipScreen(1);   /* remove call and function if orientation is fixed */
    sh1122ClearScreen();

    sh1122TurnDisplayOn(1);

    usleep(100000);  // 100ms recommended startup sequence

    // Load and display splash screen directly — no text overlay
    sh1122LoadSplashScreen();
    sh1122UpdateStagingScreen();
}

bool gpio_init()
{
    if (!IS_PS_DEVICE)
        return false;

    XGpioPs_Config *ConfigPtr;

    ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_BASEADDR);
    if (ConfigPtr == NULL) {
        xil_printf("FSBL Hook: GPIO Lookup Failed!\n");
        return false;
    }

    if (XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr) != XST_SUCCESS) {
        xil_printf("FSBL Hook: GPIO Init Failed!\n");
        return false;
    }

    u8 GPIOs[2] = {GPIO_RESETn, GPIO_DATA_CONTROL};
    for (int i = 0; i < 2; i++) {
        XGpioPs_SetDirectionPin(&Gpio, GPIOs[i], 1);
        XGpioPs_SetOutputEnablePin(&Gpio, GPIOs[i], 1);
    }

    // Hardware reset: pulse /RES low
    XGpioPs_WritePin(&Gpio, GPIO_RESETn, 0);
    usleep(10000);
    XGpioPs_WritePin(&Gpio, GPIO_RESETn, 1);
    usleep(10000);

    XGpioPs_WritePin(&Gpio, GPIO_DATA_CONTROL, COMMAND_WRITE);

    return true;
}

bool spi_init()
{
    if (!IS_PS_DEVICE)
        return false;

    XSpiPs_Config *SpiConfig;

    SpiConfig = XSpiPs_LookupConfig(SPI_DEVICE_ID);
    if (SpiConfig == NULL) {
        xil_printf("FSBL Hook: SPI Lookup Failed!\n");
        return false;
    }

    int Status = XSpiPs_CfgInitialize(&SpiInstance, SpiConfig, SpiConfig->BaseAddress);
    if (Status != XST_SUCCESS) {
        xil_printf("FSBL Hook: SPI Initialization Failed\n");
        return false;
    }

    XSpiPs_SetOptions(&SpiInstance, XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION);
    XSpiPs_SetClkPrescaler(&SpiInstance, SPI_CLOCK_PRESCALER);
    XSpiPs_SetSlaveSelect(&SpiInstance, SPI_CHIP_SELECT);
    XSpiPs_Enable(&SpiInstance);

    return true;
}

void gpio_write(int pin, int value)
{
    XGpioPs_WritePin(&Gpio, pin, value);
}

/*
 * sh1122_send_command()
 *
 * Sends 1-3 byte commands. The cmd argument is packed MSB-first into
 * the low bytes of an unsigned long; bytes are reversed before transmission.
 */
int sh1122_send_command(unsigned long cmd, int ByteCnt)
{
    char *buf = (char *)&cmd;
    char  c;
    int   i;

    gpio_write(GPIO_DATA_CONTROL, COMMAND_WRITE);

    for (i = 0; i < ByteCnt / 2; ++i) {
        c = buf[i];
        buf[i] = buf[ByteCnt - i - 1];
        buf[ByteCnt - i - 1] = c;
    }

    int Status = XSpiPs_PolledTransfer(&SpiInstance, buf, NULL, ByteCnt);
    if (Status != XST_SUCCESS) {
        xil_printf("SPI transfer is busy!\r\n");
        return XST_DEVICE_BUSY;
    }

    return XST_SUCCESS;
}

/*
 * sh1122_send_data()
 *
 * Sends raw pixel/data bytes to the SH1122 GDDRAM.
 */
int sh1122_send_data(unsigned char *data, int ByteCnt)
{
    gpio_write(GPIO_DATA_CONTROL, DATA_WRITE);

    for (int i = 0; i < ByteCnt; ++i) {
        char buf[1];
        buf[0] = data[i];
        int Status = XSpiPs_PolledTransfer(&SpiInstance, buf, NULL, 1);
        if (Status != XST_SUCCESS) {
            xil_printf("SPI transfer is busy!\r\n");
            return XST_DEVICE_BUSY;
        }
    }

    return XST_SUCCESS;
}
