/* =========================================================================
 * SH1122 Command Implementations
 *
 * All multi-byte commands are sent as two SEPARATE single-byte SPI
 * transactions, matching the SH1122 datasheet and vendor reference code.
 * The controller expects each byte as an independent CS-framed transfer.
 *
 * Command bytes reference: SH1122 datasheet Rev 2.2
 * Command order follows the datasheet command table sequence.
 * ========================================================================= */
#include "sh1122_internal.h"


/* -------------------------------------------------------------------------
 * Column Address (Set Low/High Column Address)
 *
 * Sets the column address pointer. The SH1122 splits the 7-bit byte-column
 * address into two nibble commands:
 *   Low  nibble command: 0x00 | col[3:0]
 *   High nibble command: 0x10 | col[6:4]
 * col is a BYTE-column address (0-127), not a pixel address.
 * Each pixel column pair = 1 byte column.
 * Default: 0x00
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetColumnAddress(unsigned int col)
{
    col &= 0x7F;
    sh1122_send_command(0x00 | (col & 0x0F), 1);  /* low  nibble */
    sh1122_send_command(0x10 | (col >> 4),   1);  /* high nibble */
}

/* -------------------------------------------------------------------------
 * Set Display Start Line
 *
 * Command: 0xDC, followed by start line (0x00-0x3F)
 * Specifies the display RAM line mapped to COM0.
 * Used for vertical scrolling.
 * Default: 0x00
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetDisplayStartLine(unsigned int line)
{
    sh1122_send_command(0xDC, 1);
    sh1122_send_command(line & 0x3F, 1);
}

/* -------------------------------------------------------------------------
 * Set Contrast Control (Double Byte Command)
 *
 * Command: 0x81, followed by contrast value (0x00-0xFF)
 * Controls the segment output current and therefore display brightness.
 * Default: 0x80
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetContrast(unsigned int contrast)
{
    sh1122_send_command(0x81, 1);
    sh1122_send_command(contrast & 0xFF, 1);
}

/* -------------------------------------------------------------------------
 * Set Segment Re-Map (Column Address Direction)
 *
 * Command: 0xA0 (normal) or 0xA1 (mirror horizontal)
 * 0xA0 = column address 0 maps to SEG0  (normal)
 * 0xA1 = column address 0 maps to SEG255 (mirrored left-right)
 * Default: 0xA0
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetSegmentRemap(unsigned char state)
{
    sh1122_send_command(0xA0 | (state & 0x1), 1);
}

/* -------------------------------------------------------------------------
 * Set Entire Display ON/OFF
 *
 * Command: 0xA4 (output follows GDDRAM) or 0xA5 (all pixels ON)
 * 0xA4 = normal, display reflects GDDRAM content
 * 0xA5 = force all pixels ON regardless of GDDRAM
 * Default: 0xA4
 * ------------------------------------------------------------------------- */
void sh1122Cmd_EntireDisplayOn(unsigned char state)
{
    sh1122_send_command(0xA4 | (state & 0x1), 1);
}

/* -------------------------------------------------------------------------
 * Set Normal/Inverse Display
 *
 * Command: 0xA6 (normal) or 0xA7 (inverse)
 * 0xA6 = normal  — GDDRAM value 0 = pixel off
 * 0xA7 = inverse — GDDRAM value 0 = pixel on
 * Default: 0xA6
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetInverseDisplay(unsigned char state)
{
    sh1122_send_command(0xA6 | (state & 0x1), 1);
}

/* -------------------------------------------------------------------------
 * Set Multiplex Ratio (Double Byte Command)
 *
 * Command: 0xA8, followed by (ratio - 1) where ratio = 1..64
 * Sets the number of COM lines (display height in rows).
 * For a 64-row display: pass 64, sends 0x3F.
 * Default: 0x3F (64 MUX)
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetMultiplexRatio(unsigned int ratio)
{
    sh1122_send_command(0xA8, 1);
    sh1122_send_command((ratio - 1) & 0x3F, 1);
}

/* -------------------------------------------------------------------------
 * Set DC-DC Voltage Converter (Double Byte Command)
 *
 * Command: 0xAD, followed by:
 *   0x80 = DC-DC off (use external VPP)
 *   0x81 = DC-DC on, switch freq = DCLK/2
 *   0x82 = DC-DC on, switch freq = DCLK/3 (default)
 *   0x83 = DC-DC on, switch freq = DCLK/4
 *   0x8A = DC-DC off (alternate encoding per vendor code)
 *   0x8B = DC-DC on  (alternate encoding per vendor code)
 * Default: 0x82 (internal DC-DC, DCLK/3)
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetChargePump(unsigned char enable)
{
    sh1122_send_command(0xAD, 1);
    sh1122_send_command(enable ? 0x8B : 0x8A, 1);
}

/* -------------------------------------------------------------------------
 * Set Display ON/OFF
 *
 * Command: 0xAE (off) or 0xAF (on)
 * 0xAE = display off (sleep mode)
 * 0xAF = display on
 * Default after POR: 0xAE (off)
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetDisplayOn(unsigned int on)
{
    sh1122_send_command(on ? 0xAF : 0xAE, 1);
}

/* -------------------------------------------------------------------------
 * Set Row Address
 *
 * Command: 0xB0, followed by row number (0x00-0x3F)
 * Sets the row address pointer for subsequent GDDRAM writes.
 * Unlike SSD1306 page addressing, each row here is a single pixel row.
 * Default: 0x00
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetRowAddress(unsigned int row)
{
    sh1122_send_command(0xB0, 1);
    sh1122_send_command(row & 0x3F, 1);
}

/* -------------------------------------------------------------------------
 * Set Common Output Scan Direction
 *
 * Command: 0xC0 (normal) or 0xC8 (remapped/vertical flip)
 * 0xC0 = scan from COM0 to COM[N-1]  (normal)
 * 0xC8 = scan from COM[N-1] to COM0  (vertical mirror)
 * Default: 0xC0
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetRowRemap(unsigned char state)
{
    sh1122_send_command(state ? 0xC8 : 0xC0, 1);
}

/* -------------------------------------------------------------------------
 * Set Display Offset (Double Byte Command)
 *
 * Command: 0xD3, followed by offset value (0x00-0x3F)
 * Shifts the display output vertically by the specified number of rows.
 * Default: 0x00
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetDisplayOffset(unsigned int offset)
{
    sh1122_send_command(0xD3, 1);
    sh1122_send_command(offset & 0x3F, 1);
}

/* -------------------------------------------------------------------------
 * Set Display Clock Divide Ratio / Oscillator Frequency (Double Byte Command)
 *
 * Command: 0xD5, followed by packed byte:
 *   [7:4] = oscillator frequency (0-15, higher = faster)
 *   [3:0] = clock divide ratio - 1 (0-15)
 * Example: divider=1, freq=5 → 0x50
 * Vendor init uses 0x90 → divider=1, freq=9 (80 frames/sec)
 * Default: 0x50
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetOscFrequency(unsigned int divider, unsigned int freq)
{
    unsigned char val = (unsigned char)(((freq & 0xF) << 4) | ((divider - 1) & 0xF));
    sh1122_send_command(0xD5, 1);
    sh1122_send_command(val, 1);
}

/* -------------------------------------------------------------------------
 * Set Pre-Charge Period (Double Byte Command)
 *
 * Command: 0xD9, followed by packed byte:
 *   [7:4] = phase 2 (discharge) period in DCLKs (1-15)
 *   [3:0] = phase 1 (pre-charge) period in DCLKs (1-15)
 * Vendor init uses 0x76 → phase2=7, phase1=6
 * Default: 0x08 → phase2=0, phase1=8
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetPreChargePeriod(unsigned int phase1, unsigned int phase2)
{
    unsigned char val = (unsigned char)(((phase2 & 0xF) << 4) | (phase1 & 0xF));
    sh1122_send_command(0xD9, 1);
    sh1122_send_command(val, 1);
}

/* -------------------------------------------------------------------------
 * Set VCOM Deselect Level (Double Byte Command)
 *
 * Command: 0xDB, followed by level byte (0x00-0xFF)
 * Adjusts the VCOMH voltage level, affecting contrast and power.
 * Vendor init uses 0x3B; your original init uses 0x35.
 * Default: 0x35
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetVCOMDeselectLevel(unsigned int level)
{
    sh1122_send_command(0xDB, 1);
    sh1122_send_command(level & 0xFF, 1);
}

/* -------------------------------------------------------------------------
 * Set VSEGM Level / Pre-charge Voltage (Double Byte Command)
 *
 * Command: 0xDC, followed by level byte
 * Sets the segment pad output voltage at pre-charge stage.
 * Note: 0xDC is shared with SetDisplayStartLine in some datasheet versions.
 * Vendor init uses 0x1A.
 * Default: 0x35
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetVSEGMLevel(unsigned int level)
{
    sh1122_send_command(0xDC, 1);
    sh1122_send_command(level & 0xFF, 1);
}

/* -------------------------------------------------------------------------
 * Set Discharge Voltage Level / VSL
 *
 * Command: single byte, 0x30-0x33
 * 0x30 = VSL external (recommended for most modules)
 * 0x31 = internal level 1
 * 0x32 = internal level 2
 * 0x33 = internal level 3
 * Vendor init uses 0x30.
 * Default: 0x30
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SetVSL(void)
{
    sh1122_send_command(0x30, 1);
}

/* -------------------------------------------------------------------------
 * Send Data to GDDRAM
 *
 * Sends raw pixel bytes to the display RAM at the current row/column pointer.
 * A0 must be HIGH for data writes.
 * Each byte encodes two horizontally adjacent pixels:
 *   high nibble = left pixel gray (0-15)
 *   low  nibble = right pixel gray (0-15)
 * ------------------------------------------------------------------------- */
void sh1122Cmd_SendData(unsigned char *data, int bytecnt)
{
    sh1122_send_data(data, bytecnt);
}
