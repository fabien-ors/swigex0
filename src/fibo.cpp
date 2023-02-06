#include "fibo.hpp"
#include "version.h"

#include <iostream>
#include <sstream>
#include <cmath>

/**
 * Return the Nth Fibonacci number, -1 in case of error
 *
 * @param n: index of the value
 */
int fibn(int n)
{
  if (n < 0 || n == INT_NA)
  {
    std::cout << "Error in fibn: Integer argument must be positive!" << std::endl;
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
  if (n < 0 || n == INT_NA)
  {
    std::cout << "Error in fib: Integer argument must be positive!" << std::endl;
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
Fibo::Fibo(int n, const String& title)
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
  std::vector<int> res = get();
  for (const auto& i: res)
    std::cout << i << ' ';
  std::cout << std::endl;
}

/**
 * Return the Fibonacci list as a vector of integer
 *
 * @return Fibonacci integer vector
 */
std::vector<int> Fibo::get() const
{
  return fib(_n);
}
