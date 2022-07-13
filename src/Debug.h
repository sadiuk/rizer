#pragma once
#include <iostream>
#include <chrono>
#define LOG(x) (std::cout << x << "\n")

#define TEST_TIMER_START(name) {\
std::string n = name;\
double timer_start = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000000.;

#define TEST_TIMER_END()double timer_now = std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000000.;\
double diff = timer_now - timer_start;\
LOG(std::string(n) + " took " + std::to_string(diff) + " ms.");\
}

