#pragma once

#include <string>
#include "Transaction.h"
#include "IncomeSource.h"
#include "RecurringTransaction.h"
#include "Expense.h"
#include "Wallet.h"
#include "DataList.h"

using namespace std;

void writeString(ofstream& out, const string& s);
void readString(ifstream& in, string& s);

class FinanceManager {
private:
    DataList<Wallet> wallets;
    DataList<Transaction> transactions;
    DataList<Income> incomeCategory;
    DataList<Expense> expenseCategory;
    DataList<RecurringTransaction> recurringTransactions;
    int nextWalletId = 1;
    int nextTransactionId = 1;
    int nextIncomeId = 1;
    int nextExpenseId = 1;
    int nextRecurringId = 1;
public:

    FinanceManager();
    ~FinanceManager();
    void refreshNextIds();
    double totalbalance();
    void addWallet(string name, long long initialBalance);
    void addTransaction(int walletId, int categoryId, TransactionType type, double amount, string desc, Date date);
    void addExpenseCategory(string name);
    void addIncomeCategory(string name);

    void showRecurringTransaction();
    void addRecurring(TransactionType type, int cateId, int walletId, double amount, Date start, Date end, string desc);
    bool processRecurringTransactions(Date currentDate);

    void removeIncomeCategory(int id);
    void removeExpenseCategory(int id);
    void removeRecurringTransaction(int id);

    void showWallets();
    void showIncomeCategory();
    void showExpenseCategory();

    void showTransactionFromDate(Date from, Date to);
    void statisticByDate(Date from, Date to);
    void transactionInYear(int year);
    void statisticByWallet(Date from, Date to);
    void incomeBreakdownByYear(int year);
    void expenseBreakdownByYear(int year);

    void loadData();
    void saveData();
};


