
// src/UserAuth.cpp
#include "UserAuth.h"
#include "Utility.h" // For clearScreen(), pressEnterToContinue()
#include <iostream>
#include <fstream>
#include <algorithm> // For std::find_if
#include <random>    // For std::mt19937, std::uniform_int_distribution
#include <chrono>    // For std::chrono::system_clock
#include <limits>    // For std::numeric_limits
#include <iomanip>   // For std::fixed, std::setprecision

// Initialize static members
std::vector<Account> UserAuth::accounts;
const std::string UserAuth::ACCOUNTS_FILE = "data/accounts.dat";

// Helper function to generate a unique 10-digit account number
std::string UserAuth::generateAccountNumber() {
    std::string newAccNum;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 9);

    bool unique = false;
    while (!unique) {
        newAccNum = "";
        for (int i = 0; i < 10; ++i) {
            newAccNum += std::to_string(distrib(gen));
        }

        // Check if this account number already exists
        unique = true; // Assume unique until proven otherwise
        for (const auto& acc : accounts) {
            if (acc.getAccountNumber() == newAccNum) {
                unique = false;
                break;
            }
        }
    }
    return newAccNum;
}

// Register a new user and create an account (updated for account type)
bool UserAuth::registerUser() {
    clearScreen();
    std::cout << "--- Register New Account ---" << std::endl;
    std::string ownerName, pin1, pin2;
    double initialDeposit;
    int accountTypeChoice;
    AccountType selectedAccountType = AccountType::UNKNOWN;

    std::cout << "Enter your full name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
    std::getline(std::cin, ownerName);

    // Get PIN and confirm it
    while (true) {
        std::cout << "Enter a 4-digit PIN: ";
        std::cin >> pin1;
        std::cout << "Confirm PIN: ";
        std::cin >> pin2;

        if (pin1.length() == 4 && pin2.length() == 4 && pin1 == pin2) {
            break;
        } else {
            std::cout << "PINs do not match or are not 4 digits. Please try again." << std::endl;
        }
    }

    // Select Account Type
    while (true) {
        std::cout << "\nSelect Account Type:" << std::endl;
        std::cout << "1. Savings Account" << std::endl;
        std::cout << "2. Current Account" << std::endl;
        std::cout << "3. Fixed Deposit Account (FD)" << std::endl;
        std::cout << "4. Recurring Deposit Account (RD)" << std::endl;
        std::cout << "5. Loan Account" << std::endl;
        std::cout << "6. Joint Account" << std::endl;
        std::cout << "7. Business Account" << std::endl;
        std::cout << "8. Student Account" << std::endl;
        std::cout << "9. Salary Account" << std::endl;
        std::cout << "Enter choice (1-9): ";
        std::cin >> accountTypeChoice;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (accountTypeChoice) {
            case 1: selectedAccountType = AccountType::SAVINGS; break;
            case 2: selectedAccountType = AccountType::CURRENT; break;
            case 3: selectedAccountType = AccountType::FIXED_DEPOSIT; break;
            case 4: selectedAccountType = AccountType::RECURRING_DEPOSIT; break;
            case 5: selectedAccountType = AccountType::LOAN; break;
            case 6: selectedAccountType = AccountType::JOINT; break;
            case 7: selectedAccountType = AccountType::BUSINESS; break;
            case 8: selectedAccountType = AccountType::STUDENT; break;
            case 9: selectedAccountType = AccountType::SALARY; break;
            default:
                std::cout << "Invalid account type choice. Please select a number between 1 and 9." << std::endl;
                continue; // Ask again
        }
        break; // Exit loop if a valid choice is made
    }


    // Get initial deposit
    while (true) {
        std::cout << "Enter initial deposit amount (minimum TK: 0.01): TK. ";
        std::cin >> initialDeposit;
        if (std::cin.fail() || !isValidAmount(initialDeposit)) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid amount. Please enter a positive number." << std::endl;
        } else {
            break;
        }
    }

    std::string newAccNum = generateAccountNumber();
    // Pass the selected account type to the Account constructor
    Account newAccount(newAccNum, pin1, initialDeposit, ownerName, selectedAccountType);
    accounts.push_back(newAccount);
    saveAccounts(); // Save the new account immediately

    std::cout << "\nAccount created successfully!" << std::endl;
    std::cout << "Your Account Number is: " << newAccNum << std::endl;
    std::cout << "Account Type: " << accountTypeToString(selectedAccountType) << std::endl;
    std::cout << "Initial Balance: TK. " << std::fixed << std::setprecision(2) << initialDeposit << std::endl;
    pressEnterToContinue();
    return true;
}

// Log in an existing user
Account* UserAuth::loginUser() {
    clearScreen();
    std::cout << "--- Login ---" << std::endl;
    std::string accNum, pin;

    std::cout << "Enter Account Number: ";
    std::cin >> accNum;
    std::cout << "Enter PIN: ";
    std::cin >> pin;

    // Find the account
    auto it = std::find_if(accounts.begin(), accounts.end(),
                           [&](const Account& acc) { return acc.getAccountNumber() == accNum; });

    if (it != accounts.end()) {
        // Account found, now authenticate PIN
        if (it->authenticate(pin)) {
            std::cout << "\nLogin successful! Welcome, " << it->getOwnerName() << "." << std::endl;
            pressEnterToContinue();
            return &(*it); // Return pointer to the logged-in account
        } else {
            std::cout << "\nIncorrect PIN. Please try again." << std::endl;
        }
    } else {
        std::cout << "\nAccount not found. Please check your account number." << std::endl;
    }
    pressEnterToContinue();
    return nullptr; // Login failed
}

// Load all accounts from the binary file
void UserAuth::loadAccounts() {
    std::ifstream ifs(ACCOUNTS_FILE, std::ios::binary);
    if (!ifs.is_open()) {
        std::cout << "No existing accounts file found. Starting with empty accounts." << std::endl;
        return;
    }

    accounts.clear(); // Clear existing accounts before loading
    while (ifs.peek() != EOF) { // Check for end of file
        Account acc;
        acc.loadFromFile(ifs);
        accounts.push_back(acc);
    }
    ifs.close();
    std::cout << "Accounts loaded successfully." << std::endl;
}

// Save all accounts to the binary file
void UserAuth::saveAccounts() {
    std::ofstream ofs(ACCOUNTS_FILE, std::ios::binary | std::ios::trunc); // trunc to overwrite
    if (!ofs.is_open()) {
        std::cerr << "Error: Could not open accounts file for writing." << std::endl;
        return;
    }

    for (const auto& acc : accounts) {
        acc.saveToFile(ofs);
    }
    ofs.close();
    std::cout << "Accounts saved successfully." << std::endl;
}

// Find an account by account number
Account* UserAuth::findAccount(const std::string& accNum) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
                           [&](const Account& acc) { return acc.getAccountNumber() == accNum; });
    if (it != accounts.end()) {
        return &(*it);
    }
    return nullptr;
}

// Get a reference to the accounts vector
std::vector<Account>& UserAuth::getAccounts() {
    return accounts;
}
