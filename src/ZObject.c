#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ZObject.h"

struct ZType _ZObject;
struct ZType _ZType;

struct ZType _ZObject = {
  .super = {
    .type   = &_ZType,
  },
  .name             = "Object",
  .superType        = &_ZObject,
  .objectSize       = sizeof(struct ZObject),
  .objectInit       = ZObject_init,
  .objectFinalize   = ZObject_finalize,
};

struct ZType _ZType = {
  .super = {
    .type   = &_ZType,
  },
  .name             = "Type",
  .superType        = &_ZObject,
  .objectSize       = sizeof(struct ZType),
  .objectInit       = ZType_init,
  .objectFinalize   = ZType_finalize,
};

void *ZObject(void)
{
  return &_ZObject;
}

void *ZType(void)
{
  return &_ZType;
}

void ZObject_init(void *self, va_list args)
{
}

void ZObject_finalize(void *self)
{
}

void ZType_init(void *_self, va_list args)
{
  struct ZType *self = _self;
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

char* ZType_getName(void *_self)
{
  struct ZType *self = _self;
  return self->name;
}

void *ZType_getSuperType(void *_self)
{
  struct ZType *self = _self;
  return self->superType;
}

void *ZObject_getType(void *self)
{
  return ((struct ZObject*)self)->type;
}

void *ZObject_getSuperType(void *self)
{
  return ((struct ZObject*)self)->type->superType;
}

__attribute__((malloc))
void* ZNew(void *_type, ...)
{
  va_list args;
  struct ZType *type = _type;
  struct ZObject *obj = calloc(1, type->objectSize);
  obj->type = type;
  va_start(args, _type);
  obj->type->objectInit(obj, args);
  va_end(args);
  return obj;
}

void ZDelete(void *_self)
{
  struct ZObject *self = _self;
  self->type->objectFinalize(self);
  free(self);
}

void *_ZInit(void *_self, void *_type, ...)
{
  va_list args;
  struct ZType *type = _type;
  struct ZObject *self = _self;
  self->type = type;
  va_start(args, _type);
  self->type->objectInit(self, args);
  va_end(args);
  return self;
}

void ZFinalize(void *_self)
{
  struct ZObject *self = _self;
  self->type->objectFinalize(self);
}
