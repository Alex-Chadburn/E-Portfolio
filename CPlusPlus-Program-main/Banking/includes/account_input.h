#pragma once

struct AccountInput {
    double initialDeposit;
    double monthlyDeposit;
    double annualInterest;
    int years;
};

AccountInput getAccountInput();
