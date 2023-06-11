#ifndef LARGE_HEADER_H
#define LARGE_HEADER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

// Constants
constexpr int large_constant_1 = 12345;
constexpr double large_constant_2 = 3.14159;
constexpr char large_constant_3[] = "This is a large constant string.";

// Large array
int large_array[1024*1024] = {0};

// Function prototypes
void large_function_1();
int large_function_2(int a, int b);
std::string large_function_3(const std::string& str);

// Large number of variables
extern int large_var_1;
extern double large_var_2;
extern std::string large_var_3;
// ... Add more variables as needed ...

// Large number of functions
inline void large_function_4() { std::cout << "Large function 4" << std::endl; }
inline int large_function_5(int a, int b) { return a * b; }
inline std::string large_function_6(const std::string& str) { return str + " (modified by large_function_6)"; }
// ... Add more functions as needed ...

#endif // LARGE_HEADER_H