# srtn-calc

A SRTN calculator gui application made for ICSC0113.

![srtn-calc-example](https://github.com/lhyvi/srtn-calc/assets/36299204/94617e39-c7bd-4213-a919-958c744686ce)

srtn-calc is a program that calculates Shortest Remaining Time Next (SRTN) for a given set of processes. It provides a user-friendly interface for inputting process details and displays the Gantt chart, Waiting Time, and Turn-Around Time for each process.

# Build Dependencies

The following dependencies are required to build the program on Windows:
- CMake 3.14.0 or later
- [vcpkg](https://github.com/microsoft/vcpkg)

Library dependencies, are specified in the vcpkg.json file. You can choose to use vcpkg for automatic installation or install them manually.

# Building with VCPKG

```bat
git clone https://github.com/lhyvi/srtn-calc.git
cd srtn-calc
```
For Windows:
```
cmake -B build -S . -D "CMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"
```
For Unix-based:
```
cmake -B build -S . -D "CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```
Build the project using the generated build files:
```bat
cmake --build build
```
