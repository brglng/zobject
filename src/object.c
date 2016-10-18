#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "z/object.h"

ZClass __ZObject;
ZClass __ZClass;

ZClass __ZObject = {
  .super = {
    .klass = &__ZClass,
  },
  .name             = "ZObject",
  .super_class      = &__ZObject,
  .object_size      = sizeof(ZObject),
  .object_init      = (ZConstructor)z_object_init,
  .object_finalize  = (ZDestructor)z_object_finalize,
};

ZClass _ZClass = {
  .super = {
    .klass = &__ZClass
  },
  .name             = "ZClass",
  .super_class      = &__ZObject,
  .object_size      = sizeof(ZClass),
  .object_init      = (ZConstructor)z_class_init,
  .object_finalize  = (ZDestructor)z_class_finalize,
};

ZClass *ZObject_ = &__ZObject;
ZClass *ZClass_ = &__ZClass;

void z_object_init(ZObject *self)
{
}

void z_object_finalize(ZObject *self)
{
}

void z_class_init(ZClass *self)
{}

void z_class_finalize(ZClass *self)
{}

char* z_class_get_name(ZClass *self)
{
  return self->name;
}

ZClass* z_class_get_super_class(ZClass *self)
{
  return self->super_class;
}

ZClass* z_object_get_class(void *self)
{
  return ((ZObject*)self)->klass;
}

ZClass* z_object_get_super_class(void *self)
{
  return ((ZObject*)self)->klass->super_class;
}

void* __z_new(ZClass *type, ...)
{
  va_list args;
  ZObject* obj = (ZObject*)malloc(type->object_size);
  obj->klass = type;
  obj->refcount = 1;
  va_start(args, type);
  obj->klass->object_init(obj, args);
  va_end(args);
  return obj;
}

void z_delete(void *self_)
{
  ZObject *self = self_;
  self->klass->object_finalize(self);
  free(self);
}

void* z_ref(void *self_)
{
  ZObject *self = self_;
  self->refcount++;
  return self;
}

void z_unref(void *self_)
{
  ZObject *self = self_;
  self->refcount--;
  if (self->refcount == 0) {
    z_delete(self);
  }
}

void __z_cleanup(void *self)
{
  z_unref(*(void**)self);
}
