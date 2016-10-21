#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "z/object.h"

// this is only necessary if you put this in a header
Z_DECLARE_CLASS(RaiiTest)

struct RaiiTest {
  struct ZObject    super;
  bool*             destructor_is_called;
};

struct RaiiTestClass {
  struct ZClass     super;
};

void RaiiTest_init(void *_self, va_list args)
{
  struct RaiiTest *self = _self;
  self->destructor_is_called = va_arg(args, bool*);
}

void RaiiTest_finalize(void *_self)
{
  struct RaiiTest *self = _self;
  *self->destructor_is_called = true;
}

void RaiiTestClass_init(void *_self, va_list args)
{
  // almost certainly you should call the parent meta class's constructor
  ZClass_init(_self, args);
}

void RaiiTestClass_finalize(void *_self)
{
}

// must put this *after* your class's structs and methods
Z_DEFINE_CLASS(RaiiTest, ZObject)

void test_raii(bool *destructor_is_called)
{
  *destructor_is_called = false;

  // variables declared with ZRef are RAII variables with reference counting
  ZRef(RaiiTest) raii_obj = z_new(RaiiTest(), destructor_is_called);
}

int main(int argc, char *argv[])
{
  struct ZObject *obj = z_new(ZObject());
  struct ZClass *cls = ZObject_get_class(obj);
  struct ZClass *cls_cls = ZObject_get_class(cls);

  // Object
  assert(ZObject_get_class(obj) == ZObject());
  assert(ZObject_get_class(cls) == ZClass());
  assert(ZObject_get_class(cls_cls) == ZClass());
  assert(ZObject_get_super_class(obj) == ZObject());
  assert(ZObject_get_super_class(cls) == ZObject());
  assert(ZObject_get_super_class(cls_cls) == ZObject());

  // Class
  assert(strcmp(ZClass_get_name(cls), "Object") == 0);
  assert(strcmp(ZClass_get_name(cls_cls), "Class") == 0);
  assert(ZClass_get_super_class(cls) == ZObject());
  assert(ZClass_get_super_class(cls_cls) == ZObject());

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
