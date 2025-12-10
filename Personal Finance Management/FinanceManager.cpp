
#include <fstream>
#include <iostream>
#include <iomanip>
#include "DataList.h"
#include "FinanceManager.h"

using namespace std;

FinanceManager::FinanceManager() {
    if (incomeCats.size() == 0) {
        incomeCats.add(IncomeCategory(1, "Luong"));
        incomeCats.add(IncomeCategory(2, "Thuong"));
        incomeCats.add(IncomeCategory(3, "Lai suat"));
    }

    if (expenseCats.size() == 0) {
        expenseCats.add(ExpenseCategory(1, "An uong"));
        expenseCats.add(ExpenseCategory(2, "Di chuyen"));
        expenseCats.add(ExpenseCategory(3, "Tien nha"));
        expenseCats.add(ExpenseCategory(4, "Mua sam"));
    }
}



void FinanceManager::addWallet(string name, long long initialBalance) {
    int newId = wallets.size() + 1;
    Wallet w(newId, name, initialBalance);
    wallets.add(w);
    cout << ">> Vi '" << name << "' da duoc them thanh cong voi so du: " << initialBalance << endl;
}

void FinanceManager::showWallets() {
    cout << "\n---------------- DANH SACH VI DANG CO----------------" << endl;
    cout << left << setw(5) << "ID"
        << left << setw(20) << "Ten Vi"
        << right << setw(15) << "So Du (VND)" << endl;
    for (int i = 0; i < wallets.size(); i++) {
        Wallet w = wallets.get(i);
        cout << left << setw(5) << w.id
            << left << setw(20) << w.name
            << right << setw(15) << w.balance << endl;
    }
}

int FinanceManager::findWalletIndexByID(int id) {
    for (int i = 0; i < wallets.size(); i++) {
        if (wallets.get(i).id == id) return i;
    }
    cout << "Khong tim thay vi.";
    return -1;
}
