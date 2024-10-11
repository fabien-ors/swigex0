%module(directors="1") swigex0

// Note : Keep order in this file!

//////////////////////////////////////////////////////////////
//       Specific typemaps and fragments for R language     //
//////////////////////////////////////////////////////////////

// Add typecheck typemaps for dispatching functions
%typemap(rtypecheck, noblock=1) const Fibo&, Fibo 
{ 
  length($arg) == 0 || 
  (length($arg) > 0 && (is.integer(unlist($arg)) || 
  is.numeric(unlist($arg)))) 
}

%fragment("FromCpp", "header")
{  
  int fiboFromCpp(SEXP* obj, const Fibo& fib)
  {
    // Get vector for fibonacci list
    int myres = SWIG_TypeError;
    std::vector<int> vec = fib.getVector();

    // Convert to R vector
    *obj = swig::from(vec);
    myres = (*obj) == NULL ? SWIG_TypeError : SWIG_OK;
    return myres;
  }

  int fiboFromCppCreate(SEXP* obj, const Fibo& fib)
  {
    *obj = SWIG_R_NewPointerObj(SWIG_as_voidptr(&fib), SWIGTYPE_p_Fibo, 0 |  0 );
    int myres = (*obj) == NULL ? SWIG_TypeError : SWIG_OK;
    return myres;
  }
}

// This for automatically convert R lists to externalptr
%typemap(scoerceout) Fibo,    Fibo*,    Fibo&
 %{    %}

//////////////////////////////////////////////////////////////
//         C++ library SWIG includes and typemaps           //
//////////////////////////////////////////////////////////////

%include ../swig/swig_inc.i

//////////////////////////////////////////////////////////////
//                C++ library SWIG interface                //
//////////////////////////////////////////////////////////////

%include ../swig/swig_exp.i

//////////////////////////////////////////////////////////////
//       Add target language additional features below      //
//////////////////////////////////////////////////////////////

// TODO : Redirection of std::cout for windows RGui.exe users

%insert(s) %{

`Fibo_create__SWIG_0` = function(n, title)
{
  n = as.integer(n);
  
  if(length(n) > 1) {
    warning("using only the first element of n");
  };
  
  title = as(title, "character"); 
  ;ans = .Call('R_swig_Fibo_create__SWIG_0', n, title, PACKAGE='swigex0');
  ans <- if (is.null(ans)) ans
  else new("_p_Fibo", ref=ans);
  
  ans
  
}

attr(`Fibo_create__SWIG_0`, 'returnType') = '_p_Fibo'
attr(`Fibo_create__SWIG_0`, "inputTypes") = c('integer', 'character')
class(`Fibo_create__SWIG_0`) = c("SWIGFunction", class('Fibo_create__SWIG_0'))

%}

