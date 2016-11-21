# ZObject
This is my experiment on Object-Oriented Programming with C. It is a library
which can be called from any C/C++ program. It currently only supports GCC and
Clang (because the implementation heavily depends on [GCC extensions](https://gcc.gnu.org/onlinedocs/gcc-6.1.0/gcc/Common-Variable-Attributes.html)).
This library is in very early development and is not ready for production. Use
at your own risk!

## Why Do I Create This Library?
1. I am learning the internals of Object-Oriented Programming by creating this
   library.
2. It's fun.
3. I want to implement features that are not usually seen in similar libraries,
   such as reified generics (parameterized types).
4. Maybe in the future I will create a language that compiles to C using this
   library as its language runtime.

## Usage and Features

Please see [tests](https://github.com/brglng/zobject/tree/master/tests) for now.

## References
- [Object-Oriented Programming with ANSI C](https://www.cs.rit.edu/~ats/books/ooc.pdf)
- [GObject](https://developer.gnome.org/gobject/stable/)
- [Inside the C++ Object Model](https://www.amazon.com/Inside-Object-Model-Stanley-Lippman/dp/0201834545)
- [Cello](http://libcello.org/)
- [Viola](https://github.com/eatonphil/Viola)
- [Implementing smart pointers for the C programming language](https://snai.pe/c/c-smart-pointers/)
- [libcsptr](https://github.com/Snaipe/libcsptr)
