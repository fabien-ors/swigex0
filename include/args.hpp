#pragma once

#include "swigex0_export.hpp"
#include "swigex0_define.hpp"

class SWIGEX0_EXPORT TypeClass
{
public:
  TypeClass();
  ~TypeClass();
//                                                                                     //   R         Python

  int testInt(int a);                                                                  //   OK        OK
  const int& testIntRef(const int& a);                                                 //   OK        OK
  const int* testIntPtr(const int* a);                                                 //   ## NOK    OK
  void testIntRefOut(int& a) const;                                                    //   ## NOK    ## NOK
  void testIntRefDef(const int& a = 2, const int& b = 3);                              //   OK        OK

  double testDouble(double a);                                                         //   OK        OK
  const double& testDoubleRef(const double& a);                                        //   OK        OK
  const double* testDoublePtr(const double* a);                                        //   ## NOK    OK
  void testDoubleRefOut(double& a) const;                                              //   ## NOK    ## NOK
  void testDoubleRefDef(const double& a = 2.0, const double& b = 3.0);                 //   OK        OK

  String testString(String a);                                                         //   OK        OK
  const String& testStringRef(const String& a);                                        //   OK        OK
  const String*testStringPtr(const String* a);                                         //   ## NOK    ## NOK
  void testStringRefOut(String& a) const;                                              //   ## NOK    ## NOK
  void testStringRefDef(const String& a = "Str2", const String& b = "Str3");           //   OK        OK

private:
  int                _varInt;
  double             _varDouble;
  String             _varString;
};
