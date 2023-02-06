#pragma once

#include <string>
#include <iostream>
#include <cmath>
#include <math.h>
typedef std::string String;

#define DEFAULT_TITLE "Fibonacci List"

#ifndef SWIG

#define INT_NA    -999
#define DOUBLE_NA -999.999
#define STRING_NA "NA"

template <typename T> inline T getNA();
template <> inline double getNA() { return DOUBLE_NA; }
template <> inline int    getNA() { return INT_NA; }
template <> inline String getNA() { return STRING_NA; }

template <typename T> inline bool isNA(const T& v);
template <> inline bool isNA(const double& v) { return (v == getNA<double>() || std::isnan(v) || std::isinf(v)); }
template <> inline bool isNA(const int& v)    { return (v == getNA<int>()); }
template <> inline bool isNA(const String& v) { return (v == getNA<String>()); }

#endif // SWIG
