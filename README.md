# srtn-calc

A SRTN calculator gui application made for ICSC0113.

![srtn-calc-example](https://github.com/lhyvi/srtn-calc/assets/36299204/94617e39-c7bd-4213-a919-958c744686ce)

srtn-calc is a program that calculates Shortest Remaining Time Next (SRTN) for a given set of processes. It provides a user-friendly interface for inputting process details and displays the Gantt chart, Waiting Time, and Turn-Around Time for each process.

# Build Dependencies

The following dependencies are required to build the program on Windows:
- CMake 3.14.0 or later
- [vcpkg](https://github.com/microsoft/vcpkg)
- Visual Studio 2022

# Build Instructions

**Windows Build Instructions:**

Clone the repository:
```bat
git clone https://github.com/lhyvi/srtn-calc.git
```
Navigate to the project directory:
```bat
cd srtn-calculator
```
Run CMake to generate the build files with the "release-windows" preset:
```bat
cmake . --preset "release-windows"
```
Navigate to the build directory:
```bat
cd build
```
Build the project using the generated build files:
```bat
cmake --build . --config Release
```
After building the project, the built binary will be located in the `Release/` folder.
