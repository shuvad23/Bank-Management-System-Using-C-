// include/Transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>

// Structure to represent a single transaction
struct Transaction {
    std::string accountNumber;
    std::string type; // e.g., "Deposit", "Withdrawal"
    double amount;
    std::string date; // YYYY-MM-DD HH:MM:SS

    // Default constructor
    Transaction() : accountNumber(""), type(""), amount(0.0), date("") {}

    // Parameterized constructor
    Transaction(const std::string& accNum, const std::string& t, double amt, const std::string& d)
        : accountNumber(accNum), type(t), amount(amt), date(d) {}
};

// Function to log a transaction to the logs file
void logTransaction(const Transaction& trans);

// Function to view all transactions for a specific account
void viewAccountStatement(const std::string& accountNumber);

#endif // TRANSACTION_H
