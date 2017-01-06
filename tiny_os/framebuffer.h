#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

/* The I/O ports */
#define FB_COMMAND_PORT       0x3D4
#define FB_DATA_PORT          0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND  14
#define FB_LOW_BYTE_COMMAND   15

/* Framebuffer colors */
#define FB_GREEN              2
#define FB_DARK_GREY          8

#define FB_ADDRESS            0x000B8000


/**
 * Writes the contents of the buffer of length `len` to the screen.
 * Automatically advances the cursor after a character has been written
 * and scroll the screen if necessary.
 * @param  buf The contents to write to the screen
 * @param  len The length of the buffer
 * @return int
 */
int fb_write(char *buf, unsigned int len);

#endif /* INCLUDE_FRAMEBUFFER_H */
