#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "z/object.h"

struct RaiiTest {
  struct ZObject    super;
  bool*             destructor_is_called;
};

struct RaiiTestClass {
  struct ZClass     super;
};

struct ZClass *RaiiTestClass = NULL;
struct ZClass *RaiiTest = NULL;

void RaiiTest_init(void *_self, va_list args)
{
  struct RaiiTest *self = _self;
  self->destructor_is_called = va_arg(args, bool*);
}

void RaiiTest_finalize(void *_self) {
  struct RaiiTest *self = _self;
  *self->destructor_is_called = true;
}

void RaiiTestClass_init(void *_self, va_list args)
{
  ZClass_init(_self, args);
}

void RaiiTestClass_finalize(void *_self, va_list args)
{
}

void z_init_class_RaiiTest(void)
{
  if (!RaiiTestClass) {
    RaiiTestClass = z_new(ZClass, "RaiiTestClass", ZClass,
                          sizeof(struct RaiiTestClass), RaiiTestClass_init,
                          RaiiTestClass_finalize);
  }
  if (!RaiiTest) {
    RaiiTest = z_new(RaiiTestClass, "RaiiTest", ZObject,
                     sizeof(struct RaiiTest), RaiiTest_init, RaiiTest_finalize);
  }
}

void test_raii(bool *destructor_is_called)
{
  z_init_class_RaiiTest();
  *destructor_is_called = false;
  ZRef struct RaiiTest *raii_obj = z_new(RaiiTest, destructor_is_called);
}

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

  // RAII
  bool destructor_is_called = false;
  test_raii(&destructor_is_called);
  assert(destructor_is_called);

  return 0;
}
