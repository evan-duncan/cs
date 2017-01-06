#include "io.h"
#include "ports.h"

/* The I/O ports */

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 11520 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (11520 / divisor) bits/s.
 *
 * @param com     The COM port to configure
 * @param divisor The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
  outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
  outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
  outb(SERIAL_DATA_PORT(com), divisor & 0x0FF);
}

/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com)
{
  /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
   * Content: | d | b | prty  | s | dl  |
   * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
   */
  outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_configure_fifo:
 *  Configures the FIFO queue of the given serial port.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_fifo(unsigned short com)
{
  /* Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
   * Content: | lvl | bs | r | dma | clt | clr | e |
   * Value:   | 1 1 | 0  | 0 | 0   | 1   | 1   | 1 | = 0xC7
   */
  outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

/** serial_configure_modem;
 *  Configures the modem of the given serial port.
 *  The modem control register is used for very simple hardware 
 *  flow control via the Ready To Transmit (RTS) and 
 *  Data Terminal Ready (DTR) pins. When configuring the serial 
 *  port we want RTS and DTR to be 1, which means 
 *  that we are ready to send data.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_modem(unsigned short com)
{
  /* Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
   * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
   * Value:   | 0 | 0 | 0  | 0  | 0   | 0   | 1   | 1   | = 0x03
   */
  outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com)
{
  /* 0x20 = 0010 0000 */
  return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

int serial_write(char *buf, unsigned int len)
{

  serial_configure_baud_rate(SERIAL_COM1_BASE, 3);
  serial_configure_line(SERIAL_COM1_BASE);
  serial_configure_fifo(SERIAL_COM1_BASE);
  serial_configure_modem(SERIAL_COM1_BASE);

  unsigned int i;
  for (i = 0; i < len; i++)
  {
    while(!serial_is_transmit_fifo_empty(SERIAL_COM1_BASE));
    outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), buf[i]);
  }

  return 0;
}
