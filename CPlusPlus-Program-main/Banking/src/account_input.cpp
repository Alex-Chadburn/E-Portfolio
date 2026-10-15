#include "account_input.h"

#include <iostream>
#include <limits>

AccountInput getAccountInput() {
    AccountInput input{};

    while (true) {
        std::cout << "\nInitial Investment Amount: $";
        std::cin >> input.initialDeposit;

        if (!std::cin.fail() && input.initialDeposit > 0) {
            break;
        }

        std::cout << "Please enter a positive amount.\n";
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }

    while (true) {
        std::cout << "Monthly Deposit: $";
        std::cin >> input.monthlyDeposit;

        if (!std::cin.fail() && input.monthlyDeposit >= 0) {
            break;
        }

        std::cout << "Please enter a non-negative amount.\n";
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }

    while (true) {
        std::cout << "Annual Interest: %";
        std::cin >> input.annualInterest;

        if (!std::cin.fail() && input.annualInterest >= 0) {
            break;
        }

        std::cout << "Please enter a non-negative percentage.\n";
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }

    while (true) {
        std::cout << "Number of years: ";
        std::cin >> input.years;

        if (!std::cin.fail() && input.years > 0) {
            break;
        }

        std::cout << "Please enter a positive number of years.\n";
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }

    return input;
}
