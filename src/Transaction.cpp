// src/Transaction.cpp
#include "Transaction.h"
#include <fstream>  // For std::ofstream, std::ifstream
#include <iostream> // For std::cout, std::endl
#include <iomanip>  // For std::fixed, std::setprecision
#include <sstream>  // For std::getline

// Path to the transaction logs file
const std::string LOGS_FILE = "data/logs.txt";

// Function to log a transaction to the logs file
void logTransaction(const Transaction& trans) {
    // Open the logs file in append mode
    std::ofstream ofs(LOGS_FILE, std::ios::app);
    if (!ofs.is_open()) {
        std::cerr << "Error: Could not open logs file for writing." << std::endl;
        return;
    }

    // Write transaction details in a comma-separated format
    ofs << trans.accountNumber << ","
        << trans.type << ","
        << std::fixed << std::setprecision(2) << trans.amount << ","
        << trans.date << std::endl;

    ofs.close();
}

// Function to view all transactions for a specific account
void viewAccountStatement(const std::string& accountNumber) {
    std::ifstream ifs(LOGS_FILE);
    if (!ifs.is_open()) {
        std::cout << "No transaction history found for this account yet." << std::endl;
        return;
    }

    std::cout << "\n--- Transaction Statement for Account: " << accountNumber << " ---" << std::endl;
    std::cout << std::setw(20) << std::left << "Date"
              << std::setw(15) << std::left << "Type"
              << std::setw(15) << std::left << "Amount" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    std::string line;
    bool foundTransactions = false;
    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        std::string accNum, type, amountStr, date;

        // Parse the comma-separated values
        if (std::getline(ss, accNum, ',') &&
            std::getline(ss, type, ',') &&
            std::getline(ss, amountStr, ',') &&
            std::getline(ss, date)) {

            if (accNum == accountNumber) {
                foundTransactions = true;
                std::cout << std::setw(20) << std::left << date
                          << std::setw(15) << std::left << type
                          << std::setw(15) << std::left << "TK: " + amountStr << std::endl;
            }
        }
    }

    if (!foundTransactions) {
        std::cout << "No transactions found for this account." << std::endl;
    }
    std::cout << std::string(50, '-') << std::endl;
    ifs.close();
}
