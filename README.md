# threads-integral
Parallel processing an integral problem using C++ threads and mutex.

File Descriptions:
- integrate-threads.cpp:  C++ code that performs the calculation
- xint_threads.exe:  Executable file
- .png files:  Plots of the numerical error as a function of the number of plots and the strong scaling efficiency as a function of the number of threads.

**Compilation details:**

g++ -std=c++14 -o xint_threads integrate-threads.cpp


**Running the program**
./xint_threads <num points> <num threads>
