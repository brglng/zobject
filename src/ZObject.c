#include <stdio.h>
#include <stdlib.h>
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
  .isEqual      = ZObject_isEqual,
};

void *ZObject(void) {
  return &_ZObject;
}

void ZObject_init(void *self, va_list args) {
}

void ZObject_finalize(void *self) {
}

bool ZObject_isEqual(void *self, void *other) {
  assert(ZIsObject(self));
  assert(ZIsObject(other));
  return self == other;
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
  .isEqual      = ZObject_isEqual,
};

void *ZType(void) {
  return &_ZType;
}

void ZType_new(void *_self) {
  struct ZType *self = ZCast(&_ZType, _self);

  self->isEqual = ZObject_isEqual;
}

void ZType_init(void *_self, va_list args) {
  struct ZType *self = ZCast(&_ZType, _self);
  self->name = va_arg(args, char *);
  self->superType = va_arg(args, void *);
  self->objectSize = Z_VA_ARG_SIZE_T(args);
  self->init = va_arg(args, ZConstructor);
  self->finalize = va_arg(args, ZDestructor);
  self->isEqual = ZObject_isEqual;
}

void ZType_finalize(void *_self) {
}

__attribute__((malloc))
void *ZNew(void *_type, ...) {
  struct ZType *type = ZCast(&_ZType, _type);
  struct ZObject *obj;
  va_list args;

  obj = calloc(1, type->objectSize);
  obj->magic = Z_MAGIC;
  obj->type = type;

  va_start(args, _type);
  type->init(obj, args);
  va_end(args);

  return obj;
}

void ZDelete(void *_self) {
  struct ZObject *self = ZCast(&_ZObject, _self);
  ((struct ZType *)self->type)->finalize(self);
  free(self);
}
