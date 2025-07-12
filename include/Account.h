// include/Account.h
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <fstream> // For std::ofstream, std::ifstream
#include <iostream> // For std::cout
#include <vector> // For potential future use with transactions within account

// Enum to define different types of accounts
enum class AccountType {
    SAVINGS,
    CURRENT,
    FIXED_DEPOSIT,
    RECURRING_DEPOSIT,
    LOAN,
    JOINT,
    BUSINESS,
    STUDENT,
    SALARY,
    UNKNOWN // Default or error state
};

// Helper function to convert AccountType enum to string
std::string accountTypeToString(AccountType type);

// Helper function to convert string to AccountType enum
AccountType stringToAccountType(const std::string& typeStr);


class Account {
private:
    std::string accountNumber;
    std::string pin; // Stored as plain text for simplicity; in real app, hash this.
    double balance;
    std::string ownerName;
    AccountType accountType; // New member for account type

public:
    // Default constructor
    Account();

    // Parameterized constructor (updated to include accountType)
    Account(const std::string& accNum, const std::string& p, double bal, const std::string& name, AccountType type);

    // Getters
    std::string getAccountNumber() const;
    std::string getPin() const;
    double getBalance() const;
    std::string getOwnerName() const;
    AccountType getAccountType() const; // New getter for account type

    // Setters (if needed, though direct modification is often avoided)
    void setBalance(double newBalance);

    // Core account operations
    bool deposit(double amount);
    bool withdraw(double amount);

    // Authentication
    bool authenticate(const std::string& enteredPin) const;

    // Display account information
    void displayAccountInfo() const;

    // File I/O operations for saving/loading account data
    // These methods handle the serialization/deserialization of an Account object
    void saveToFile(std::ofstream& ofs) const;
    void loadFromFile(std::ifstream& ifs);

    // Operator overload for comparison (useful for finding accounts)
    bool operator==(const Account& other) const;
};

#endif // ACCOUNT_H
