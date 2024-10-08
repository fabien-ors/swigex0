%{
  #include <iostream>
  #include "swigex0_export.hpp"
  #include "fibo.hpp"
%}

////////////////////////////
//        Typemaps        //
////////////////////////////

%include std_vector.i
%include std_string.i
%template(VectorInt) std::vector< int >;

////////////////////////////////////////////////
// Conversion C++ => Target language

// Note : Before including this file :
//        - fiboFromCpp functions must be defined in FromCpp fragment

%typemap(out, fragment="FromCpp") Fibo
{
  std::cout << "%typemap(out) Fibo" << std::endl;
  int errcode = fiboFromCpp(&($result), $1);
  if (!SWIG_IsOK(errcode))
    SWIG_exception_fail(SWIG_ArgError(errcode), "in method $symname, wrong return value: $type");
}

%typemap(out, fragment="FromCpp") Fibo*, Fibo&
{
  std::cout << "%typemap(out) Fibo*, Fibo&" << std::endl;
  int errcode = fiboFromCpp(&($result), *$1);
  if (!SWIG_IsOK(errcode))
    SWIG_exception_fail(SWIG_ArgError(errcode), "in method $symname, wrong return value: $type");
}