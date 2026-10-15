#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <limits>

#include "banking.h"
#include "Database.h"
#include "database_menu.h"
#include "account_input.h"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseProgram() {
    std::cout << "\nPress Enter to continue...";

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::cin.get();
}

int main() {
    Database database("banking.db");

    if (!database.isOpen()) {
        std::cerr << "Could not open the database.\n";
        return 1;
    }

    if (!database.initialize()) {
        std::cerr << "Could not initialize the database.\n";
        return 1;
    }

    while (true) {
        clearScreen();

        std::cout << "==============================\n";
        std::cout << "       Banking Calculator\n";
        std::cout << "==============================\n";
        std::cout << "1. Create new calculation\n";
        std::cout << "2. View previous accounts\n";
        std::cout << "3. Delete existing accounts\n";
        std::cout << "4. Exit\n";
        std::cout << "\nChoose an option: ";

        char choice;
        std::cin >> choice;

        if (choice == '1') {
            clearScreen();

            AccountInput input = getAccountInput();

            banking bank(
                input.initialDeposit,
                input.monthlyDeposit,
                input.annualInterest,
                input.years
            );

            clearScreen();

            bank.printROI();

            runDatabaseMenu(
                database,
                input.initialDeposit,
                input.monthlyDeposit,
                input.years,
                (input.annualInterest / 100)
            );

        }
        else if (choice == '2') {
            clearScreen();

            database.printAccounts();

            pauseProgram();
        }
        else if (choice == '3') {
            clearScreen();

            std::cout << "Enter account number to delete: ";
            std::int64_t accNum;
            std::cin >> accNum;

            if (database.deleteAccount(accNum)) {
                std::cout << "\nAccount deleted successfully.\n";
            }
            else {
                std::cout << "\nCould not delete account (not found).\n";
            }

            pauseProgram();
        }
        else if (choice == '4')
        {
            break;
        }

        else {
            std::cout << "\nInvalid option.\n";
        }
    }

    std::cout << "\nGoodbye.\n";
    return 0;
}
