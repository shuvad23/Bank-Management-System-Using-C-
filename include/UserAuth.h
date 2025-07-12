// include/UserAuth.h
#ifndef USERAUTH_H
#define USERAUTH_H

#include "Account.h"
#include <vector>
#include <string>

class UserAuth {
private:
    // Static member to hold all accounts in memory
    static std::vector<Account> accounts;
    static const std::string ACCOUNTS_FILE; // File to store account data

    // Private helper to generate a unique account number
    static std::string generateAccountNumber();

    // Private constructor to prevent instantiation (it's a utility class)
    UserAuth() = delete;

public:
    // Static methods for user management
    static bool registerUser();
    static Account* loginUser(); // Returns pointer to logged-in account, or nullptr

    // Static methods for data persistence
    static void loadAccounts();
    static void saveAccounts();

    // Static method to find an account by number
    static Account* findAccount(const std::string& accNum);

    // Static method to get a reference to the accounts vector (for external modification, e.g., main)
    static std::vector<Account>& getAccounts();
};

#endif // USERAUTH_H
