#pragma once

#include "swigex0_export.hpp"
#include "swigex0_define.hpp"
#include <vector>

/// Global functions
SWIGEX0_EXPORT int fibn(int n);
SWIGEX0_EXPORT std::vector<int> fib(int n);

/**
 * Class which handles Fibonacci integers list
 */
class SWIGEX0_EXPORT Fibo
{
  public:
    Fibo (int n, const String& title = "");
    virtual ~Fibo();

    void resetFromFiboVal(Fibo fib);        // In order to test class argument as value
    void resetFromFiboRef(const Fibo& fib); //  In order to test class argument as reference

    void display(bool showTitle = true) const;

    std::vector<int> get() const;

  protected:
    int    _n;     /// Maximum integer of the list
    String _title; /// Title to be shown when displaying the list
};

