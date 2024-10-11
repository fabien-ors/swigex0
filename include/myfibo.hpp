#pragma once

#include "swigex0_export.hpp"
#include <string>
#include <vector>

class SWIGEX0_EXPORT MyFibo
{
  public:
    MyFibo(int n=3, const std::string& title = "Default");
    virtual ~MyFibo();

    void display() const;
    void setValue(int n) { _n = n; };

    static MyFibo* create(int n = 5, const std::string& title = "create string") { 
      return new MyFibo(n, title); }

    static void workInPlaceRef(int nincr, MyFibo& myfibo);
    static void workInPlacePtr(int nincr, MyFibo* myfibo);

  protected:
    int _n;
    std::string _title;
};

