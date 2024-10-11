#include "myfibo.hpp"
#include <iostream>
#include <string>

MyFibo::MyFibo(int n, const std::string& title)
  : _n(n)
  , _title(title)
{
}

/**
 * Destructor
 */
MyFibo::~MyFibo()
{
}

void MyFibo::display() const
{
  std::cout << "MyFibo : Valeur de n = " << _n << std::endl;
}

void MyFibo::workInPlaceRef(int nincr, MyFibo& myfibo)
{
  myfibo._n += nincr;
}

void MyFibo::workInPlacePtr(int nincr, MyFibo* myfibo)
{
  myfibo->_n += nincr;
}