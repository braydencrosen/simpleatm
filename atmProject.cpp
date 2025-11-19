#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// Author : Brayden Crosen
// Version: 1.0
// Purpose: Simple atm interface that allows the user to have a specific login pin, and the user may create an account. Once logged into a correct pin, the user will be able to add to, withdrawal from, transfer to, or view accounts.
//// The project is meant to run in an infinite loop, this is to simulate how an ATM resets after each user signs out.

// Do not allow one-time deposits greater than $1,000 for Checking accounts nor $5,000 for Savings accounts.
//  Do not allow one-time withdrawals greater than $500 for Checking accounts nor $1,000 for Savings accounts.

using namespace std;

/* LOGIN INFO (there is an option to add user)

24601 - Hugh Jackman
98649 - Maya Ellison
21036 - Jordan Hale
72791 - Yasmina Dungan
91476 - Lucas Ramirez

*/

// Create new account
void createNewAccount(vector<string> &userID, vector<string> &userName, vector<double> &checkingBalance, vector<double> &savingsBalance)
{
    string newID;
    string newName;
    bool validInput = false;

    // Create a valid 5-digit unique ID
    while (!validInput)
    {
        bool sizeIsFive = false;
        int testFailure = 0;

        cout << "Please create a 5 digit userID: ";
        getline(cin, newID);

        // Check numeric
        bool isNumerical = true;
        for (char c : newID)
        {
            if (!isdigit(c))
            {
                isNumerical = false;
                break;
            }
        }
        if (!isNumerical)
        {
            cout << "Your user pin must be numerical.\n";
            continue;
        }

        // Check size
        if (newID.size() == 5)
            sizeIsFive = true;
        else
            cout << "Your ID must be 5 digits.\n";

        // Check if ID already exists
        for (int i = 0; i < userID.size(); i++)
        {
            if (newID == userID[i])
            {
                cout << "Error, please create a new ID\n";
                testFailure++;
            }
        }

        if (testFailure == 0 && sizeIsFive)
            validInput = true;
    }

    // Store new ID
    userID.push_back(newID);

    // Enter new name
    do
    {
        cout << "Please enter your name --> ";
        getline(cin, newName);

        if (newName.empty())
            cout << "Invalid, try again.\n";
    } while (newName.empty());
    

    // Store new name and add empty balances
    userName.push_back(newName);
    checkingBalance.push_back(0.00);
    savingsBalance.push_back(0.00);

    cout << "\nNew user created! You will now be prompted to login.\n\n";
}

// Login screen
void loginScreen(vector<string> &userID, vector<string> &userName, int &userIndex, vector<double> &checkingBalance, vector<double> &savingsBalance)
{
    // Account check
    bool userFound = false;
    bool creatingAccount = true;
    string loginInput;

    // Ask the user if they have an existing account, if not, call the createNewAccount function
    while (!userFound)
    {
        cout << "Enter your userID, if you want to create a new account, please enter 0\n"
             << "-->";

        getline(cin, loginInput);

        // Create new account

        if (loginInput == "0")
        {
            createNewAccount(userID, userName, checkingBalance, savingsBalance);
            continue;
        }

        // Check numeric
        bool isNumerical = true;
        for (char c : loginInput)
        {
            if (!isdigit(c))
            {
                isNumerical = false;
                break;
            }
        }
        if (!isNumerical)
        {
            cout << "Your user pin must be numerical.\n";
            continue;
        }
        // Check length
        if (loginInput.size() != 5)
        {
            cout << "Your user pin should be 5 digits.\n";
            continue;
        }

        // Search for user ID
        for (int i = 0; i < userID.size(); i++)
        {
            if (loginInput == userID[i])
            {
                userFound = true;
                userIndex = i;
                break;
            }
        }

        // User ID valid, but not found
        if (!userFound)
        {
            cout << "User not found. Try again.\n";
        }
    }
}

// User dashboard
void displayDashboard(vector<string> &userName, int &userIndex, vector<double> &checkingBalance, vector<double> &savingsBalance)
{
    cout << "\nWelcome, " << userName[userIndex] << endl;
    cout << "Checking: $" << fixed << setprecision(2) << checkingBalance[userIndex] << endl;
    cout << "Savings: $" << fixed << setprecision(2) << savingsBalance[userIndex] << endl;
}

// Action menu - returns char for switch in main function
char actionMenu()
{
    cout << "\nWould you like to:\n"
         << "1 - Check Balance\n"
         << "2 - Deposit funds\n"
         << "3 - Withdrawal funds\n"
         << "4 - Transfer funds\n"
         << "5 - Exit\n"
         << "6 - Close Accounts\n";

    string userInput;
    char choice = '\0';

    // Check for valid input
    while (true)
    {
        cout << "\n-->";
        getline(cin, userInput);

        if (userInput.size() == 1 && userInput[0] >= '1' && userInput[0] <= '6')
        {
            choice = userInput[0];
            break;
        }
        else
        {
            cout << "Invalid input. Please enter a number from 1 to 6.";
        }
    }

    return choice;
}

// Deposit funds
void deposit(vector<double> &checkingBalance, vector<double> &savingsBalance, int &userIndex)
{
    // Account deposit limits
    double checkingDepositLimit = 1000.00;
    double savingsDepositLimit = 5000.00;
    cout << "Would you like to deposit to:\n"
         << "1 - Checking\n"
         << "2 - Savings\n";

    string input;
    char choice = '\0';

    // Get valid account choice
    while (true)
    {
        cout << "-->";
        getline(cin, input);

        if (input.size() == 1 && (input[0] == '1' || input[0] == '2'))
        {
            choice = input[0];
            break;
        }
        else
        {
            cout << "Invalid input. Please enter 1 or 2\n";
        }
    }
    // Get deposit amount
    double amount;
    while (true)
    {
        cout << "How much would you like to deposit?\n-->";
        cin >> amount;
        // Fix overflow into getline function
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Input validity tests
        // Negative / zero input
        if (amount <= 0)
        {
            cout << "Invalid amount. Must be greater than 0.\n";
            continue;
        }
        // Prevent more than $500 deposit for checking
        if (amount > checkingDepositLimit && choice == '1')
        {
            cout << "Error - the deposit limit for your SF Bank checking account is $" << checkingDepositLimit << "\ncontact your bank for help, or deposit the desired amount in increments not exceeding $" << checkingDepositLimit << endl;
            return;
        }
        // Prevent more than $1000 deposit for savings
        if (amount > savingsDepositLimit && choice == '2')
        {
            cout << "Error - the deposit limit for your SF Bank savings account is $" << savingsDepositLimit << "\ncontact your bank for help, or deposit the desired amount in increments not exceeding $" << savingsDepositLimit << endl;
            return;
        }
        // Valid amount
        if (amount > 0 && ((choice == '1' && amount <= 1000) || (choice == '2' && amount <= 5000)))
        {
            break;
        }
    }

    // Apply deposit based on account choice
    switch (choice)
    {
        // Checking
    case '1':
        checkingBalance[userIndex] += amount;
        cout << "Deposited $" << fixed << setprecision(2) << amount << " to checking.\n";
        break;
        // Savings
    case '2':
        savingsBalance[userIndex] += amount;
        cout << "Deposited $" << fixed << setprecision(2) << amount << " to savings.\n";
        break;
    }
}

// Withdrawal funds
void withdrawal(vector<double> &checkingBalance, vector<double> &savingsBalance, int &userIndex)
{
    // Account withdrawal limits
    double checkingWithdrawalLimit = 500.00;
    double savingsWithdrawalLimit = 1000.00;
    cout << "Would you like to withdrawal from:\n"
         << "1 - Checking\n"
         << "2 - Savings\n";

    string input;
    char choice = '\0';

    // Get valid account choice
    while (true)
    {
        cout << "-->";
        getline(cin, input);

        if (input.size() == 1 && (input[0] == '1' || input[0] == '2'))
        {
            choice = input[0];
            break;
        }
        else
        {
            cout << "Invalid input. Please enter 1 or 2\n";
        }
    }
    // Prevent withdrawal if balance is $0
    if (checkingBalance[userIndex] == 0.00 && choice == '1')
    {
        cout << "Error - Your checking balance is $" << checkingBalance[userIndex] << ". Your account balance may not be negative." << endl;
        return;
    }
    if (savingsBalance[userIndex] == 0.00 && choice == '2')
    {
        cout << "Error - Your savings balance is $" << savingsBalance[userIndex] << ". Your account balance may not be negative." << endl;
        return;
    }

    // Get withdrawal amount
    double amount;

    while (true)
    {
        cout << "How much would you like to withdrawal?\n-->";
        cin >> amount;
        // Fix overflow from cin to getline function
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Input validity tests
        // Negative / zero input
        if (amount <= 0)
        {
            cout << "Invalid amount. Must be greater than 0.\n";
            continue;
        }
        // Prevent more than $500 withdraw for checking
        if (amount > checkingWithdrawalLimit && choice == '1')
        {
            cout << "Error - the withdrawal limit for your SF Bank checking account is $" << checkingWithdrawalLimit << "\ncontact your bank for help, or withdrawal the desired amount in increments not exceeding $" << checkingWithdrawalLimit << endl;
            return;
        }
        // Prevent more than $1000 withdrawal for savings
        if (amount > savingsWithdrawalLimit && choice == '2')
        {
            cout << "Error - the withdrawal limit for your SF Bank savings account is $" << savingsWithdrawalLimit << "\ncontact your bank for help, or withdrawal the desired amount in increments not exceeding $" << savingsWithdrawalLimit << endl;
            return;
        }

        // Apply withdrawal and associate it with account
        switch (choice)
        {
            // Checking
        case '1':

            // Prevent overdrafting
            if (amount > checkingBalance[userIndex])
            {
                cout << "Error - your checking account cannot have a negative balance.\n"
                     << "Your checking balance is: $" << checkingBalance[userIndex] << endl;
                return;
            }
            // Valid amount, execute withdrawal
            else
            {
                checkingBalance[userIndex] -= amount;
                cout << "Withdrew $" << fixed << setprecision(2) << amount << " from checking.\n";
                return;
            }
            break;

        case '2':

            // Prevent overdrafting
            if (amount > savingsBalance[userIndex])
            {
                cout << "Error - your savings account cannot have a negative balance.\n"
                     << "Your savings balance is: $ " << savingsBalance[userIndex] << endl;
                return;
            }
            else
            {
                savingsBalance[userIndex] -= amount;
                cout << "Withdrew $" << fixed << setprecision(2) << amount << " from savings.\n";
                return;
            }
            break;
        }
    }
}

// Transfer funds
void transfer(vector<double> &checkingBalance, vector<double> &savingsBalance, int &userIndex)
{
    string userInput;
    double amount;

    while (true)
    {
        cout << "Which account would you like to transfer FROM\n"
             << "1 - Checking\n"
             << "2 - Savings\n"
             << "-->";

             getline(cin, userInput);

        // From checking
        if (userInput == "1")
        {
            cout << "Checking -> Savings\n"
                 << "Please enter your desired transfer amount\n"
                 << "-->";

            cin >> amount;
            // Fix overflow into getline function
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Prevent overdraft
            if (amount > checkingBalance[userIndex])
            {
                cout << "Error - Your desired amount exceeds the balance of $" << checkingBalance[userIndex] << " in your Checking account\n";
                continue;
            }
            // Check for negative and nonzero
            else if (amount <= 0)
            {
                cout << "Error - Your desired amount must be greater than $0.00\n";
                continue;
            }
            // Complete transfer
            else
            {
                checkingBalance[userIndex] -= amount;
                savingsBalance[userIndex] += amount;
                cout << "Transferred $" << amount << " from Checking to Savings\n";
                break;
            }
        }

        // From savings
        else if (userInput == "2")
        {
            cout << "Savings -> Checking\n"
                 << "Please enter your desired transfer amount\n"
                 << "-->";

            cin >> amount;
            // Fix overflow into getline function
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Prevent overdraft
            if (amount > savingsBalance[userIndex])
            {
                cout << "Error - Your desired amount exceeds the balance of $" << savingsBalance[userIndex] << " in your Savings account\n";
                continue;
            }
            // Check for negative and nonzero
            else if (amount <= 0)
            {
                cout << "Error - Your desired amount must be greater than $0.00\n";
                continue;
            }
            // Complete transfer
            else
            {
                savingsBalance[userIndex] -= amount;
                checkingBalance[userIndex] += amount;

                cout << "Transferred $" << amount << " from Savings to Checking\n";
                break;
            }
        }

        // Invalid input
        else
        {
            cout << "Invalid input, please enter 1 or 2\n";
            continue;
        }
    }
}

// Check balance
void checkBalance(vector<double> &checkingBalance, vector<double> &savingsBalance, int &userIndex, vector<string> &userName)
{
    cout << userName[userIndex] << endl;
    // Checking
    cout << "Checking: $" << checkingBalance[userIndex] << endl;
    // Savings
    cout << "Savings: $" << savingsBalance[userIndex] << endl;
}

// Close account - returning 0 will sign out and return to login page - any other return value will repeat the action menu
int closeAccounts(vector<double> &checkingBalance, vector<double> &savingsBalance, int &userIndex, vector<string> &userName, vector<string> &userID)
{
    // Nonzero account balance
    if (checkingBalance[userIndex] > 0.00 || savingsBalance[userIndex] > 0.00)
    {
        cout << "Funds have been detected in one or multiple accounts.\n"
             << "Your accounts cannot be closed until all balances are $0.00\n";
        return 1;
    }
    // Security check header
    cout << "We're sorry to see you go!\n"
         << "For security purposes, please enter your userID" << endl;

    string idInput;
    int attempts = 3;

    // Limit the user to 3 pin attempts, this prevents possible fraud. If 3 attempts are reached, return 0.
    while (attempts > 0)
    {
        getline(cin, idInput);

        // Pin pass
        if (idInput == userID[userIndex])
        {
            // Delete user data
            userID.erase(userID.begin() + userIndex);
            userName.erase(userName.begin() + userIndex);
            checkingBalance.erase(checkingBalance.begin() + userIndex);
            savingsBalance.erase(savingsBalance.begin() + userIndex);

            cout << "Your account has been closed.\n";
            return 0;
        }
        // Pin fail
        else
        {
            attempts--;
            if (attempts > 0)
            {
                cout << "Incorrect pin. " << attempts << " attempts remaining.\n";
            }
            else
            {
                cout << "Maximum attempts reached. Deletion has failed, you are being signed out.\n";
                return 0;
            }
        }
    }
}

int main()
{
    // Establish user data (users can be added later)
    vector<string> userID{
        "24601",  // 0 - Hugh
        "98649",  // 1 - Maya
        "21036",  // 2 - Jordan
        "72791",  // 3 - Yasmina
        "91476"}; // 4 - Lucas
    vector<string> userName{
        "Hugh Jackman",   // 0 - Hugh
        "Maya Ellison",   // 1 - Maya
        "Jordan Hale",    // 2 - Jordan
        "Yasmina Dungan", // 3 - Yasmina
        "Lucas Ramirez"}; // 4 - Lucas

    vector<double> checkingBalance{
        303.42,  // 0 - Hugh
        211.96,  // 1 - Maya
        103.92,  // 2 - Jordan
        5035.17, // 3 - Yasmina
        3.53};   // 4 - Lucas

    vector<double> savingsBalance{
        900.00,   // 0 - Hugh
        1210.95,  // 1 - Maya
        152.67,   // 2 - Jordan
        15000.25, // 3 - Yasmina
        25.00};   // 4 - Lucas

bool accountClosed = false;

    while (true)
    {
        int userIndex = -1;

        // Welcome header
        cout << "Welcome to SF Bank\n"
             << "------------------\n";

        // User login
        loginScreen(userID, userName, userIndex, checkingBalance, savingsBalance);

        // Dashboard
        displayDashboard(userName, userIndex, checkingBalance, savingsBalance);

        // Display actions, return char value 1 - 6 which tells the main function which function to call next
        bool run = true;
        while (run)
        {
            switch (actionMenu())
            {
            // Check balance
            case '1':
                checkBalance(checkingBalance, savingsBalance, userIndex, userName);
                break;
            // Deposit funds
            case '2':
                deposit(checkingBalance, savingsBalance, userIndex);
                break;
            // withdrawal funds
            case '3':
                withdrawal(checkingBalance, savingsBalance, userIndex);
                break;
            // Transfer funds
            case '4':
                transfer(checkingBalance, savingsBalance, userIndex);
                break;
            // Exit
            case '5':
                run = false;
                break;
            // Close account
            case '6':
                if (closeAccounts(checkingBalance, savingsBalance, userIndex, userName, userID) == 0)
                {
                    // Sign out user
                    accountClosed = true;
                    run = false;
                }
                break;
            }
        }
        if (!accountClosed)
        {
        cout << "Goodbye " << userName[userIndex] << ", thanks for using SF Bank!\n\n";
        }
    }
}