// Helper functions (date, validation)

// include/Utility.h
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <iostream> // For std::cin, std::cout, std::endl
#include <limits>   // For std::numeric_limits

// Function to get the current date as a string
std::string getCurrentDate();

// Function to validate if an amount is positive
bool isValidAmount(double amount);

// Function to clear the console screen (platform-dependent)
void clearScreen();

// Function to pause execution until user presses Enter
void pressEnterToContinue();

#endif // UTILITY_H