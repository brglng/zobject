#include <string.h>
#include "ZObject.h"

struct ZType _ZObject;
struct ZType _ZType;

void *ZObject(void) {
  return &_ZObject;
}

void *ZObject_init(void *self) {
  return self;
}

void ZObject_finalize(void *self) {
}

bool _ZObject_equal(void *self, void *other) {
  assert(ZIsObject(self));
  assert(ZIsObject(other));
  return self == other;
}

struct ZType _ZObject = {
  .super = {
    .magic  = Z_MAGIC,
    .type   = &_ZType,
  },
  .name         = "Object",
  .superType    = &_ZObject,
  .objectSize   = sizeof(struct ZObject),
  .finalize     = ZObject_finalize,
  .equal        = _ZObject_equal,
};

void *ZType(void) {
  return &_ZType;
}

void *ZType_init(void *_self, char *name, void *superType, size_t objectSize,
                 ZDestructor finalize) {
  struct ZType *self = ZCast(&_ZType, _self);

  self->name = name;
  self->superType = superType;
  self->objectSize = objectSize;
  self->finalize = finalize;
  self->equal = _ZObject_equal;

  return self;
}

void ZType_finalize(void *_self) {
}

struct ZType _ZType = {
  .super = {
    .magic  = Z_MAGIC,
    .type   = &_ZType,
  },
  .name         = "Type",
  .superType    = &_ZObject,
  .objectSize   = sizeof(struct ZType),
  .finalize     = ZType_finalize,
  .equal        = _ZObject_equal,
};

__attribute__((malloc))
void *ZNew(void *_type) {
  struct ZType *type = ZCast(&_ZType, _type);
  struct ZObject *obj;

  obj = calloc(1, type->objectSize);
  obj->magic = Z_MAGIC;
  obj->type = type;

  return obj;
}

void ZDelete(void *_self) {
  struct ZObject *self = ZCast(&_ZObject, _self);
  ((struct ZType *)self->type)->finalize(self);
  free(self);
}
