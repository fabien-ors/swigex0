#include "args.hpp"

#include <iostream>

TypeClass::TypeClass()
: _varInt()
, _varDouble()
, _varString()
{

}
TypeClass::~TypeClass()
{

}

int TypeClass::testInt(int a)
{
  _varInt = a;
  std::cout << "Test int: " << a << std::endl;
  return _varInt;
}

const int& TypeClass::testIntRef(const int& a)
{
  _varInt = a;
  std::cout << "Test int Reference: " << a << std::endl;
  return _varInt;
}

const int* TypeClass::testIntPtr(const int* a)
{
  _varInt = *a;
  std::cout << "Test int Pointer: " << *a << std::endl;
  return &_varInt;
}

void TypeClass::testIntRefOut(int& a) const
{
  a = _varInt;
  std::cout << "Test int Reference Out: " << a << std::endl;
}

void TypeClass::testIntRefDef(const int& a, const int& b)
{
  _varInt = a;
  std::cout << "Test int Reference Def: " << a << " - " << b << std::endl;
}

double TypeClass::testDouble(double a)
{
  _varDouble = a;
  std::cout << "Test double: " << a << std::endl;
  return _varDouble;
}

const double& TypeClass::testDoubleRef(const double& a)
{
  _varDouble = a;
  std::cout << "Test double Reference: " << a << std::endl;
  return _varDouble;
}

const double* TypeClass::testDoublePtr(const double* a)
{
  _varDouble = *a;
  std::cout << "Test double Pointer: " << *a << std::endl;
  return &_varDouble;
}

void TypeClass::testDoubleRefOut(double& a) const
{
  a = _varDouble;
  std::cout << "Test Double Reference Out: " << a << std::endl;
}

void TypeClass::testDoubleRefDef(const double& a, const double& b)
{
  _varDouble = a;
  std::cout << "Test Double Reference Def: " << a << " - " << b << std::endl;
}

String TypeClass::testString(String a)
{
  _varString = a;
  std::cout << "Test String: " << a << std::endl;
  return _varString;
}

const String& TypeClass::testStringRef(const String& a)
{
  _varString = a;
  std::cout << "Test String Reference: " << a << std::endl;
  return a;
}

const String* TypeClass::testStringPtr(const String* a)
{
  _varString = *a;
  std::cout << "Test String Pointer: " << *a << std::endl;
  return &_varString;
}

void TypeClass::testStringRefOut(String& a) const
{
  a = _varString;
  std::cout << "Test String Reference Out: " << a << std::endl;
}

void TypeClass::testStringRefDef(const String& a, const String& b)
{
  _varString = a;
  std::cout << "Test String Reference Def: " << a << " - " << b << std::endl;
}

