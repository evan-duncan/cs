#include "io.h"
#include "framebuffer.h"

unsigned short __fb_present_pos = 0x00000000;

/**
 * Moves the cursor of the framebuffer to the given position
 * @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT,    pos & 0x00FF);
  __fb_present_pos = pos;
}

/**
 * Writes a character with the given foreground and background to position i
 * in the framebuffer.
 * @param i  The location in the framebuffer
 * @param c  The character
 * @param fg The foreground color
 * @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
  char *fb = (char *) FB_ADDRESS;
  fb[i] = c;
  fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

/**
 * Writes the contents of the buffer of length `len` to the screen.
 * Automatically advances the cursor after a character has been written
 * and scroll the screen if necessary.
 * @param  buf The contents to write to the screen
 * @param  len The length of the buffer
 * @return int
 */
int fb_write(char *buf, unsigned int len)
{
  unsigned int i;
  for (i = 0; i < len; i++)
  {
    fb_write_cell(2 * __fb_present_pos, buf[i], FB_GREEN, FB_DARK_GREY);
    fb_move_cursor(__fb_present_pos + 1);
  }

  return 0;
}
