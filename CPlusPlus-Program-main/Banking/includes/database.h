#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <chrono>

#include "sqlite3.h"

struct Account {
    std::int64_t accountNumber;
    std::string personName;
    std::int64_t initialDepositCents;
    std::int64_t monthlyDepositCents;
    std::chrono::year_month_day startDate;
    double interestRate;
};

struct Deposit {
    std::int64_t depositId;
    std::int64_t accountNumber;
    std::int64_t amountCents;
    std::string depositDate;
};

class Database {
public:
    explicit Database(const std::string& databasePath);
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    bool isOpen() const;
    bool initialize();

    std::int64_t createAccount(
        const std::string& personName,
        std::int64_t initialDepositCents,
        std::int64_t monthlyDepositCents,
        int yearsAgo,
        double interestRate
    );

    bool addDeposit(
        std::int64_t accountNumber,
        std::int64_t amountCents,
        const std::string& depositDate
    );

    std::optional<Account> getAccount(std::int64_t accountNumber);
    std::vector<Deposit> getDeposits(std::int64_t accountNumber);
    bool deleteAccount(std::int64_t accountNumber);

    void printAccounts();

private:
    sqlite3* db_;
};
