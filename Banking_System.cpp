#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

// Transaction Class
class Transaction {
public:
    string type; // Deposit, Withdraw, Transfer
    double amount;
    string details;
    string timestamp;

    Transaction(string t, double amt, string det) {
        type = t;
        amount = amt;
        details = det;
        // Get current time
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // remove newline
    }

    void display() const {
        cout << left << setw(12) << type
             << setw(10) << amount
             << setw(25) << details
             << timestamp << endl;
    }
};

// Account Class
class Account {
public:
    int accountNumber;
    double balance;
    vector<Transaction> history;

    Account(int accNo, double initialBalance = 0) {
        accountNumber = accNo;
        balance = initialBalance;
        if (initialBalance > 0) {
            history.push_back(Transaction("Deposit", initialBalance, "Initial Deposit"));
        }
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Error: Invalid deposit amount!" << endl;
            return;
        }
        balance += amount;
        history.push_back(Transaction("Deposit", amount, "Deposited to Acc " + to_string(accountNumber)));
        cout << "Success: $" << amount << " deposited. New Balance: $" << balance << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Error: Invalid withdraw amount!" << endl;
            return;
        }
        if (amount > balance) {
            cout << "Error: Insufficient balance! Available: $" << balance << endl;
            return;
        }
        balance -= amount;
        history.push_back(Transaction("Withdraw", amount, "Withdrawn from Acc " + to_string(accountNumber)));
        cout << "Success: $" << amount << " withdrawn. New Balance: $" << balance << endl;
    }

    void transferTo(Account &toAccount, double amount) {
        if (amount <= 0 || amount > balance) {
            cout << "Error: Invalid amount or insufficient balance!" << endl;
            return;
        }
        balance -= amount;
        toAccount.balance += amount;

        history.push_back(Transaction("Transfer", amount, "To Acc " + to_string(toAccount.accountNumber)));
        toAccount.history.push_back(Transaction("Transfer", amount, "From Acc " + to_string(accountNumber)));

        cout << "Success: $" << amount << " transferred from " << accountNumber << " to " << toAccount.accountNumber << endl;
    }

    void displayAccountInfo() const {
        cout << "\n--- Account Info ---" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
    }

    void displayTransactions() const {
        cout << "\n--- Transaction History for Account " << accountNumber << " ---" << endl;
        if (history.empty()) {
            cout << "No transactions yet." << endl;
            return;
        }
        cout << left << setw(12) << "Type" << setw(10) << "Amount" << setw(25) << "Details" << "Date" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        for (const auto& t : history) {
            t.display();
        }
    }
};

// Customer Class
class Customer {
public:
    int customerId;
    string name;
    vector<Account> accounts;

    Customer(int id, string n) {
        customerId = id;
        name = n;
    }

    void createAccount(int accNumber, double initialBalance) {
        accounts.push_back(Account(accNumber, initialBalance));
        cout << "Account " << accNumber << " created for customer " << name << endl;
    }

    Account* findAccount(int accNumber) {
        for (auto &acc : accounts) {
            if (acc.accountNumber == accNumber) return &acc;
        }
        return nullptr;
    }

    void displayCustomer() const {
        cout << "\nCustomer ID: " << customerId << " | Name: " << name
             << " | Total Accounts: " << accounts.size() << endl;
    }
};

// Main Banking System
int main() {
    vector<Customer> customers;
    int nextCustomerId = 1001;
    int nextAccountNo = 50001;
    int choice;

    // Create a demo customer for testing
    customers.push_back(Customer(nextCustomerId++, "John Doe"));
    customers[0].createAccount(nextAccountNo++, 1000);

    while (true) {
        cout << "\n========================================" << endl;
        cout << " BANKING SYSTEM MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. Create New Customer" << endl;
        cout << "2. Create New Account" << endl;
        cout << "3. Deposit" << endl;
        cout << "4. Withdraw" << endl;
        cout << "5. Fund Transfer" << endl;
        cout << "6. View Account Info & Transactions" << endl;
        cout << "7. Display All Customers" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            cout << "Enter customer name: ";
            cin.ignore();
            getline(cin, name);
            customers.push_back(Customer(nextCustomerId++, name));
            cout << "Customer created with ID: " << nextCustomerId - 1 << endl;
        }
        else if (choice == 2) {
            int custId;
            double initial;
            cout << "Enter Customer ID: "; cin >> custId;
            cout << "Enter Initial Balance: "; cin >> initial;
            for (auto &c : customers) {
                if (c.customerId == custId) {
                    c.createAccount(nextAccountNo++, initial);
                    break;
                }
            }
        }
        else if (choice == 3 || choice == 4 || choice == 6) {
            int accNo;
            cout << "Enter Account Number: "; cin >> accNo;
            bool found = false;
            for (auto &c : customers) {
                Account* acc = c.findAccount(accNo);
                if (acc) {
                    found = true;
                    if (choice == 3) { double amt; cout << "Enter amount: "; cin >> amt; acc->deposit(amt); }
                    else if (choice == 4) { double amt; cout << "Enter amount: "; cin >> amt; acc->withdraw(amt); }
                    else { acc->displayAccountInfo(); acc->displayTransactions(); }
                    break;
                }
            }
            if (!found) cout << "Account not found!" << endl;
        }
        else if (choice == 5) {
            int fromAcc, toAcc;
            double amt;
            cout << "Enter From Account: "; cin >> fromAcc;
            cout << "Enter To Account: "; cin >> toAcc;
            cout << "Enter Amount: "; cin >> amt;
            Account *from = nullptr, *to = nullptr;
            for (auto &c : customers) {
                if (!from) from = c.findAccount(fromAcc);
                if (!to) to = c.findAccount(toAcc);
            }
            if (from && to) from->transferTo(*to, amt);
            else cout << "One or both accounts not found!" << endl;
        }
        else if (choice == 7) {
            for (auto &c : customers) {
                c.displayCustomer();
                for (auto &acc : c.accounts) acc.displayAccountInfo();
            }
        }
        else if (choice == 8) {
            break;
        }
    }
    return 0;
}
