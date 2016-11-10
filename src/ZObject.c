#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "ZObject.h"

struct ZType _ZObject;
struct ZType _ZType;

struct ZType _ZObject = {
  .super = {
    .magic  = Z_MAGIC,
    .type   = &_ZType,
  },
  .name         = "Object",
  .superType    = &_ZObject,
  .objectSize   = sizeof(struct ZObject),
  .init         = ZObject_init,
  .finalize     = ZObject_finalize,
  .isInstanceOf = ZObject_isInstanceOf,
};

void *ZObject(void)
{
  return &_ZObject;
}

void ZObject_init(void *_self, va_list args)
{
}

void ZObject_finalize(void *self)
{
}

bool ZObject_isInstanceOf(void *self, void *type)
{
  if (type == ZObject()) {
    return Z_isObject(self);
  } else {
    void *t = ZObject_getType(self);
    while (t != type && t != ZObject()) {
      t = ZType_getSuperType(t);
    }
    return t == type;
  }
}

struct ZType _ZType = {
  .super = {
    .magic  = Z_MAGIC,
    .type   = &_ZType,
  },
  .name         = "Type",
  .superType    = &_ZObject,
  .objectSize   = sizeof(struct ZType),
  .init         = ZType_init,
  .finalize     = ZType_finalize,
  .isInstanceOf = ZObject_isInstanceOf,
};

void *ZType(void)
{
  return &_ZType;
}

void ZType_init(void *_self, va_list args)
{
  struct ZType *self = Z_cast(ZType(), _self);
  self->name = va_arg(args, char *);
  self->superType = va_arg(args, void *);
  self->objectSize = va_arg(args, size_t);
  self->init = va_arg(args, void *);
  self->finalize = va_arg(args, void *);
  self->isInstanceOf = ZObject_isInstanceOf;
}

void ZType_finalize(void *self)
{
}

__attribute__((malloc))
void *Z_vnew(void *_type, va_list args)
{
  struct ZType *type = Z_cast(ZType(), _type);
  struct ZObject *obj = calloc(1, type->objectSize);
  obj->magic = Z_MAGIC;
  obj->type = type;
  type->init(obj, args);
  return obj;
}

__attribute__((malloc))
void *Z_new(void *type, ...)
{
  va_list args;
  va_start(args, type);
  void *obj = Z_vnew(type, args);
  va_end(args);
  return obj;
}

void Z_delete(void *_self)
{
  struct ZObject *self = Z_cast(ZObject(), _self);
  self->type->finalize(self);
  free(self);
}
