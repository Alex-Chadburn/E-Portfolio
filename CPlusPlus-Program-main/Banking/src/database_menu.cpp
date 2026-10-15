#include "database_menu.h"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <chrono>
#include <format>

void runDatabaseMenu(
    Database& database,
    double initialDeposit,
    double monthlyDeposit,
    int years,
    double interestRate
) {
    char choice;

    std::cout << "\nSave this account to the database? (y/n): ";
    std::cin >> choice;

    if (choice != 'y' && choice != 'Y') {
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::string personName;

    std::cout << "Enter account holder name: ";
    std::getline(std::cin, personName);

    std::int64_t initialDepositCents =
        static_cast<std::int64_t>(
            std::llround(initialDeposit * 100.0)
            );

    std::int64_t monthlyDepositCents =
        static_cast<std::int64_t>(
            std::llround(monthlyDeposit * 100.0)
            );

    std::int64_t accountNumber = database.createAccount(
        personName,
        initialDepositCents,
        monthlyDepositCents,
        years,
        interestRate
    );

    if (accountNumber == -1) {
        std::cout << "\nFailed to create account.\n";
        return;
    }

    // Add initial deposit using today's date
    if (initialDepositCents > 0) {
        auto today = std::chrono::floor<std::chrono::days>(
            std::chrono::system_clock::now()
        );

        std::string todayStr = std::format(
            "{:%Y-%m-%d}",
            std::chrono::year_month_day{ today }
        );

        database.addDeposit(
            accountNumber,
            initialDepositCents,
            todayStr
        );
    }

    std::cout << "\nAccount created successfully.\n";
    std::cout << "Account number: " << accountNumber << '\n';
}
