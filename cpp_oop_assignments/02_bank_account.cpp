#include <iostream>
#include <string>

class BankAccount {
private:
    std::string owner;
    double balance;

protected:
    void setBalance(double amount) { balance = amount; }

public:
    BankAccount(std::string name, double openingBalance)
        : owner(std::move(name)), balance(openingBalance < 0 ? 0 : openingBalance) {}

    virtual ~BankAccount() = default;

    std::string getOwner() const { return owner; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        if (amount > 0) balance += amount;
    }

    virtual bool withdraw(double amount) {
        if (amount <= 0 || amount > balance) return false;
        balance -= amount;
        return true;
    }

    virtual void display() const {
        std::cout << owner << " | Balance: " << balance << '\n';
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(std::string name, double amount, double rate)
        : BankAccount(std::move(name), amount), interestRate(rate) {}

    void addAnnualInterest() {
        if (interestRate > 0)
            deposit(getBalance() * interestRate / 100.0);
    }

    void display() const override {
        BankAccount::display();
        std::cout << "Interest rate: " << interestRate << "%\n";
    }
};

int main() {
    SavingsAccount account("Riya", 1000.0, 4.5);
    account.deposit(250);
    account.withdraw(100);
    account.addAnnualInterest();
    account.display();
}
