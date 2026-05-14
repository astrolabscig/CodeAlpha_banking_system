# Banking System

A console-based Banking System built in C++ as part of my CodeAlpha internship.

## Features
- Create a bank account with full name, account number, and PIN
- PIN is masked with asterisks while typing
- Login with account number and PIN verification
- Deposit money into your account
- Withdraw money with insufficient funds protection
- Check current account balance
- View full transaction history
- All account data and transactions persist after restart

## Validation Rules
| Field | Rule |
|-------|------|
| Account Number | Exactly 4 digits, no duplicates allowed |
| PIN | Exactly 4 digits |
| Deposit Amount | Must be greater than 0 |
| Withdrawal Amount | Must be greater than 0 and not exceed balance |
| Menu Input | Rejects letters and out-of-range numbers |

## How To Run
1. Compile: g++ banking_system.cpp -o banking_system
2. Run: banking_system (Windows)

## Sample Usage


==========================================
BANKING SYSTEM
CodeAlpha Internship

1.Create Account
2.Login
3.Exit

--- CREATE ACCOUNT ---
Enter your full name    : Astro Lab
Choose an account number: 1001
Choose a PIN            : ****


Account created successfully!


--- LOGIN ---
Enter account number: 1001
Enter PIN           : ****


Welcome, Astro Lab!



1.Deposit
2.Withdraw
3.Check Balance
4.Transaction History
5.Logout

Enter deposit amount: GHS 500.00


GHS 500.00 deposited successfully!
New balance: GHS 500.00



## Data Storage
| File | What It Stores |
|------|---------------|
| accounts.txt | Account number, name, PIN, balance |
| transactions.txt | Account number, transaction type, amount |

## Built With
- C++
- Concepts: structs, file I/O, file updating,
  transaction logging, PIN masking, input validation

## Note
- PIN masking uses conio.h — Windows only
- Delete accounts.txt and transactions.txt before
  pushing to GitHub

