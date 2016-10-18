#include <stdio.h>
#include <stdlib.h>
#include "z/object.h"

int main(int argc, char *argv[])
{
  ZVar a = z_new(ZObject);
  ZVar b = z_ref(a);
  return 0;
}
