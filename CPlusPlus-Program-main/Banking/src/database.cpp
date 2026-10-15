#include "Database.h"
#include <chrono>
#include <iostream>

Database::Database(const std::string& databasePath)
    : db_(nullptr) {
    int result = sqlite3_open(databasePath.c_str(), &db_);

    if (result != SQLITE_OK) {
        std::cerr << "Could not open database: "
                  << sqlite3_errmsg(db_) << '\n';

        sqlite3_close(db_);
        db_ = nullptr;
        return;
    }

    // Enable foreign-key enforcement for this connection.
    char* errorMessage = nullptr;

    result = sqlite3_exec(
        db_,
        "PRAGMA foreign_keys = ON;",
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {
        std::cerr << "Could not enable foreign keys: "
                  << errorMessage << '\n';

        sqlite3_free(errorMessage);
    }
}

Database::~Database() {
    if (db_ != nullptr) {
        sqlite3_close(db_);
    }
}

bool Database::isOpen() const {
    return db_ != nullptr;
}

bool Database::initialize() {
    if (!isOpen()) {
        return false;
    }

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS accounts (
            account_number INTEGER PRIMARY KEY AUTOINCREMENT,
            person_name TEXT NOT NULL,
            initial_deposit_cents INTEGER NOT NULL DEFAULT 0,
            monthly_deposit_cents INTEGER NOT NULL DEFAULT 0,
            start_date TEXT NOT NULL,
            interest_rate REAL NOT NULL
        );

        CREATE TABLE IF NOT EXISTS deposits (
            deposit_id INTEGER PRIMARY KEY AUTOINCREMENT,
            account_number INTEGER NOT NULL,
            amount_cents INTEGER NOT NULL,
            deposit_date TEXT NOT NULL,

            FOREIGN KEY (account_number)
                REFERENCES accounts(account_number)
                ON DELETE CASCADE
        );
    )";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        db_,
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {
        std::cerr << "Could not initialize database: "
            << errorMessage << '\n';
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

bool Database::deleteAccount(std::int64_t accountNumber) {
    if (!isOpen()) {
        return false;
    }

    const char* sql = R"(
        DELETE FROM accounts
        WHERE account_number = ?;
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        db_,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {
        std::cerr << "Could not prepare delete statement: "
            << sqlite3_errmsg(db_) << '\n';
        return false;
    }

    sqlite3_bind_int64(statement, 1, accountNumber);

    result = sqlite3_step(statement);

    sqlite3_finalize(statement);

    // Only return true if a row was actually deleted
    return (result == SQLITE_DONE && sqlite3_changes(db_) > 0);
}

std::int64_t Database::createAccount(
    const std::string& personName,
    std::int64_t initialDepositCents,
    std::int64_t monthlyDepositCents,
    int yearsAgo,
    double interestRate
) {
    if (!isOpen()) {
        return -1;
    }

    // Compute start date using chrono (today - yearsAgo)
    auto today = std::chrono::floor<std::chrono::days>(
        std::chrono::system_clock::now()
    );

    std::chrono::year_month_day ymdToday{ today };
    std::chrono::year_month_day start =
        ymdToday - std::chrono::years{ yearsAgo };

    // Convert chrono date to "YYYY-MM-DD" using std::format
    std::string startDateString = std::format("{:%Y-%m-%d}", start);

    const char* sql = R"(
        INSERT INTO accounts (
            person_name,
            initial_deposit_cents,
            monthly_deposit_cents,
            start_date,
            interest_rate
        )
        VALUES (?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        db_,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {
        std::cerr << "Could not prepare account insert: "
            << sqlite3_errmsg(db_) << '\n';
        return -1;
    }

    sqlite3_bind_text(statement, 1, personName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(statement, 2, initialDepositCents);
    sqlite3_bind_int64(statement, 3, monthlyDepositCents);
    sqlite3_bind_text(statement, 4, startDateString.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(statement, 5, interestRate);

    result = sqlite3_step(statement);

    if (result != SQLITE_DONE) {
        std::cerr << "Could not create account: "
            << sqlite3_errmsg(db_) << '\n';
        sqlite3_finalize(statement);
        return -1;
    }

    sqlite3_finalize(statement);

    return sqlite3_last_insert_rowid(db_);
}



bool Database::addDeposit(
    std::int64_t accountNumber,
    std::int64_t amountCents,
    const std::string& depositDate
) {
    if (!isOpen()) {
        return false;
    }

    const char* sql = R"(
        INSERT INTO deposits (
            account_number,
            amount_cents,
            deposit_date
        )
        VALUES (?, ?, ?);
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        db_,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {
        std::cerr << "Could not prepare deposit insert: "
                  << sqlite3_errmsg(db_) << '\n';

        return false;
    }

    sqlite3_bind_int64(
        statement,
        1,
        accountNumber
    );

    sqlite3_bind_int64(
        statement,
        2,
        amountCents
    );

    sqlite3_bind_text(
        statement,
        3,
        depositDate.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    result = sqlite3_step(statement);

    if (result != SQLITE_DONE) {
        std::cerr << "Could not add deposit: "
                  << sqlite3_errmsg(db_) << '\n';

        sqlite3_finalize(statement);
        return false;
    }

    sqlite3_finalize(statement);

    return true;
}

std::optional<Account> Database::getAccount(std::int64_t accountNumber) {
    if (!isOpen()) {
        return std::nullopt;
    }

    const char* sql = R"(
        SELECT
            account_number,
            person_name,
            initial_deposit_cents,
            monthly_deposit_cents,
            start_date,
            interest_rate
        FROM accounts
        WHERE account_number = ?;
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        db_,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {
        std::cerr << "Could not prepare account query: "
            << sqlite3_errmsg(db_) << '\n';
        return std::nullopt;
    }

    sqlite3_bind_int64(statement, 1, accountNumber);

    result = sqlite3_step(statement);

    if (result == SQLITE_ROW) {
        Account account;

        account.accountNumber =
            sqlite3_column_int64(statement, 0);

        account.personName =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
                );

        account.initialDepositCents =
            sqlite3_column_int64(statement, 2);

        account.monthlyDepositCents =
            sqlite3_column_int64(statement, 3);

        // Convert "YYYY-MM-DD" string to chrono::year_month_day
        std::string dateStr =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 4)
                );

        int y, m, d;
        std::sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d);

        account.startDate =
            std::chrono::year{ y } /
            std::chrono::month{ static_cast<unsigned>(m) } /
            std::chrono::day{ static_cast<unsigned>(d) };


        account.interestRate =
            sqlite3_column_double(statement, 5);

        sqlite3_finalize(statement);
        return account;
    }

    sqlite3_finalize(statement);
    return std::nullopt;
}


std::vector<Deposit> Database::getDeposits(
    std::int64_t accountNumber
) {
    std::vector<Deposit> deposits;

    if (!isOpen()) {
        return deposits;
    }

    const char* sql = R"(
        SELECT
            deposit_id,
            account_number,
            amount_cents,
            deposit_date
        FROM deposits
        WHERE account_number = ?
        ORDER BY deposit_date;
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        db_,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {
        std::cerr << "Could not prepare deposit query: "
                  << sqlite3_errmsg(db_) << '\n';

        return deposits;
    }

    sqlite3_bind_int64(
        statement,
        1,
        accountNumber
    );

    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {
        Deposit deposit;

        deposit.depositId =
            sqlite3_column_int64(statement, 0);

        deposit.accountNumber =
            sqlite3_column_int64(statement, 1);

        deposit.amountCents =
            sqlite3_column_int64(statement, 2);

        deposit.depositDate =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 3)
            );

        deposits.push_back(deposit);
    }

    if (result != SQLITE_DONE) {
        std::cerr << "Could not retrieve deposits: "
                  << sqlite3_errmsg(db_) << '\n';
    }

    sqlite3_finalize(statement);

    return deposits;
}
void Database::printAccounts() {
    const char* sql = R"(
        SELECT
            account_number,
            person_name,
            initial_deposit_cents,
            monthly_deposit_cents,
            start_date,
            interest_rate
        FROM accounts
        ORDER BY account_number;
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        db_,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK) {
        std::cerr << "Could not retrieve accounts.\n";
        return;
    }

    while (sqlite3_step(statement) == SQLITE_ROW) {
        Account account;

        account.accountNumber =
            sqlite3_column_int64(statement, 0);

        account.personName =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 1)
                );

        account.initialDepositCents =
            sqlite3_column_int64(statement, 2);

        account.monthlyDepositCents =
            sqlite3_column_int64(statement, 3);

        // Parse start_date string into chrono::year_month_day
        std::string dateStr =
            reinterpret_cast<const char*>(
                sqlite3_column_text(statement, 4)
                );

        int y, m, d;
        std::sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d);

        account.startDate =
            std::chrono::year{ y } /
            std::chrono::month{ static_cast<unsigned>(m) } /
            std::chrono::day{ static_cast<unsigned>(d) };

        account.interestRate =
            sqlite3_column_double(statement, 5);

        // Compute months passed using chrono
        auto today = std::chrono::floor<std::chrono::days>(
            std::chrono::system_clock::now()
        );

        std::chrono::year_month_day ymdToday{ today };

        int monthsPassed =
            (int(ymdToday.year()) - int(account.startDate.year())) * 12 +
            (unsigned(ymdToday.month()) - unsigned(account.startDate.month()));

        if (unsigned(ymdToday.day()) < unsigned(account.startDate.day())) {
            monthsPassed--;
        }

        // Compute balance using the same logic as banking::printROI()
        double rm = account.interestRate / 12.0;

        double initial = account.initialDepositCents / 100.0;
        double monthly = account.monthlyDepositCents / 100.0;

        double balance = initial;

        // Apply monthly deposits + monthly compounding
        for (int i = 0; i < monthsPassed; ++i) {
            balance += monthly;        // deposit first
            balance += balance * rm;   // then compound
        }

        std::cout << "\nAccount Number: " << account.accountNumber << '\n';
        std::cout << "Name: " << account.personName << '\n';
        std::cout << "Initial Deposit: $" << initial << '\n';
        std::cout << "Monthly Deposit: $" << monthly << '\n';
        std::cout << "Interest Rate: " << account.interestRate * 100 << "%\n";
        std::cout << "Start Date: " << dateStr << '\n';
        std::cout << "Current Balance: $" << std::fixed
            << std::setprecision(2)
            << balance << "\n";
    }

    sqlite3_finalize(statement);
}

