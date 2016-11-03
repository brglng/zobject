#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ZObject.h"

struct ZType _ZObject;
struct ZType _ZType;

void *ZObject   = &_ZObject;
void *ZType     = &_ZType;

struct ZType _ZObject = {
  .super = {
    .magic  = Z_MAGIC,
    .type   = &_ZType,
  },
  .name             = "Object",
  .superType        = &_ZObject,
  .objectSize       = sizeof(struct ZObject),
  .objectInit       = ZObject_init,
  .objectFinalize   = ZObject_finalize,
};

void *ZObject_initType(void)
{
  return &_ZObject;
}

void ZObject_init(void *_self, va_list args)
{
}

void ZObject_finalize(void *self)
{
}

struct ZType _ZType = {
  .super = {
    .magic  = Z_MAGIC,
    .type   = &_ZType,
  },
  .name             = "Type",
  .superType        = &_ZObject,
  .objectSize       = sizeof(struct ZType),
  .objectInit       = ZType_init,
  .objectFinalize   = ZType_finalize,
};

void *ZType_initType(void)
{
  return &_ZType;
}

void ZType_init(void *_self, va_list args)
{
  struct ZType *self = Z_cast(ZType, _self);
  self->super.type = &_ZType;
  self->name = va_arg(args, char*);
  self->superType = va_arg(args, void*);
  self->objectSize = va_arg(args, size_t);
  self->objectInit = va_arg(args, ZConstructor);
  self->objectFinalize = va_arg(args, ZDestructor);
}

void ZType_finalize(void *self)
{
}

__attribute__((malloc))
void* _Z_new(void *_type, ...)
{
  va_list args;
  struct ZType *type = Z_cast(ZType, _type);
  struct ZObject *obj = calloc(1, type->objectSize);
  obj->magic = Z_MAGIC;
  obj->type = type;
  va_start(args, _type);
  obj->type->objectInit(obj, args);
  va_end(args);
  return obj;
}

void Z_delete(void *_self)
{
  struct ZObject *self = Z_cast(ZObject, _self);
  self->type->objectFinalize(self);
  free(self);
}
