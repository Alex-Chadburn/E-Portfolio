#pragma once

class banking {
private:
    double init_Dep;
    double mon_Dep;
    double ann_Int;
    int numYears;

public:
    banking(
        double initialDeposit,
        double monthlyDeposit,
        double annualInterestPercent,
        int years
    );

    void printROI();
};
