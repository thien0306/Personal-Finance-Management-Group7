
#include <fstream>
#include <iostream>
#include <iomanip>
#include "DataList.h"
#include "FinanceManager.h"
#include "Wallet.h"
#include "Transaction.h"
#include "Date.h"


using namespace std;

FinanceManager::FinanceManager() {
    
}

FinanceManager::~FinanceManager() {

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
    for (int i = 0; i < recurringTransactions.size(); i++) {
        if (recurringTransactions[i].id >= nextRecurringId) nextRecurringId = recurringTransactions[i].id + 1;
    }
}

double FinanceManager::totalbalance() {
    double total = 0;
    for (int i = 0; i < wallets.size(); i++) {
        total += wallets[i].balance;
    }
    return total;
}

void FinanceManager::addWallet(string name, long long initialBalance) {
    int newId = wallets.size() + 1;
    Wallet w(newId, name, initialBalance);
    wallets.add(w);
    cout << ">> Vi '" << name << "' da duoc them thanh cong voi so du: " << initialBalance << " VND" << endl;
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
            cout << "\nCanh bao: So du khong du! Van tiep tuc? (1: Co, 0: Khong): ";
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

void FinanceManager::addRecurring(TransactionType type, int cateId, int walletId, double amount,
    Date start, Date end, string desc) {
    RecurringTransaction rt;
    rt.id = nextRecurringId++;
    rt.type = type;
    rt.categoryId = cateId;
    rt.walletId = walletId;
    rt.amount = amount;
    rt.startDate = start;
    rt.endDate = end;
    rt.desc = desc;
    rt.lastProcessedDate = { 0,0,0 };
    recurringTransactions.add(rt);
    cout << " Da them giao dich dinh ki moi"<< endl;
}

bool FinanceManager::processRecurringTransactions(Date currentDate) {
    bool hasChanges = false;
    int count = 0;

    for (int i = 0; i < recurringTransactions.size(); i++) {
        RecurringTransaction& rt = recurringTransactions[i];
        Date checkPoint;
        if (rt.lastProcessedDate.getDay() == 0) {
            checkPoint = rt.startDate;
            if (checkPoint.month == 1) {
                checkPoint.month = 12;
                checkPoint.year--;
            }
            else {
                checkPoint.month--;
            }
        }
        else {
            checkPoint = rt.lastProcessedDate;
        }

        while (true) {
            checkPoint.addOneMonth();
            Date nextDueDate = checkPoint;
            if (nextDueDate > currentDate) break;
            if (!rt.endDate.isIndefinite() && nextDueDate > rt.endDate) break;
            if (nextDueDate < rt.startDate) {
                checkPoint = nextDueDate;
                continue;
            }

            addTransaction(
                rt.walletId,
                rt.categoryId,
                (TransactionType)rt.type,
                rt.amount,
                rt.desc,
                nextDueDate
            );

            count++;
            checkPoint = nextDueDate;
            rt.lastProcessedDate = nextDueDate;
            hasChanges = true;
        }
    }

    if (hasChanges) {
        saveData();
        cout << "Da xu ly xong giao dich dinh ki con thieu." << endl;
    }
    return hasChanges;
}

void FinanceManager::showRecurringTransaction() {
    cout << "\n---------------- DANH SACH GIAO DICH DINH KY ----------------" << endl;
    cout << setfill(' ');
    cout << left << setw(5) << "ID"
        << setw(10) << "Loai"
        << setw(15) << "So tien (VND)"
        << setw(15) << "Vi"
        << setw(20) << "Danh muc"
        << setw(12) << "Bat dau"
        << setw(15) << "Ket thuc"
        << "Mo ta" << endl;

    for (int i = 0; i < recurringTransactions.size(); i++) {
        RecurringTransaction t = recurringTransactions[i];

        string walletName = "";
        for (int j = 0; j < wallets.size(); j++) {
            if (wallets[j].id == t.walletId) {
                walletName = wallets[j].name;
                break;
            }
        }

        string catName = "";
        if (t.type == 1) {
            for (int k = 0; k < incomeCategory.size(); k++) {
                if (incomeCategory[k].id == t.categoryId) {
                    catName = incomeCategory[k].name;
                    break;
                }
            }
        }
        else {
            for (int k = 0; k < expenseCategory.size(); k++) {
                if (expenseCategory[k].id == t.categoryId) {
                    catName = expenseCategory[k].name;
                    break;
                }
            }
        }

        cout << left << setw(5) << t.id
            << setw(10) << (t.type == 1 ? "Thu" : "Chi")
            << setw(15) << (long long)t.amount
            << setw(15) << walletName
            << setw(20) << catName;

        cout << t.startDate;
        cout << "   ";

        if (t.endDate.getDay() == 0 && t.endDate.getMonth() == 0 && t.endDate.getYear() == 0) {
            cout << setw(15) << "Vo thoi han";
        }
        else {
            cout << t.endDate;
            cout << "     ";
        }
        cout << " " << t.desc << endl;
    }
}

void FinanceManager::removeRecurringTransaction(int id) {
    int index = -1;
    for (int i = 0; i < recurringTransactions.size(); i++) {
        if (recurringTransactions[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "Loi: Khong tim thay giao dich  co ID = " << id << endl;
        return;
    }
    recurringTransactions.removeAt(index);
    cout << "Da xoa thanh cong giao dich dinh ky co ID: " << id << endl;
}

void FinanceManager::removeIncomeCategory(int id) {
    int index = -1;
    for (int i = 0; i < incomeCategory.size(); i++) {
        if (incomeCategory[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "Loi: Khong tim thay category  co ID = " << id << endl;
        return;
    }
    incomeCategory.removeAt(index);
    cout << "Da xoa thanh cong category co ID: " << id << endl;
}

void FinanceManager::removeExpenseCategory(int id) {
    int index = -1;
    for (int i = 0; i < expenseCategory.size(); i++) {
        if (expenseCategory[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "Loi: Khong tim thay category co ID = " << id << endl;
        return;
    }
    expenseCategory.removeAt(index);
    cout << "Da xoa thanh cong category co ID: " << id << endl;
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
    cout << setfill(' ');
    cout << "\n---------------- DANH SACH VI DANG CO----------------" << endl;
    cout << left << setw(5) << "ID"
        << left << setw(20) << "Ten Vi"
        << right << setw(15) << "So Du (VND)" << endl;
    for (int i = 0; i < wallets.size(); i++) {
        cout << wallets[i] << endl;
    }
}

void FinanceManager::showTransactionFromDate(Date from, Date to) {
    cout << setfill(' ');
    cout << "\n---------------- DANH SACH GIAO DICH ----------------\n";
    cout << "Tu ngay: " << from << "  Den ngay: " << to << endl;
    cout << left << setw(5) << "ID"
        << left << setw(12) << "Loai"
        << left << setw(12) << "Ngay"
        << left << setw(25) << "Category"
        << left << setw(15) << "Vi"
        << right << setw(15) << "So Tien(VND)"
        << "  Mo Ta" << endl;
    for (int i = 0; i < transactions.size(); i++) {
        Transaction& t = transactions[i];
        if (t.date >= from && t.date <= to) {
            string walletName = "";
            for (int j = 0; j < wallets.size(); j++) {
                if (wallets[j].id == t.walletId) {
                    walletName = wallets[j].name;
                    break;
                }
            }

            string catName = "";
            if (t.type == INCOME) {
                for (int i = 0; i < incomeCategory.size(); i++) {
                    if (incomeCategory[i].id == t.CategoryId) {
                        catName = incomeCategory[i].name;
                        break;
                    }
                }
            }
            else {
                for (int i = 0; i < expenseCategory.size(); i++) {
                    if (expenseCategory[i].id == t.CategoryId) {
                        catName = expenseCategory[i].name;
                        break;
                    }
                }
            }
            cout << left << setw(5) << t.id
                << setw(12) << (t.type == INCOME ? "Thu" : "Chi")
                << setw(12) << t.date << "   "
                << setw(25) << catName
                << setw(15) << walletName
                << right << setw(15) << t.amount << "  "
                << left << t.description << endl;

        }
    }
}

void FinanceManager::statisticByDate(Date from, Date to) {
    double totalIncome = 0;
    double totalExpense = 0;
    for (int i = 0; i < transactions.size(); i++) {
        Transaction& t = transactions[i];
        if (t.date >= from && t.date <= to) {
            if (t.type == INCOME) totalIncome += t.amount;
            else totalExpense += t.amount;
        }
    }
    cout << "\n----------- THONG KE TU " << from << " DEN " << to << " -----------\n";
    cout << left << setw(20) << "Tong Thu:"
        << right << setw(15) << totalIncome <<" VND" << endl;
    cout << left << setw(20) << "Tong Chi:"
        << right << setw(15) << totalExpense << " VND" << endl;
    cout << left << setw(20) << "So Du:"
        << right << setw(15) << totalIncome - totalExpense << " VND" << endl;
}

void FinanceManager::statisticByWallet(Date from, Date to) {
    cout << "\n------ THONG KE THEO VI ------\n";
    cout << left << setw(5) << "ID"
        << left << setw(20) << "Ten Vi"
        << right << setw(15) << "Thu"
        << right << setw(15) << "Chi"
        << right << setw(15) << "So Du" << endl;
    for (int w = 0; w < wallets.size(); w++) {
        Wallet& wallet = wallets[w];
        double income = 0, expense = 0;
        for (int i = 0; i < transactions.size(); i++) {
            Transaction& t = transactions[i];
            if (t.walletId == wallet.id &&
                t.date >= from && t.date <= to) {

                if (t.type == INCOME) income += t.amount;
                else expense += t.amount;
            }
        }
        cout << left << setw(5) << wallet.id
            << left << setw(20) << wallet.name
            << right << setw(15) << income << " VND"
            << right << setw(15) << expense << " VND"
            << right << setw(15) << income - expense << " VND" << endl;
    }
}

void FinanceManager::transactionInYear(int year) {
    double totalIncome = 0;
    double totalExpense = 0;
    for (int i = 0; i < transactions.size(); i++) {
        Transaction& t = transactions[i];
        if (t.date.getYear() == year) {
            if (t.type == INCOME) totalIncome += t.amount;
            else totalExpense += t.amount;
        }
    }
    cout << "\n----------- THONG KE NAM " << year << " ---------- - \n";
    cout << left << setw(20) << "Tong Thu:"
        << right << setw(15) << totalIncome << " VND" << endl;
    cout << left << setw(20) << "Tong Chi:"
        << right << setw(15) << totalExpense << " VND" << endl;
    cout << left << setw(20) << "So Du:"
        << right << setw(15) << totalIncome - totalExpense << " VND" << endl;
}

void FinanceManager::incomeBreakdownByYear(int year) {
    cout << "\n--- THONG KE NGUON THU NAM " << year << " ---\n";
    cout << left << setw(5) << "ID"
        << left << setw(25) << "Ten Nguon Thu"
        << right << setw(15) << "Tong Tien" << endl;
    for (int c = 0; c < incomeCategory.size(); c++) {
        Income& inc = incomeCategory[c];
        double total = 0;
        for (int i = 0; i < transactions.size(); i++) {
            Transaction& t = transactions[i];
            if (t.type == INCOME &&
                t.CategoryId == inc.id &&
                t.date.getYear() == year) {
                total += t.amount;
            }
        }
        cout << left << setw(5) << inc.id
            << left << setw(25) << inc.name
            << right << setw(15) << total << " VND" << endl;
    }
}

void FinanceManager::expenseBreakdownByYear(int year) {
    cout << "\n--- THONG KE NGUON CHI NAM " << year << " ---\n";
    cout << left << setw(5) << "ID"
        << left << setw(25) << "Ten Nguon Chi"
        << right << setw(15) << "Tong Tien" << endl;
    for (int c = 0; c < expenseCategory.size(); c++) {
        Expense& exp = expenseCategory[c];
        double total = 0;
        for (int i = 0; i < transactions.size(); i++) {
            Transaction& t = transactions[i];
            if (t.type == EXPENSE &&
                t.CategoryId == exp.id &&
                t.date.getYear() == year) {
                total += t.amount;
            }
        }
        cout << left << setw(5) << exp.id
            << left << setw(25) << exp.name
            << right << setw(15) << total << " VND" << endl;
    }
}

void writeString(ofstream& out, const string& s) {
    int len = s.length();
    out.write((char*)&len, sizeof(int));
    out.write(s.c_str(), len);
}

void readString(ifstream& in, string& s) {
    int len;
    in.read((char*)&len, sizeof(int));
    char* buffer = new char[len + 1];
    in.read(buffer, len);
    buffer[len] = '\0';
    s = buffer;
    delete[] buffer;
}

void FinanceManager::saveData() {
    ofstream out("finance.dat", ios::binary);
    if (!out) return;

    //Wallet
    int walletCount = wallets.size();
    out.write((char*)&walletCount, sizeof(int));
    for (int i = 0; i < walletCount; i++) {
        Wallet& w = wallets[i];

        out.write((char*)&w.id, sizeof(int));
        writeString(out, w.name);
        out.write((char*)&w.balance, sizeof(double));
    }

    //Income
    int incCount = incomeCategory.size();
    out.write((char*)&incCount, sizeof(int));
    for (int i = 0; i < incCount; i++) {
        Income& inc = incomeCategory[i];

        out.write((char*)&inc.id, sizeof(int));
        writeString(out, inc.name);
    }

    //Expense
    int expCount = expenseCategory.size();
    out.write((char*)&expCount, sizeof(int));
    for (int i = 0; i < expCount; i++) {
        Expense& exp = expenseCategory[i];

        out.write((char*)&exp.id, sizeof(int));
        writeString(out, exp.name);
    }

    //Transaction
    int transCount = transactions.size();
    out.write((char*)&transCount, sizeof(int));
    for (int i = 0; i < transCount; i++) {
        Transaction& t = transactions[i];

        out.write((char*)&t.id, sizeof(int));
        out.write((char*)&t.type, sizeof(int));
        out.write((char*)&t.CategoryId, sizeof(int));
        out.write((char*)&t.walletId, sizeof(int));
        out.write((char*)&t.amount, sizeof(double));
        out.write((char*)&t.date, sizeof(Date));
        writeString(out, t.description);
    }

    //Recurring Transaction
    int recurCount = recurringTransactions.size();
    out.write((char*)&recurCount, sizeof(int));
    for (int i = 0; i < recurCount; i++) {
        RecurringTransaction& r = recurringTransactions[i];

        out.write((char*)&r.id, sizeof(int));
        out.write((char*)&r.type, sizeof(int));
        out.write((char*)&r.categoryId, sizeof(int));
        out.write((char*)&r.walletId, sizeof(int));
        out.write((char*)&r.amount, sizeof(double));
        out.write((char*)&r.startDate, sizeof(Date));
        out.write((char*)&r.endDate, sizeof(Date));
        out.write((char*)&r.lastProcessedDate, sizeof(Date));
        writeString(out, r.desc);
    }

    out.close();
}

void FinanceManager::loadData() {
    ifstream in("finance.dat", ios::binary);
    if (!in) return;

    wallets.clear();
    incomeCategory.clear();
    expenseCategory.clear();
    transactions.clear();
    recurringTransactions.clear();

    //Wallet
    int walletCount;
    in.read((char*)&walletCount, sizeof(int));
    for (int i = 0; i < walletCount; i++) {
        Wallet w;
        in.read((char*)&w.id, sizeof(int));
        readString(in, w.name);
        in.read((char*)&w.balance, sizeof(double));

        wallets.add(w);
    }

    //Income
    int incCount;
    in.read((char*)&incCount, sizeof(int));
    for (int i = 0; i < incCount; i++) {
        Income inc;
        in.read((char*)&inc.id, sizeof(int));
        readString(in, inc.name);

        incomeCategory.add(inc);
    }

    //Expense
    int expCount;
    in.read((char*)&expCount, sizeof(int));
    for (int i = 0; i < expCount; i++) {
        Expense exp;
        in.read((char*)&exp.id, sizeof(int));
        readString(in, exp.name);

        expenseCategory.add(exp);
    }

    //Transaction
    int transCount;
    in.read((char*)&transCount, sizeof(int));
    for (int i = 0; i < transCount; i++) {
        Transaction t;
        in.read((char*)&t.id, sizeof(int));
        in.read((char*)&t.type, sizeof(int));
        in.read((char*)&t.CategoryId, sizeof(int));
        in.read((char*)&t.walletId, sizeof(int));
        in.read((char*)&t.amount, sizeof(double));
        in.read((char*)&t.date, sizeof(Date));
        readString(in, t.description);

        transactions.add(t);
    }

    //Recurring Transaction
    int recurCount;
    in.read((char*)&recurCount, sizeof(int));
    for (int i = 0; i < recurCount; i++) {
        RecurringTransaction r;
        in.read((char*)&r.id, sizeof(int));
        in.read((char*)&r.type, sizeof(int));
        in.read((char*)&r.categoryId, sizeof(int));
        in.read((char*)&r.walletId, sizeof(int));
        in.read((char*)&r.amount, sizeof(double));
        in.read((char*)&r.startDate, sizeof(Date));
        in.read((char*)&r.endDate, sizeof(Date));
        in.read((char*)&r.lastProcessedDate, sizeof(Date));
        readString(in, r.desc);

        recurringTransactions.add(r);
    }

    in.close();
    refreshNextIds();
}