# swigex0 C++ Library and Wrappers

* Author: Fabien Ors (MINES PARIS - PSL University) 
* Date: Feb. 2023

Example of a cross-platform C++ library exported in Python and R using SWIG and CMake. Following features are tested:
* Non regression tests in C++, Python and R (using CTest and automatic github actions)
* Python and R packages local installation through CMake
* Python and R packages deployment on TestPyPi and CRAN (not yet available) (using manual github actions)
* Simple C++ class export (see testFibo)
* Use of typemaps for testing *arguments* wrapping (see testArgs):
  * For C++ types: int, double, std::string
  * Passed by input value, input reference, input pointers and output pointers
  * Possibly having C++ default values
* *Inheritance* in target language (see testPolymorph) (still in development)

A lot of CMake and SWIG instructions in this project have been used to solve issues I encountered. There certainly exist smarter and simpler ways to accomplish all the stuff. Feel free to suggest any simplifications in order to make swigex0 as simple as possible!

Look for 'TODO' keyword for remaining issues.

## Prerequisites

This library has been successfully tested with Ubuntu 16/18/20 LTS and Windows 10 (MacOS: not tested).

For compiling and installing *swigex0* C++ Library, the following tools must be available (See [required tools installation](#required-tools-installation) instructions below):

* Git client
* CMake tool 3.20 or higher
* A C++ compiler:
  * Linux:
    * GCC compiler 5.4 or higher
  * Windows:
    * Microsoft Visual Studio C++ 14 or higher
    * MinGW 7 or higher (we propose the one provided with RTools)
  * MacOS (not tested):
    * Clang 12 or higher

The following tools must be available for compiling and installing Python package:
* SWIG 4 or higher
* Python 3 or higher with *pip* modules installed

The following tools must be available for compiling and installing R package:
* SWIG 4.2.0 **customized by Fabien Ors** (not the official version!)
* R 4 or higher
* RTools 4 for Windows user

See [required tools installation](#required-tools-installation) instructions below.

## Get the sources

For getting the sources files, just clone the github repository:

    git clone https://github.com/fabien-ors/swigex0
    cd swigex0

Notes:

* In the following, all instructions must be executed from a command prompt inside this *root* directory (thus the last command `cd swigex0` above)

## Usage

First, check that you can compile / test / install... swigex0. Then, look at how to adapt this project to your needs in the [next section](#derivative-usage)

### Configure project
Depending on the package you want to build you must adapt the command below:
#### GCC / MinGW / CLang
    cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Release -DBUILD_PYTHON=ON -DBUILD_R=ON
#### MSVC (Visual)
    cmake -Bbuild -H. -DBUILD_PYTHON=ON -DBUILD_R=ON

### Important Notes

* Using MingGW on a Windows where Visual Studio is also installed may need to add `-G "MSYS Makefiles"` in the command above.
* The default installation directory named *swigex0_install* is located in your *Home*. If you want to change it, you can either:
    * Define the `swigex0_INSTALL_DIR` environment variable or
    * Add `-Dswigex0_INSTALL_DIR=<path/of/swigex0/install/dir>` to the first cmake command above
* If you want to build and install the *Debug* version, you must replace `Release` by `Debug` above (GCC/MinGW) and below (MSVC)
* The *static* version of the library is mandatory for creating Python and R packages
* Only the *shared* library (built by default) is installed by the 'install' target

### Build static library (and install Python package)
#### GCC / MinGW / CLang
    cmake --build build --target python_install
#### MSVC (Visual)
    cmake --build build --target python_install --config Release

### Build static library (and install R package)
#### GCC / MinGW / CLang
    cmake --build build --target r_install
#### MSVC (Visual)
    cmake --build build --target r_install --config Release

### Build and install shared library (and don't build Python and R packages)
#### GCC / MinGW / CLang
    cmake --build build --target install
#### MSVC (Visual)
    cmake --build build --target install --config Release

### Execute non-regression tests (C++, Python and R when relevant)
#### GCC / MinGW / CLang
    cmake --build build --target check
##### MSVC (Visual)
    cmake --build build --target check --config Release


## Derivative Usage

The objective of this package is to provide an example of wrapping a C++ library in R and Python using CMake and SWIG.
Here are swigex0 adaptation instructions (See also shortcuts for 'make' users in *Makefile* file):

### For generating your C++ library:

1. Clone the repository
2. Rename swigex0 root folder with your project name (i.e. let's call it 'myproject')
3. Update the root Readme.md file to your needs
4. In the root CMakeLists.txt file:
  1. Adapt the header comment to your needs
  2. Adapt the project name, the VERSION, DESCRIPTION and HOMEPAGE_URL (line 45)
5. In version.h.in file: Replace swigex0 by the name of your projet 
6. Replace the content of 'include' directory with your C++ header files
7. Replace the content of 'src' directory with your C++ body files
8. In cmake/cpp.cmake file:
  1. Adapt C++ body files list (line 27)
  2. If needed, adapt include directories (line 24)

=> you already can test the following targets (on windows, linux or mac, with or without DEBUG=1) :
  - static
  - shared
  - install
  - uninstall
  - clean
  - clean_all
  
Note:
 - You may need to adapt cmake/cpp.cmake to add external library dependencies or compilator options. Please refer to CMake user guide.


### Adapting your C++ code:

1. Include myproject_export.hpp file and export symbols using MYPROJECT_EXPORT macro
2. Include version.h file for accessing to MYPROJECT_RELEASE and MYPROJECT_DATE macros


### For generating python and R Packages:

1. In swig/swig_exp.i:
  1. Adapt the C++ header files list with the headers you want to export to python and R
  2. Replace swigex0 by the name of your project (in swigex0_export.hpp)
2. In swig/swig_inc.i:
  1. Adapt the C++ header files list with the headers needed for compiling (bigger list than in 9a)
  2. Replace swigex0 by the name of your project (in swigex0_export.hpp)
  3. Add SWIG typemaps needed
3. In python folder:
  1. Rename interface file pyswigex0.i file into pymyproject.i
  2. In file pymyproject.i, rename module swigex0 to myproject (without 'py' prefix)
  3. In file CMakeLists.txt, change interface filename to pymyproject.i (line 24)
4. In r folder:
  1. Rename interface file rswigex0.i file into rmyproject.i
  2. In file rmyproject.i, rename module swigex0 to myproject (without 'r' prefix)
  3. In file CMakeLists.txt, change interface filename to rmyproject.i (line 28)

=> you already can test the following targets (on windows, linux or mac) :
  - python_install
  - r_install
  

### For adding non-regression tests:

1. In tests/cpp folder:
  1. Ensure that your C++ test executables are able to generate an output log file (see example in testFibo.cpp)
  2. Remove testFibo.cpp and paste here your source files for C++ test that link to myproject library
  3. Update test files list in CMakeLists.txt (line 2)
  4. Run check_cpp target and verify the generated log files in 'build' directory (in build/tests/cpp/Release)
  5. Remove testFibo.ref file and copy generated logs into the tests/cpp folder (replace 'out' extension to 'ref')
2. In tests/py folder:
  1. Ensure that your python scripts print some useful information
  2. Remove testFibo.py and paste here your python scripts that import myproject python module
  3. Update test files list in CMakeLists.txt (line 6)
  4. Run check_py target and verify the generated log files in 'build' directory (in build/tests/py/Release)
  5. Remove testFibo.ref file and copy generated logs into the tests/py folder (replace 'out' extension to 'ref')
3. In tests/r folder:
  1. Ensure that your R scripts print some useful information
  2. Remove testFibo.r and paste here your R scripts that load myproject R package
  3. Update test files list in CMakeLists.txt (line 6)
  4. Run check_r target and verify the generated log files in 'build' directory (in build/tests/r/Release)
  5. Remove testFibo.ref file and copy generated logs into the tests/r folder (replace 'out' extension to 'ref')

=> you can test the following targets (on Widows, linux or mac):
  - build_tests
  - check_cpp
  - check_py
  - check_r
  - check (which run all 3 previous targets)
 
Note:
 - Output logs must be reproducible. For example, if you use some random numbers generation, you will have to set a seed.
 
## Required tools installation

Depending on the package you want to build, all dependencies are not mandatory (R and Python)

### Linux (Ubuntu)

1. (For R users only) - Remove any previous installation of SWIG (if any)

2. Then, execute the following commands:

````
sudo apt install git
sudo apt install cmake
sudo apt install r-base
sudo apt install python3
sudo apt install python3-pip
sudo apt install bison
sudo apt install pcre2-devel # Ubuntu 18
sudo apt install libpcre2-dev # Ubuntu 20
python3 -m ensurepip --upgrade
````

3. In a folder of your own, compile and install SWIG 4.2.0 [customized] by executing following commands:

````
git clone https://github.com/fabien-ors/swig.git
cd swig
cmake -Bbuild
cd build
make
sudo make install
````

Notes:

* If you don't have sudo permissions, you may have to install swig in a folder of your choice. In that case, use `-DCMAKE_INSTALL_PREFIX:PATH=/home/user/Programs` (adapt installation folder) in the `cmake` command above.
* Under Linux, the GCC compiler and GNU make is already installed
* If your Linux distribution repositories don't provide minimum required versions, please install the tools manually (see provider website)

### MacOS

1. (For R users only) - Remove any previous installation of SWIG (if any)

2. Then, execute the following commands (Not tested):

````
brew install git
brew install cmake
brew install r
brew install bison
brew install pcre2-devel
brew install python3
````

3. In a folder of your own, compile and install SWIG 4.2.0 [customized] by executing following commands:

````
git clone https://github.com/fabien-ors/swig.git
cd swig
cmake -Bbuild
cd build
make
sudo make install
````

Notes:

* If you don't have sudo permissions, you may have to install swig in a folder of your choice. In that case, use `-DCMAKE_INSTALL_PREFIX:PATH=/home/user/Programs` (adapt installation folder) in the `cmake` command above.
* Under MacOS, the GCC (or Clang) compiler and GNU make is already installed
* If your MacOS repositories don't provide minimum required versions, please install the tools manually (see provider website)
  
### Windows - Microsoft Visual Studio

These requirements are also recommended to people who wants to compile *swigex0* Python package. If you want to compile *swigex0* R package under Windows, you should look at the next section.

Download and install the following tools using default options during installation:

* Git client [from here](https://gitforwindows.org) (*Setup program* [exe])
* CMake tool [from here](https://cmake.org/download) (*Windows Installer* [msi], check the *'Add CMake to the system PATH for all users'* option during installation)
* Microsoft Visual Studio (Community) [from here](https://visualstudio.microsoft.com/fr/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false) (*VisualStudioSetup.exe* - only select the *Visual Studio Desktop C++* component)
* Python 3+ [from here](https://www.python.org/downloads) (*Windows installer* [exe] - check 'Add python.exe to PATH' in the first panel)
* SWIG 4+ [from here](http://www.swig.org/download.html) (*swigwin archive* [zip], Archive file [zip] to be extracted in a folder of your own - and remind that folder)

Notes:

* The *Path* environment variable (**System variables**) must be updated to make *swig.exe* available in the batch command line (follow [this guide](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10) to add *SWIG* installation folders in the *Path* variable and restart Windows)
* The Windows C++ Compiler used must be the same that the one used for compiling Python (Visual C++). Using another compiler than Visual C++ is not supported.
* You must restart your computer after installing these requirements

### Windows - MinGW (via RTools)

These requirements are also recommended to people who wants to compile *swigex0* R package. If you want to compile *swigex0* Python package under Windows, you should look at the previous section. This is not the only way to install MinGW. But using MinGW provided with RTools permits us to also handle *swigex0* R package compilation.

#### Install R and RTools

Download and install the following tools using default option during installation:

* R [from here](https://cran.r-project.org/bin/windows/base) (*Setup program* [exe] - remind the installation folder, assume it is `C:\Program Files\R\R-4.2.2`)
* RTools [from here](https://cran.r-project.org/bin/windows/Rtools) (RTools *Installer* [exe] - remind the installation folder, assume it is `C:\rtools42`)

Notes:

* Choose the corresponding RTools version according to the R version installed
* Instructions in this section are **valid since R v4.2** (for older versions please contact us)
* RTools is not the unique way to install MinGW on Windows, but it is our preferred way as we can handle R packages compilation
* The *Path* environment variable (*System variables*) must be updated to make *R.exe* available in the batch command line (follow [this guide](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10) to add `bin` directory from the *R* installation folder in the *Path* variable and restart Windows, ie: `C:\Program Files\R\R-4.2.2\bin`)

#### Add MSYS2 Required Packages

1. Edit the `etc/pacman.conf` file in the RTools installation directory (ie: `C:\rtools42`) by changing the `SigLevel` variable to `Never` (otherwise, *git* cannot be installed using *pacman*):

````
SigLevel=Never
````

2. Edit the `mingw64.ini` file in the RTools installation directory (ie: `C:\rtools42`) by un-commenting the following line (remove '#' character at the beginning):

````
MSYS2_PATH_TYPE=inherit
````

3. Launch *mingw64.exe* in RTools installation directory (ie: `C:\rtools42`) and pin the icon to the task bar

4. From the *mingw64* shell command prompt, execute following instructions:

````
pacman -Sy git
pacman -Sy mingw-w64-x86_64-cmake
pacman -Sy mingw-w64-x86_64-gcc
pacman -Sy bison
pacman -Sy mingw-w64-x86_64-pcre2
````

#### Install customized SWIG 4.2.0

In a directory of your own, compile and install SWIG 4.2.0 [customized] by executing following commands from the *mingw64* shell command prompt:

````
git clone https://github.com/fabien-ors/swig.git
cd swig
cmake -G "MSYS Makefiles" -Bbuild -DCMAKE_INSTALL_PREFIX:PATH=/mingw64/
cd build
make
make install
````

---

## License
MIT
2023 Fabien Ors
