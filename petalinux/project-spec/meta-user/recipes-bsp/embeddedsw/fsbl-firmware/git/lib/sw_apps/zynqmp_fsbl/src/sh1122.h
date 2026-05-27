#ifndef SH1122_H
#define SH1122_H

#include <stdbool.h>
#include "sh1122_internal.h"

/* -----------------------------------------------------------------------
 * Display geometry
 * ----------------------------------------------------------------------- */
#define DISPLAY_WIDTH       256
#define DISPLAY_HEIGHT      64
#define DISPLAY_BYTE_WIDTH  (DISPLAY_WIDTH / 2)   // 128 bytes per row
#define DISPLAY_BUF_SIZE    (DISPLAY_BYTE_WIDTH * DISPLAY_HEIGHT)  // 8192

/* Gray levels (4-bit, 0 = off, 15 = full brightness) */
#define SH1122_GRAY_OFF     0
#define SH1122_GRAY_HALF    8
#define SH1122_GRAY_MAX     15

struct sh1122_dev {
    unsigned char displayBuffer[DISPLAY_BUF_SIZE];
    unsigned char stagingBuffer[DISPLAY_BUF_SIZE];
    unsigned char cleanBuffer[DISPLAY_BUF_SIZE];

    int prevRowAddr_low;
    int prevRowAddr_high;
    int prevColumnAddr_low;
    int prevColumnAddr_high;

    // Dirty rectangle (in byte-column units)
    int upd_x;
    int upd_y;
    int upd_w;
    int upd_h;

    int rotated;
};

typedef enum {
    OP_REPLACE = 1,
    OP_OR      = 2,
} bitmap_op_t;

extern const unsigned char splash_buffer[DISPLAY_BUF_SIZE];

/* High-level API */
void sh1122Init(void);
void sh1122FlipScreen(unsigned int flip);   /* remove if orientation is fixed */
void sh1122TurnDisplayOn(unsigned int display_on);
void sh1122ClearScreen(void);
void sh1122LoadSplashScreen(void);
void sh1122UpdateStagingScreen(void);
void sh1122CalcUpdateRect(void);
void sh1122Render(void);
void sh1122RenderAll(void);

/* Buffer helpers */
void sh1122UpdateStagingBuffer(const unsigned char *buffer, unsigned int x,
                                unsigned int row, unsigned int w);
void sh1122UpdateStagingBufferPix(const unsigned char *buffer, unsigned int x,
                                   unsigned int y, unsigned int w,
                                   bitmap_op_t op);

#endif /* SH1122_H */
