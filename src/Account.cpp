
// src/Account.cpp
#include "Account.h"
#include <iostream>
#include <limits>   // Required for std::numeric_limits
#include <iomanip>  // Required for std::fixed and std::setprecision
#include <map>      // For mapping enum to string

// Helper function to write a string to a binary file
void writeString(std::ofstream& ofs, const std::string& str) {
    size_t len = str.length();
    ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
    ofs.write(str.c_str(), len);
}

// Helper function to read a string from a binary file
std::string readString(std::ifstream& ifs) {
    size_t len;
    ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
    std::string str(len, ' '); // Pre-allocate string with correct size
    ifs.read(&str[0], len);    // Read directly into string buffer
    return str;
}

// Helper function to convert AccountType enum to string
std::string accountTypeToString(AccountType type) {
    switch (type) {
        case AccountType::SAVINGS: return "Savings Account";
        case AccountType::CURRENT: return "Current Account";
        case AccountType::FIXED_DEPOSIT: return "Fixed Deposit Account";
        case AccountType::RECURRING_DEPOSIT: return "Recurring Deposit Account";
        case AccountType::LOAN: return "Loan Account";
        case AccountType::JOINT: return "Joint Account";
        case AccountType::BUSINESS: return "Business Account";
        case AccountType::STUDENT: return "Student Account";
        case AccountType::SALARY: return "Salary Account";
        default: return "Unknown Account Type";
    }
}

// Helper function to convert string to AccountType enum
AccountType stringToAccountType(const std::string& typeStr) {
    if (typeStr == "Savings Account") return AccountType::SAVINGS;
    if (typeStr == "Current Account") return AccountType::CURRENT;
    if (typeStr == "Fixed Deposit Account") return AccountType::FIXED_DEPOSIT;
    if (typeStr == "Recurring Deposit Account") return AccountType::RECURRING_DEPOSIT;
    if (typeStr == "Loan Account") return AccountType::LOAN;
    if (typeStr == "Joint Account") return AccountType::JOINT;
    if (typeStr == "Business Account") return AccountType::BUSINESS;
    if (typeStr == "Student Account") return AccountType::STUDENT;
    if (typeStr == "Salary Account") return AccountType::SALARY;
    return AccountType::UNKNOWN;
}


// Default constructor
Account::Account() : accountNumber(""), pin(""), balance(0.0), ownerName(""), accountType(AccountType::UNKNOWN) {}

// Parameterized constructor (updated to include accountType)
Account::Account(const std::string& accNum, const std::string& p, double bal, const std::string& name, AccountType type)
    : accountNumber(accNum), pin(p), balance(bal), ownerName(name), accountType(type) {}

// Getters
std::string Account::getAccountNumber() const {
    return accountNumber;
}

std::string Account::getPin() const {
    return pin;
}

double Account::getBalance() const {
    return balance;
}

std::string Account::getOwnerName() const {
    return ownerName;
}

// New getter for account type
AccountType Account::getAccountType() const {
    return accountType;
}

// Setter for balance (used internally by deposit/withdraw)
void Account::setBalance(double newBalance) {
    balance = newBalance;
}

// Deposit funds into the account
bool Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        return true;
    }
    return false;
}

// Withdraw funds from the account
bool Account::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

// Authenticate the account with a given PIN
bool Account::authenticate(const std::string& enteredPin) const {
    return pin == enteredPin;
}

// Display account information (updated to include account type)
void Account::displayAccountInfo() const {
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Owner Name:     " << ownerName << std::endl;
    std::cout << "Account Type:   " << accountTypeToString(accountType) << std::endl; // Display account type
    std::cout << "Balance:        TK." << std::fixed << std::setprecision(2) << balance << std::endl;
}

// Save account data to a binary file (updated to include account type)
void Account::saveToFile(std::ofstream& ofs) const {
    writeString(ofs, accountNumber);
    writeString(ofs, pin);
    ofs.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
    writeString(ofs, ownerName);
    writeString(ofs, accountTypeToString(accountType)); // Save account type as string
}

// Load account data from a binary file (updated to include account type)
void Account::loadFromFile(std::ifstream& ifs) {
    accountNumber = readString(ifs);
    pin = readString(ifs);
    ifs.read(reinterpret_cast<char*>(&balance), sizeof(balance));
    ownerName = readString(ifs);
    accountType = stringToAccountType(readString(ifs)); // Load account type from string
}

// Operator overload for comparison (useful for finding accounts in a vector)
bool Account::operator==(const Account& other) const {
    return accountNumber == other.accountNumber;
}
