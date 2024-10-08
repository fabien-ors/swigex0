// Keep sync with PYTHON_PACKAGE_NAME in CMakeLists.txt
%module(directors="1") swigex0

// Note : Keep order in this file!

//////////////////////////////////////////////////////////////
//    Specific typemaps and fragments for Python language   //
//////////////////////////////////////////////////////////////

%begin %{
// For converting NumPy integers to C++ integers
// https://github.com/swig/swig/issues/888
#define SWIG_PYTHON_CAST_MODE

// Look below (python code) for integer NaN (inan definition)
#if defined(_WIN32) || defined(_WIN64)
  #define NPY_INT_TYPE       NPY_INT32
  #define NPY_INT_OUT_TYPE   int
#else // Linux or MacOS
  #define NPY_INT_TYPE       NPY_INT64
  #define NPY_INT_OUT_TYPE   int64_t
#endif
%}

// Include numpy interface for creating arrays

%{
  #define SWIG_FILE_WITH_INIT
%}
%include numpy.i
%init %{
  import_array(); // Mandatory for using PyArray_* functions
%}

%fragment("FromCpp", "header")
{
  int fiboFromCpp(PyObject** obj, const Fibo& fib)
  {
    // Type definitions
    int myres = SWIG_TypeError;
    
    // Conversion to 1D NumPy array of integers

    std::vector<int> vec = fib.getVector(); // Copy
    npy_intp dims[1] = { (npy_intp)vec.size() };
    PyArray_Descr* descr = PyArray_DescrFromType(NPY_INT_TYPE);
    *obj = PyArray_NewFromDescr(&PyArray_Type, descr, 1, dims, NULL, NULL, 0, NULL);
    if (*obj != NULL)
    {
      NPY_INT_OUT_TYPE* array_ptr = (NPY_INT_OUT_TYPE*) PyArray_DATA((PyArrayObject*)(*obj));
      std::copy(vec.cbegin(), vec.cend(), array_ptr);
      myres = SWIG_OK;
    }
    return myres;
  }
}

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

%pythoncode %{

%}