#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "ZGeneric.h"

Z_DEFINE_TYPE(ZGeneric, ZType)

void ZGeneric_init(void *_self, va_list args)
{
  struct ZGeneric *self = Z_cast(ZGeneric(), _self);
  ZType_init(self, args);
  self->numArgs = Z_VA_ARG_SIZE_T(args);
  self->args = calloc(self->numArgs, sizeof(void *));
  assert(self->args);
  memcpy(self->args, va_arg(args, void **), self->numArgs * sizeof(void *));
}

void ZGeneric_finalize(void *_self)
{
  struct ZGeneric *self = Z_cast(ZGeneric(), _self);
  free(self->args);
}

void ZGenericType_init(void *_self, va_list args)
{
  struct ZGenericType *self = Z_cast(ZGenericType(), _self);
  ZType_init(self, args);
}

void ZGenericType_finalize(void *_self)
{}
