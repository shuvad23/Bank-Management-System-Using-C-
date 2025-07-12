// src/main.cpp
#include "Account.h"
#include "UserAuth.h"
#include "Transaction.h"
#include "Utility.h"
#include <iostream>
#include <limits>   // Required for std::numeric_limits
#include <iomanip>  // Required for std::fixed and std::setprecision

// Function prototypes for menu options
void displayMainMenu();
void displayAccountMenu(Account* loggedInAccount);

int main() {
    // Ensure the data directory exists
    // This is a simple check; a more robust solution might use boost::filesystem or C++17 std::filesystem
#ifdef _WIN32
    system("mkdir data >nul 2>&1"); // Create data directory quietly on Windows
#else
    system("mkdir -p data >/dev/null 2>&1"); // Create data directory 
#endif

    // Load existing accounts when the program starts
    UserAuth::loadAccounts();

    Account* currentLoggedInAccount = nullptr; // Pointer to the currently logged-in account

    int choice;
    do {
        clearScreen();
        displayMainMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // Input validation for choice
        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number." << std::endl;
            pressEnterToContinue();
            continue;
        }

        switch (choice) {
            case 1: // Register
                UserAuth::registerUser();
                break;
            case 2: // Login
                currentLoggedInAccount = UserAuth::loginUser();
                if (currentLoggedInAccount) {
                    // If login is successful, enter the account menu loop
                    displayAccountMenu(currentLoggedInAccount);
                    currentLoggedInAccount = nullptr; // Reset after logout
                }
                break;
            case 3: // Exit
                std::cout << "Exiting Bank Management System. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                pressEnterToContinue();
                break;
        }
    } while (choice != 3);

    // Save all accounts before exiting (important for persistence)
    UserAuth::saveAccounts();

    return 0;
}

// Displays the main menu options
void displayMainMenu() {
    std::cout << "--- Bank Management System ---" << std::endl;
    std::cout << "1. Register New Account" << std::endl;
    std::cout << "2. Login" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "------------------------------" << std::endl;
}

// Displays the menu for a logged-in account
void displayAccountMenu(Account* loggedInAccount) {
    if (!loggedInAccount) {
        std::cout << "Error: No account logged in." << std::endl;
        pressEnterToContinue();
        return;
    }

    int choice;
    do {
        clearScreen();
        std::cout << "--- Welcome, " << loggedInAccount->getOwnerName() << " ---" << std::endl;
        loggedInAccount->displayAccountInfo(); // Show current balance and account type
        std::cout << "\n1. Deposit Funds" << std::endl;
        std::cout << "2. Withdraw Funds" << std::endl;
        std::cout << "3. View Account Statement" << std::endl;
        std::cout << "4. Logout" << std::endl;
        std::cout << "------------------------------" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // Input validation for choice
        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number." << std::endl;
            pressEnterToContinue();
            continue;
        }

        double amount;
        Transaction trans;

        switch (choice) {
            case 1: // Deposit
                std::cout << "Enter amount to deposit: TK. ";
                std::cin >> amount;
                if (std::cin.fail() || !isValidAmount(amount)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid amount. Please enter a positive number." << std::endl;
                } else {
                    if (loggedInAccount->deposit(amount)) {
                        std::cout << "Deposit successful. New balance: TK. " << std::fixed << std::setprecision(2) << loggedInAccount->getBalance() << std::endl;
                        trans = Transaction(loggedInAccount->getAccountNumber(), "Deposit", amount, getCurrentDate());
                        logTransaction(trans);
                        UserAuth::saveAccounts(); // Save updated balance
                    } else {
                        std::cout << "Deposit failed. Amount must be positive." << std::endl;
                    }
                }
                pressEnterToContinue();
                break;
            case 2: // Withdraw
                std::cout << "Enter amount to withdraw: TK. ";
                std::cin >> amount;
                if (std::cin.fail() || !isValidAmount(amount)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid amount. Please enter a positive number." << std::endl;
                } else {
                    if (loggedInAccount->withdraw(amount)) {
                        std::cout << "Withdrawal successful. New balance: TK. " << std::fixed << std::setprecision(2) << loggedInAccount->getBalance() << std::endl;
                        trans = Transaction(loggedInAccount->getAccountNumber(), "Withdrawal", amount, getCurrentDate());
                        logTransaction(trans);
                        UserAuth::saveAccounts(); // Save updated balance
                    } else {
                        std::cout << "Withdrawal failed. Insufficient funds or invalid amount." << std::endl;
                    }
                }
                pressEnterToContinue();
                break;
            case 3: // View Account Statement
                viewAccountStatement(loggedInAccount->getAccountNumber());
                pressEnterToContinue();
                break;
            case 4: // Logout
                std::cout << "Logging out. Goodbye, " << loggedInAccount->getOwnerName() << "!" << std::endl;
                pressEnterToContinue();
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                pressEnterToContinue();
                break;
        }
    } while (choice != 4);
}
