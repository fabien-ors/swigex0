%{
  #include "swigex0_export.hpp"
  #include "fibo.hpp"
%}

////////////////////////////
//        Typemaps        //
////////////////////////////

%include std_vector.i
%include std_string.i
%template(VectorInt) std::vector< int >;
