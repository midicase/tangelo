#ifndef SH1122_INTERNAL_H
#define SH1122_INTERNAL_H

#include "oled_main.h"

/* Raw SH1122 command helpers */
void sh1122Cmd_SetColumnAddress(unsigned int col);
void sh1122Cmd_SetDisplayStartLine(unsigned int line);
void sh1122Cmd_SetContrast(unsigned int contrast);
void sh1122Cmd_SetSegmentRemap(unsigned char state);
void sh1122Cmd_EntireDisplayOn(unsigned char state);
void sh1122Cmd_SetInverseDisplay(unsigned char state);
void sh1122Cmd_SetMultiplexRatio(unsigned int ratio);
void sh1122Cmd_SetChargePump(unsigned char enable);
void sh1122Cmd_SetDisplayOn(unsigned int on);
void sh1122Cmd_SetRowAddress(unsigned int row);
void sh1122Cmd_SetRowRemap(unsigned char state);
void sh1122Cmd_SetDisplayOffset(unsigned int offset);
void sh1122Cmd_SetOscFrequency(unsigned int divider, unsigned int freq);
void sh1122Cmd_SetPreChargePeriod(unsigned int phase1, unsigned int phase2);
void sh1122Cmd_SetVCOMDeselectLevel(unsigned int level);
void sh1122Cmd_SetVSEGMLevel(unsigned int level);
void sh1122Cmd_SetVSL(void);
void sh1122Cmd_SendData(unsigned char *data, int bytecnt);

#endif /* SH1122_INTERNAL_H */
