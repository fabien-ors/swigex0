#include "stdoutredirect.hpp"
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

StdoutRedirect::StdoutRedirect(const String& file) :
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
void StdoutRedirect::start(const String& file)
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
