# E-Portfolio

## What did I change in Software test?

I added a pom.xml and organized the architecture so that it supports regression testing. This is a very good way to run tests continuously to make sure new changes don't change old functionality. I personally use `mvn clean package`, we can also inject dependencies so that the program runs and we can test new functionalities. By using the command `mvn clean package dependency:copy-dependencies`.

## What did I change in Embedded Systems?

I double-checked the logic using the debug statements, and I noticed that it was pausing at wrong times, this was due to the indentation levels being misplaced. While I was there, I simplified the logic using an enumerate instead of a for loop. I also removed a bunch of auto-generated comments that were not really relevant nor practical to keep up. The code is now readable enough that you can check the control flow without the need for judicious comments. The debug flag was also removed, this might matter depending on how you compile the program, usually with optimization flag set and debug set to false however, this would not matter. The way I tested these changes was on my Raspberry PI and also console the output.

## What did I change in the Banking C++ program?

The program still starts by accepting and validating the user’s financial information, including the interest rate, monthly deposit, and initial deposit. It uses those values to calculate how an investment grows. I extended that flow so users can also save account details to a database and check them again later. When an account is checked, the program recalculates its growth to reflect the time that has passed. This lets users keep track of multiple accounts without losing the original calculator functionality.

To support this, I added database functionality and reorganized the project into separate folders for dependencies, include files, and source files. I used the chrono library and date parsing to work with elapsed time, so the program can determine how an account’s balance has changed since it was saved. To compile this, I used Microsoft's CL command like so;
`cl /EHsc /std:c++20 /I includes /I dependencies  src\main.cpp src\account_input.cpp src\banking.cpp src\database.cpp src\database_menu.cpp dependencies\sqlite3.c /Fe:Banking.exe`
