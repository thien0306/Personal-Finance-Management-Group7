#pragma once

#include <string>
#include "Model.h"
#include "DataList.h"

using namespace std;

class FinanceManager {
private:
    DataList<Wallet> wallets;
    DataList<Transaction> transactions;
    DataList<IncomeCategory> incomeCats;
    DataList<ExpenseCategory> expenseCats;
    int findWalletIndexByID(int id);
public:

    FinanceManager();
    ~FinanceManager();

    void addWallet(string name, long long initialBalance);
    void showWallets();

    void addTransaction(int walletId, int catId, long long amount, std::string desc);

    void loadData();
    void saveData();
};


