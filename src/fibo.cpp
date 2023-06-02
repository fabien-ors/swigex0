#include "fibo.hpp"
#include "version.h"

#include <iostream>
#include <sstream>
#include <cmath> // std::isnan

#define DEFAULT_TITLE "Fibonacci List"

/**
 * Return the Nth Fibonacci number, -1 in case of error
 *
 * @param n: index of the value
 */
int fibn(int n)
{
  if (n <= 0)
  {
    std::cout << "Error in fibn: Integer argument must be strictly positive!" << std::endl;
    return -1;
  }
  int a = 0;
  int b = 1;
  int i = 1;
  while (1)
  {
    if (i == n) return a;
    int aa = a;
    a = b;
    b = aa+b;
    i++;
  }
  std::cout << "Error in fibn: Unknown error!" << std::endl;
  return -1;
}

/**
 * Print Fibonacci numbers up to the provided value
 *
 * @param n: maximum value to be generated
 */
std::vector<int> fib(int n)
{
  std::vector<int> res;
  if (n <= 0)
  {
    std::cout << "Error in fib: Integer argument must be strictly positive!" << std::endl;
    return res;
  }
  int a = 0;
  int b = 1;
  while (a < n)
  {
    res.push_back(a);
    int aa = a;
    a = b;
    b = aa+b;
  }
  return res;
}

/**
 * Default constructor of a class which handle Fibonacci integer list up to n
 * 
 * @param n     Strict Positive Integer
 * @param title Title to be printed
 */
Fibo::Fibo(int n, const std::string& title)
: _n(n)
, _title(title)
{
  if (_n <= 0)
  {
    std::cout << "Fibonacci class must be initialized with a strict positive integer. N is set to 50." << std::endl;
    _n = 50;
  }

  // Test static_assert compilation
  static_assert( true, "Class cannot be cloned as it is abstract" );
  // Test isnan
  double a = 5.5;
  if (std::isnan(a))
    std::cout << "a is nan" << std::endl;

  if (_title.empty())
  {
    std::stringstream sstr;
    sstr << DEFAULT_TITLE << " (" << SWIGEX0_RELEASE << " - " << SWIGEX0_DATE << ")";
    _title = sstr.str();
  }
}

/**
 * Destructor
 */
Fibo::~Fibo()
{
}

/**
 * Reset the Fibonacci list from another one
 *
 * @param fibo Source Fibonacci list
 */
void Fibo::resetFromFiboVal(Fibo fibo)
{
  _title = fibo._title;
  _n = fibo._n;
}

/**
 * Reset the Fibonacci list from another one
 *
 * @param fibo Source Fibonacci list
 */
void Fibo::resetFromFiboRef(const Fibo& fibo)
{
  _title = fibo._title;
  _n = fibo._n;
}

/**
 * Write the Fibonacci list to standard output
 *
 * @param showTitle Flag for printing the title
 */
void Fibo::display(bool showTitle) const
{
  if (showTitle)
    std::cout << _title << ": ";
  std::vector<int> res = getVector();
  for (const auto& i: res)
    std::cout << i << ' ';
  std::cout << std::endl;
}

/**
 * Return the Fibonacci list as a vector of integer
 *
 * @return Fibonacci integer vector
 */
std::vector<int> Fibo::getVector() const
{
  return fib(_n);
}

/**
 * Return the Fibo object title (including version and date by default)
 *
 * @return title
 */
std::string Fibo::getTitle() const
{
  return _title;
}

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

StdoutRedirect::StdoutRedirect(const std::string& file) :
#if defined(_WIN32) || defined(_WIN64)
  _old_stdout(0)
#else
  _coutbuf(nullptr),
  _out()
#endif
{
  if (!file.empty())
    start(file);
}

StdoutRedirect::~StdoutRedirect()
{
  stop();
}

/**
 * Save current stdout handle and redirect std::cout to a file
 *
 * @param[in] file File path to be written
 */
void StdoutRedirect::start(const std::string& file)
{
#if defined(_WIN32) || defined(_WIN64)
  // https://stackoverflow.com/questions/54094127/redirecting-stdout-in-win32-does-not-redirect-stdout/54096218
  _old_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE new_stdout = CreateFileA(file.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  SetStdHandle(STD_OUTPUT_HANDLE, new_stdout);
  int fd = _open_osfhandle((intptr_t)new_stdout, _O_WRONLY|_O_TEXT);
  _dup2(fd, _fileno(stdout));
  _close(fd);
#else
  _coutbuf = std::cout.rdbuf();
  _out.open(file, std::fstream::out | std::fstream::trunc);
  std::cout.rdbuf(_out.rdbuf());
#endif
}

/**
 *  Restore original stdout
 */
void StdoutRedirect::stop()
{
#if defined(_WIN32) || defined(_WIN64)
  // https://stackoverflow.com/questions/32185512/output-to-console-from-a-win32-gui-application-on-windows-10
  SetStdHandle(STD_OUTPUT_HANDLE, _old_stdout);
  int fd = _open_osfhandle((intptr_t)_old_stdout, _O_WRONLY|_O_TEXT);
  FILE* fp = _fdopen(fd, "w");
  freopen_s( &fp, "CONOUT$", "w", stdout);
#else
  std::cout.rdbuf(_coutbuf);
  _out.close();
#endif
}
