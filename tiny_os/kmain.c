#include "io.h"
#include "framebuffer.h"
#include "ports.h"

char foo[] = "foo";
char bar[] = "bar";

void kmain()
{
  fb_write(foo, 3);
  serial_write(bar, 3);
}