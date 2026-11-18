#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

class BankingException : public std::runtime_error {
public:
    explicit BankingException(const std::string& message)
        : std::runtime_error(message) {}
};

class InsufficientFundsException : public BankingException {
public:
    InsufficientFundsException()
        : BankingException("Transaction rejected: insufficient account balance.") {}
};

class InvalidAmountException : public BankingException {
public:
    InvalidAmountException()
        : BankingException("Transaction rejected: amount must be greater than zero.") {}
};

class AccountBlockedException : public BankingException {
public:
    AccountBlockedException()
        : BankingException("Transaction rejected: account is blocked.") {}
};

class BankAccount {
private:
    std::string accountHolder;
    double balance;
    bool blocked;

public:
    BankAccount(std::string holder, double openingBalance)
        : accountHolder(std::move(holder)), balance(openingBalance), blocked(false) {}

    void blockAccount() { blocked = true; }

    void deposit(double amount) {
        if (blocked) throw AccountBlockedException();
        if (amount <= 0) throw InvalidAmountException();
        balance += amount;
        std::cout << "Deposit successful. Balance: " << balance << '\n';
    }

    void withdraw(double amount) {
        if (blocked) throw AccountBlockedException();
        if (amount <= 0) throw InvalidAmountException();
        if (amount > balance) throw InsufficientFundsException();
        balance -= amount;
        std::cout << "Withdrawal successful. Balance: " << balance << '\n';
    }

    void showBalance() const {
        std::cout << accountHolder << "'s balance: " << balance << '\n';
    }
};

int main() {
    BankAccount account("Ananya", 1500.0);
    account.showBalance();

    try {
        account.deposit(500.0);
        account.withdraw(700.0);
        account.withdraw(5000.0); // demonstrates insufficient funds
    } catch (const InsufficientFundsException& ex) {
        std::cerr << ex.what() << '\n';
    } catch (const InvalidAmountException& ex) {
        std::cerr << ex.what() << '\n';
    } catch (const AccountBlockedException& ex) {
        std::cerr << ex.what() << '\n';
    } catch (const BankingException& ex) {
        std::cerr << "Banking error: " << ex.what() << '\n';
    }

    try {
        account.deposit(-50.0); // demonstrates invalid amount
    } catch (const BankingException& ex) {
        std::cerr << ex.what() << '\n';
    }

    account.showBalance();
}
