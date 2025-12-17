#pragma once

#include <string>
#include "Transaction.h"
#include "IncomeSource.h"
#include "RecurringTransaction.h"
#include "Expense.h"
#include "Wallet.h"
#include "DataList.h"

using namespace std;

class FinanceManager {
private:
    DataList<Wallet> wallets;
    DataList<Transaction> transactions;
    DataList<Income> incomeCategory;
    DataList<Expense> expenseCategory;
    DataList<RecurringTransaction> recurringTransactions;
    int findWalletIndexByID(int id);
    int nextWalletId = 1;
    int nextTransactionId = 1;
    int nextIncomeId = 1;
    int nextExpenseId = 1;
    void refreshNextIds();
public:

    FinanceManager();
    ~FinanceManager();
    void addWallet(string name, long long initialBalance);
    void addTransaction(int walletId, int categoryId, TransactionType type, double amount, string desc, Date date);
    void addExpenseCategory(string name);
    void addIncomeCategory(string name);

    void addRecurring(int type, int catId, int walletId, double amount, Date start, Date end, string desc);
    void processRecurringTransaction();

    void showWallets();
    void showTransactionFromDate(Date d);
    void showIncomeCategory();
    void showExpenseCategory();

    void loadData();
    void saveData();
};


