#include <iostream> //for input/output
#include <iomanip> // setprecision, fixed
#include "banking.h" // header for definitions
using namespace std;

// If you don't see a comment, I probably already explained it.

banking::banking(
    double initialDeposit,
    double monthlyDeposit,
    double annualInterestPercent,
    int years
) {
    init_Dep = initialDeposit;
    mon_Dep = monthlyDeposit;
    ann_Int = annualInterestPercent / 100.0;
    numYears = years;
}

void banking::printROI() {
    int digits = 0;
    double compounded = init_Dep; //Initialize compounded interest with the initial deposit
    double pyeb = init_Dep; // Initialize previous year end balance to initial deposit
    cout << "    Balance and Interest Without Additional Monthly Deposits    " << endl;
    cout << "================================================================" << endl;
    cout << "  Year        Year End Balance     Year End Balance Interest    " << endl;
    cout << "----------------------------------------------------------------" << endl;
    for (int i = 1; i <= numYears; ++i) { // outer loop prints all the earned interest at the end of the year.
        for (int month = 1; month <= 12; ++month) { // 12 months in a year, interest compounds monthly
            compounded += compounded * (ann_Int / 12);
        } // compounded interest for the year
        double compounded2 = compounded; // copy of compounded interest used to find digits
        while (static_cast<int>(compounded2) > 0) { //casts the double to an integer(explicitly), checks that it is greater than 0.
            compounded2 = compounded2 / 10; // divides by 10, reducing the digits in the number by 1
            digits += 1; // adds a digit to the counter
        }
        double interest = compounded - pyeb; // Calculate interest earned for the year
        cout << setw(5) << i << setw(20-digits) << "$" << setprecision(2) << fixed << compounded; // uses digits for formatting
        cout << setw(30) << "$" << setprecision(2) << fixed << interest << endl;
        pyeb = compounded; // Update previous year end balance
        digits = 0; // resets digits at the end of each year to prevent formatting issues.
    }
    cout << "================================================================\n" << endl;

    // Reset compounded interest and previous year end balance
    compounded = init_Dep;
    pyeb = init_Dep;
    cout << "       Balance and Interest With Additional Monthly Deposits    " << endl;
    cout << "================================================================" << endl;
    cout << "  Year        Year End Balance     Year End Balance Interest    " << endl;
    cout << "----------------------------------------------------------------" << endl;
    for (int i = 1; i <= numYears; ++i) {
        for(int month = 1; month <= 12; ++month) {
            compounded += mon_Dep; // adds monthly deposit
            compounded += compounded * (ann_Int / 12);
        } // compounded interest for the year including the monthly deposits
        double interest;
        //Same formula, however, we remove the amount of money we deposited as well.
        interest = compounded - pyeb - (mon_Dep * 12); 
        double compounded2 = compounded;
        while (static_cast<int>(compounded2) > 0) {
            compounded2 = compounded2 / 10;
            digits += 1;
        }
        cout << setw(5) << i << setw(20-digits) << "$" << setprecision(2) << fixed << compounded;
        cout << setw(30) << "$" << setprecision(2) << fixed << interest << endl;
        pyeb = compounded; // Update previous year end balance
        digits = 0;
    }
    cout << "================================================================\n\n\n\n\n\n" << endl;
}
