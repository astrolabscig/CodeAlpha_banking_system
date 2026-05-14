#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <conio.h>
#include <windows.h>
using namespace std;

// Account form template
struct Account {
    int accountNumber;
    string name;
    string pin;
    double balance;
};


// replace spaces with underscores for saving
string encodeSpaces(string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] == ' ') text[i] = '_';
    }
    return text;
}

// Replace underscores with spaces for displaying
string decodeSpaces(string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        if (text[i] == '_') text[i] = ' ';
    }
    return text;
}

// Get hidden PIN input
string getHiddenPin() {
    string pin = "";
    char ch;

    while (true) {
        ch = _getch();

        if (ch == '\r') {
            cout << endl;
            break;
        }
        else if (ch == '\b') {
            if (!pin.empty()) {
                pin.pop_back();
                cout << "\b \b";
            }
        }
        else {
            pin += ch;
            cout << "*";
        }
    }
    return pin;
}

// Check if string is only digits
bool isAllDigits(string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        if (!isdigit(text[i])) return false;
    }
    return true;
}

// Check if account number already exists
bool accountExists(int accountNumber) {
    ifstream inFile("accounts.txt");
    if (!inFile.is_open()) return false;

    Account acc;
    while (inFile   >> acc.accountNumber
                    >> acc.name
                    >> acc.pin
                    >> acc.balance ) {
        if (acc.accountNumber == accountNumber) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

// Save a brand new file
void saveAccount(Account acc) {
    ofstream outFile("accounts.txt", ios::app);
    outFile << acc.accountNumber << " "
            << encodeSpaces(acc.name) << " "
            << acc.pin << " "
            << fixed << setprecision(2)
            << acc.balance << endl;
    outFile.close();
}

// Update balance of existing account
void updateBalance(int accountNumber, double newBalance) {
    Account accounts[100];
    int count = 0;

    // Read all accounts
    ifstream inFile("accounts.txt");
    while (inFile >> accounts[count].accountNumber
                  >> accounts[count].name
                  >> accounts[count].pin
                  >> accounts[count].balance) {
        count++;
    }
    inFile.close();

    //  Find and update the account
    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            accounts[i].balance = newBalance;
        }
    }

    // Write all back fresh
    ofstream outFile("accounts.txt");
    for (int i = 0; i < count; i++) {
        outFile << accounts[i].accountNumber << " "
                << accounts[i].name          << " "
                << accounts[i].pin           << " "
                << fixed << setprecision(2)
                << accounts[i].balance       << endl;
    }
    outFile.close();
}

// Log transaction
void logTransaction(int accountNumber, string type, double amount) {
    ofstream logFile("transactions.txt", ios::app);
    logFile << accountNumber << " "
            << type << " "
            << fixed << setprecision(2)
            << amount << endl;
    logFile.close();
}


// Show transaction history
void showHistory(int accountNumber) {
    ifstream logFile("transactions.txt");
    if (!logFile.is_open()) {
        cout << " No transactions found." << endl;
        return;
    }

    int savedNumber;
    string type;
    double amount;
    int count = 1;
    bool found = false;

    cout << "\n TRANSACTION HISTORY " << endl;

    while (logFile >> savedNumber >> type >> amount) {
        if (savedNumber == accountNumber) {
            found = true;
            if (type == "Deposit") {
                cout << count << ". Deposit    : + GHS "
                     << fixed << setprecision(2) << amount << endl;
            } else {
                cout << count << ". Withdrawal :- GHS "
                     << fixed << setprecision(2) << amount << endl;
            }
            count++;
        }
    }

    logFile.close();

    if (!found) {
        cout << ">> No transactions yet for this account." << endl;
    }
}

// Find and return an account by number and  PIN
Account findAccount(int accountNumber, string pin, bool &found) {
    Account acc;
    found = false;
    
    ifstream inFile("accounts.txt");
    if (!inFile.is_open()) return acc;

    while (inFile >> acc.accountNumber
                  >> acc.name
                  >> acc.pin
                  >> acc.balance) {
        if (acc.accountNumber == accountNumber && acc.pin == pin) {
            found = true;
            inFile.close();
            return acc;
        }
    }

    inFile.close();
    return acc;
}


// Show main menu
void showMainMenu() {
    cout << "\n==========================================" << endl;
    cout << "        CodeAlpha Internship Task 3        " << endl;
    cout << "           BANKING SYSTEM                 " << endl;
    cout << "==========================================" << endl;
    cout << "  1. Create Account                       " << endl;
    cout << "  2. Login                                " << endl;
    cout << "  3. Exit                                 " << endl;
    cout << "==========================================" << endl;
    cout << "Choose an option: ";
}

// Show account menu after login
void showAccountMenu(string name) {
    cout << "\n==========================================" << endl;
    cout << "  Welcome, " << decodeSpaces(name) << "!" << endl;
    cout << "==========================================" << endl;
    cout << "  1. Deposit                              " << endl;
    cout << "  2. Withdraw                             " << endl;
    cout << "  3. Check Balance                        " << endl;
    cout << "  4. Transaction History                  " << endl;
    cout << "  5. Logout                               " << endl;
    cout << "==========================================" << endl;
    cout << "Choose an option: ";
}

// Create new account
void createAccount() {
    Account acc;
    string nameInput;

    cout << "\n CREATE ACCOUNT" << endl;

    // Get full name
    cout << "Enter your full name: ";
    cin.ignore();
    getline(cin, nameInput); // getline reads the whole line including spaces
    acc.name = nameInput;

    // Get and validate account number
    string accNumInput;
    do {
        cout << "Choose an account number (4 digits): ";
        cin >> accNumInput;

        if (!isAllDigits(accNumInput) || accNumInput.length() != 4) {
            cout << "Invalid! Account number must be exactly 4 digits." << endl;
        }
        else if (accountExists(stoi(accNumInput))) {
            cout << "Sorry! That account number is already taken." << endl;
            accNumInput = "";
        }
    } while (!isAllDigits(accNumInput) ||
              accNumInput.length() != 4 ||
              accountExists(stoi(accNumInput)));

     acc.accountNumber = stoi(accNumInput);   // convert string to int

         // ---- Get and validate PIN ----
    string pinInput;
    do {
        cout << "Choose a PIN (exactly 4 digits): ";
        pinInput = getHiddenPin();

        if (!isAllDigits(pinInput) || pinInput.length() != 4) {
            cout << ">> Invalid! PIN must be exactly 4 digits." << endl;
        }

    } while (!isAllDigits(pinInput) || pinInput.length() != 4);

    acc.pin     = pinInput;
    acc.balance = 0.00;   

    //  Save to file 
    saveAccount(acc);

    cout << ">> Account created successfully!" << endl;
    cout << ">> Account Number : " << acc.accountNumber << endl;
    cout << ">> Name           : " << acc.name << endl;
    cout << ">> Starting Balance: GHS 0.00" << endl;

}


// ---- Login ----
void loginAccount() {

    cout << "\n--- LOGIN ---" << endl;

    string accNumInput, pinInput;

    cout << "Enter account number: ";
    cin >> accNumInput;

    cout << "Enter PIN: ";
    pinInput = getHiddenPin();

    // Validate format first
    if (!isAllDigits(accNumInput) || !isAllDigits(pinInput)) {
        cout << ">> Invalid credentials. Please try again." << endl;
        return;
    }

    int accountNumber = stoi(accNumInput);

    // Find account
    bool found;
    Account acc = findAccount(accountNumber, pinInput, found);

    if (!found) {
        cout << ">> Incorrect account number or PIN." << endl;
        return;
    }

    // ---- Account menu loop ----
    int choice;

    do {
        showAccountMenu(acc.name);
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << ">> Invalid! Please enter 1 to 5." << endl;
        }

        // ---- DEPOSIT ----
        else if (choice == 1) {
            double amount;

            cout << "Enter deposit amount: GHS ";
            cin >> amount;

            if (cin.fail() || amount <= 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << ">> Invalid amount. Must be greater than 0." << endl;
            }
            else {
                acc.balance += amount;
                updateBalance(acc.accountNumber, acc.balance);
                logTransaction(acc.accountNumber, "Deposit", amount);
                cout << ">> GHS " << fixed << setprecision(2)
                     << amount << " deposited successfully!" << endl;
                cout << ">> New balance: GHS " << fixed << setprecision(2)
                     << acc.balance << endl;
            }
        }

        // ---- WITHDRAW ----
        else if (choice == 2) {
            double amount;

            cout << "Enter withdrawal amount: GHS ";
            cin >> amount;

            if (cin.fail() || amount <= 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << ">> Invalid amount. Must be greater than 0." << endl;
            }
            else if (amount > acc.balance) {
                // Insufficient funds check!
                cout << ">> Insufficient funds!" << endl;
                cout << ">> Current balance: GHS " << fixed << setprecision(2)
                     << acc.balance << endl;
            }
            else {
                acc.balance -= amount;
                updateBalance(acc.accountNumber, acc.balance);
                logTransaction(acc.accountNumber, "Withdrawal", amount);
                cout << ">> GHS " << fixed << setprecision(2)
                     << amount << " withdrawn successfully!" << endl;
                cout << ">> New balance: GHS " << fixed << setprecision(2)
                     << acc.balance << endl;
            }
        }

        // ---- CHECK BALANCE ----
        else if (choice == 3) {
            cout << "\n>> Current balance: GHS " << fixed << setprecision(2)
                 << acc.balance << endl;
        }

        // ---- TRANSACTION HISTORY ----
        else if (choice == 4) {
            showHistory(acc.accountNumber);
        }

        // ---- LOGOUT ----
        else if (choice == 5) {
            cout << ">> Logged out successfully. Goodbye, "
                 << decodeSpaces(acc.name) << "!" << endl;
        }

    } while (choice != 5);
}



int main() {

    SetConsoleOutputCP(65001);

    int choice;

    do {
        showMainMenu();
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << ">> Invalid! Please enter 1, 2, or 3." << endl;
        }
        else if (choice == 1) {
            createAccount();
        }
        else if (choice == 2) {
            loginAccount();
        }
        else if (choice == 3) {
            cout << "\n==========================================" << endl;
            cout << "   Thank you for banking with us!         " << endl;
            cout << "   Goodbye!                               " << endl;
            cout << "==========================================" << endl;
        }

    } while (choice != 3);

    return 0;
}
