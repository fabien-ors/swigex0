%{
  #include "swigex0_export.hpp"
  #include "swigex0_define.hpp"
  #include "fibo.hpp"
  #include "stdoutredirect.hpp"
%}

////////////////////////////
//        Typemaps        //
////////////////////////////

%include std_vector.i
%include std_string.i
%template(VectorInt) std::vector< int >;
