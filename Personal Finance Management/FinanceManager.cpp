
#include <fstream>
#include <iostream>
#include <iomanip>
#include "DataList.h"
#include "FinanceManager.h"
#include "Wallet.h"
#include "Transaction.h"


using namespace std;

FinanceManager::FinanceManager() {
    if (incomeCategory.size() == 0) {
        incomeCategory.add(Income(1, "Luong"));
        incomeCategory.add(Income(2, "Thuong"));
        incomeCategory.add(Income(3, "Cho thue nha"));
    }

    if (expenseCategory.size() == 0) {
        expenseCategory.add(Expense(1, "An uong"));
        expenseCategory.add(Expense(2, "Di chuyen"));
        expenseCategory.add(Expense(3, "Tien nha"));
        expenseCategory.add(Expense(4, "Mua sam"));
    }
}

void FinanceManager::refreshNextIds() {
    for (int i = 0; i < wallets.size(); i++)
        if (wallets[i].id >= nextWalletId) nextWalletId = wallets[i].id + 1;

    for (int i = 0; i < transactions.size(); i++)
        if (transactions[i].id >= nextTransactionId) nextTransactionId = transactions[i].id + 1;

    for (int i = 0; i < incomeCategory.size(); i++)
        if (incomeCategory[i].id >= nextIncomeId) nextIncomeId = incomeCategory[i].id + 1;

    for (int i = 0; i < expenseCategory.size(); i++)
        if (expenseCategory[i].id >= nextExpenseId) nextExpenseId = expenseCategory[i].id + 1;
}


void FinanceManager::addWallet(string name, long long initialBalance) {
    int newId = wallets.size() + 1;
    Wallet w(newId, name, initialBalance);
    wallets.add(w);
    cout << ">> Vi '" << name << "' da duoc them thanh cong voi so du: " << initialBalance << endl;
}

void FinanceManager::addTransaction(int walletId, int categoryId, TransactionType type, double amount, string desc, Date date) {
    int walletIndex = -1;
    for (int i = 0; i < wallets.size(); i++) {
        if (wallets[i].id == walletId) { 
            walletIndex = i; 
            break; 
        }
    }

    if (walletIndex == -1) {
        cout << "Loi: Khong tim thay Vi co ID " << walletId << endl;
        return;
    }

    bool categoryFound = false;
    string categoryName = ""; 

    if (type == INCOME) {
        for (int i = 0; i < incomeCategory.size(); i++) {
            if (incomeCategory[i].id == categoryId) {
                categoryFound = true;
                categoryName = incomeCategory[i].name;
                break;
            }
        }
    }
    else {
        for (int i = 0; i < expenseCategory.size(); i++) {
            if (expenseCategory[i].id == categoryId) {
                categoryFound = true;
                categoryName = expenseCategory[i].name;
                break;
            }
        }
    }

    if (!categoryFound) {
        cout << "Loi: Khong tim thay ID danh muc ban muon trong danh sach "
            << (type == INCOME ? "Thu nhap" : "Chi tieu") << endl;
        return;
    }

    if (type == INCOME) {
        wallets[walletIndex].balance += amount;
    }
    else {
        if (wallets[walletIndex].balance < amount) {
            cout << "Canh bao: So du khong du! Van tiep tuc? (1: Co, 0: Khong): ";
            int choice;
            cin >> choice;
            if (choice == 0) return;
        }
        wallets[walletIndex].balance -= amount;
    }

    Transaction t;
    t.id = nextTransactionId++;
    t.walletId = walletId;
    t.CategoryId = categoryId;
    t.type = type;
    t.amount = amount;
    t.description = desc;
    t.date = date;

    transactions.add(t);
    cout << " Giao dich thanh cong!\n";
}

void FinanceManager::addIncomeCategory(string name) {
    if (name.empty()) {
        cout << "Loi: Ten danh muc khong duoc de trong.\n";
        return;
    }
    Income inc(nextIncomeId++, name);
    incomeCategory.add(inc);
    cout << "Da them Nguon thu: '" << name << "' (ID: " << inc.id << ")\n";
}

void FinanceManager::addExpenseCategory(string name) {
    if (name.empty()) {
        cout << "Loi: Ten danh muc khong duoc de trong.\n";
        return;
    }
    Expense exp(nextExpenseId++, name);
    expenseCategory.add(exp);
    cout << "Da them nguon chi: '" << name << "' (ID: " << exp.id << ")\n";
}

void FinanceManager::addRecurring(int type, int cateId, int walletId, double amount,
    Date start, Date end, string desc) {
    RecurringTransaction rt(type, cateId, walletId, amount, start, end, desc);
    recurringTransactions.add(rt);
    cout << ">> Da cai dat lich tu " << start << " den " << end << endl;
}

void FinanceManager::processRecurringTransaction() {
    Date today;
    int count = 0;
    for (int i = 0; i < recurringTransactions.size(); i++) {
        RecurringTransaction& rt = recurringTransactions[i];
        if (rt.isFinished) continue;
        while (rt.nextDueDate <= today && rt.nextDueDate <= rt.endDate) {
            TransactionType tType = (rt.type == 1) ? INCOME : EXPENSE;
            addTransaction(rt.walletId, rt.CategoryId, tType, rt.amount,
                rt.description + " (Dinh ky)", rt.nextDueDate);
            rt.nextDueDate.addOneMonth();
            count++;
        }
        if (rt.nextDueDate > rt.endDate) {
            rt.isFinished = true;
        }
    }
    if (count > 0) cout << "Da tu dong tao " << count << " giao dich dinh ky.\n";
}

void FinanceManager::showTransactionFromDate(Date d) {
    cout << "\n--- DANH SACH GIAO DICH ---\n";
    cout << left << setw(5) << "ID"
        << setw(12) << "Ngay"
        << setw(10) << "Loai (Thu - Chi)"
        << right << setw(15) << "So tien giao dich" << "  "
        << left << "Mo ta" << endl;
    for (int i = 0; i < transactions.size(); i++) {
        if (transactions[i].date > d) {
            cout << transactions[i];
        }
    }
}

void FinanceManager::showIncomeCategory() {
    cout << "\n--- DANH SACH NGUON THU ---\n";
    cout << left << setw(5) << "ID" << "Ten Nguon Thu" << endl;
    for (int i = 0; i < incomeCategory.size(); i++) {
        cout << incomeCategory[i] << endl;
    }
}

void FinanceManager::showExpenseCategory() {
    cout << "\n--- DANH SACH NGUON CHI ---\n";
    cout << left << setw(5) << "ID" << "Ten Nguon Chi" << endl;
    for (int i = 0; i < expenseCategory.size(); i++) {
        cout << expenseCategory[i] << endl;
    }
}

void FinanceManager::showWallets() {
    cout << "\n---------------- DANH SACH VI DANG CO----------------" << endl;
    cout << left << setw(5) << "ID"
        << left << setw(20) << "Ten Vi"
        << right << setw(15) << "So Du (VND)" << endl;
    for (int i = 0; i < wallets.size(); i++) {
        cout << wallets[i];
    }
}

int FinanceManager::findWalletIndexByID(int id) {
    for (int i = 0; i < wallets.size(); i++) {
        if (wallets.get(i).id == id) return i;
    }
    cout << "Khong tim thay vi.";
    return -1;
}
