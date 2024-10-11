#pragma once

#include "swigex0_export.hpp"
#include <vector>
#include <string>
#include <fstream>

SWIGEX0_EXPORT int fibn(int n);
SWIGEX0_EXPORT std::vector<int> fib(int n);

/**
 * Class which handles Fibonacci integers list
 */
class SWIGEX0_EXPORT Fibo
{
  public:
    Fibo(int n=3, const std::string& title = "Valeur par defaut");
    virtual ~Fibo();

    static Fibo* create(int n = 5, const std::string& title = "") { 
      return new Fibo(n, title); }

    void resetFromFiboVal(Fibo& fib);
    void resetFromFiboRef(const Fibo& fib);

    void display(bool showTitle = true) const;

    void setValue(int n) { _n = n; }
    int  getValue() const { return _n; }

    std::vector<int> getVector() const;
    std::string getTitle() const;

  protected:
    int         _n;     ///< Maximum integer of the list
    std::string _title; ///< Title to be shown when displaying the list
};

/**
 * Standard output stream redirection
 */
class SWIGEX0_EXPORT StdoutRedirect {
  public:
    StdoutRedirect(const std::string& file = "");
    ~StdoutRedirect();
    StdoutRedirect(const StdoutRedirect&) = delete;
    StdoutRedirect& operator=(const StdoutRedirect&) = delete;

    void start(const std::string& file);
    void stop();

  private:
  #if defined(_WIN32) || defined(_WIN64)
    // HANDLE _old_stdout;
    void* _old_stdout;
  #else
    std::streambuf* _coutbuf;
    std::ofstream _out;
  #endif
};
