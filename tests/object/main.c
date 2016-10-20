#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "z/object.h"

int main(int argc, char *argv[])
{
  struct ZObject *obj = z_new(ZObject);
  struct ZClass *cls = ZObject_get_class(obj);
  struct ZClass *cls_cls = ZObject_get_class(cls);

  // Object
  assert(ZObject_get_class(obj) == ZObject);
  assert(ZObject_get_class(cls) == ZClass);
  assert(ZObject_get_class(cls_cls) == ZClass);
  assert(ZObject_get_super_class(obj) == ZObject);
  assert(ZObject_get_super_class(cls) == ZObject);
  assert(ZObject_get_super_class(cls_cls) == ZObject);

  // Class
  assert(strcmp(ZClass_get_name(cls), "Object") == 0);
  assert(strcmp(ZClass_get_name(cls_cls), "Class") == 0);
  assert(ZClass_get_super_class(cls) == ZObject);
  assert(ZClass_get_super_class(cls_cls) == ZObject);

  // reference counting
  assert(obj->refcount == 1);
  struct ZObject *obj1 = z_ref(obj);
  assert(obj->refcount == 2);
  z_unref(obj1);
  assert(obj->refcount == 1);

  return 0;
}
