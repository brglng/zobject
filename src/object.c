#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "z/object.h"

struct ZClass _ZObject;
struct ZClass _ZClass;
struct ZClass *ZObject = &_ZObject;
struct ZClass *ZClass = &_ZClass;

struct ZClass _ZObject = {
  .super = {
    .refcount   = 1,
    .klass      = &_ZClass,
  },
  .name             = "Object",
  .super_class      = &_ZObject,
  .object_size      = sizeof(struct ZObject),
  .object_init      = (ZConstructor)ZObject_init,
  .object_finalize  = (ZDestructor)ZObject_finalize,
};

struct ZClass _ZClass = {
  .super = {
    .refcount   = 1,
    .klass      = &_ZClass,
  },
  .name             = "Class",
  .super_class      = &_ZObject,
  .object_size      = sizeof(struct ZClass),
  .object_init      = (ZConstructor)ZClass_init,
  .object_finalize  = (ZDestructor)ZClass_finalize,
};

void ZObject_init(struct ZObject *self)
{
}

void ZObject_finalize(struct ZObject *self)
{
}

void ZClass_init(struct ZClass *self)
{
}

void ZClass_finalize(struct ZClass *self)
{
}

char* ZClass_get_name(struct ZClass *self)
{
  return self->name;
}

struct ZClass* ZClass_get_super_class(struct ZClass *self)
{
  return self->super_class;
}

struct ZClass* ZObject_get_class(void *self)
{
  return ((struct ZObject*)self)->klass;
}

struct ZClass* ZObject_get_super_class(void *self)
{
  return ((struct ZObject*)self)->klass->super_class;
}

__attribute__((malloc))
void* z_new(struct ZClass *type, ...)
{
  va_list args;
  struct ZObject* obj = (struct ZObject*)malloc(type->object_size);
  obj->klass = type;
  obj->refcount = 1;
  va_start(args, type);
  obj->klass->object_init(obj, args);
  va_end(args);
  return obj;
}

void z_delete(void *_self)
{
  struct ZObject *self = _self;
  self->klass->object_finalize(self);
  free(self);
}

void* z_ref(void *_self)
{
  struct ZObject *self = _self;
  self->refcount++;
  return self;
}

void z_unref(void *_self)
{
  struct ZObject *self = _self;
  self->refcount--;
  if (self->refcount == 0) {
    z_delete(self);
  }
}
