// src/Utility.cpp
#include "Utility.h"
#include <chrono>   // For std::chrono::system_clock, std::chrono::duration_cast
#include <ctime>    // For std::time_t, std::localtime, std::mktime, std::strftime
#include <iomanip>  // For std::put_time

// Function to get the current date as a string in YYYY-MM-DD format
std::string getCurrentDate() {
    // Get the current time point
    auto now = std::chrono::system_clock::now();
    // Convert to time_t
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    // Convert to tm structure (local time)
    std::tm* localTime = std::localtime(&currentTime);

    // Format the time into a string
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S"); // Include time for more detail
    return oss.str();
}

// Function to validate if an amount is positive
bool isValidAmount(double amount) {
    return amount > 0;
}

// Function to clear the console screen
// This is platform-dependent. For Windows, use "cls". For Unix/Linux/macOS, use "clear".
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // Assume POSIX-like system
    system("clear");
#endif
}

// Function to pause execution until user presses Enter
void pressEnterToContinue() {
    std::cout << "\nPress Enter to continue...";
    // Clear any remaining characters in the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get(); // Wait for Enter key
}
