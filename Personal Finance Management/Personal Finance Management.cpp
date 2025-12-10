#include <iostream>
#include "Transaction.h"

using namespace std;

int main() {
    TransactionManager app("data.txt");
    cout << "=== CORE SYSTEM TEST (WITH WALLET) ===" << endl;

    // 1. Thêm dữ liệu mẫu (Nhớ tham số thứ 5 là Wallet)
    // app.addTransaction(55000, "An sang", "2023-12-08", "EXPENSE", "Tien Mat", "Pho Bo");
    // app.addTransaction(250000, "Internet", "2023-12-08", "EXPENSE", "VIB Bank", "Cuoc thang 12");
    // app.addTransaction(15000000, "Luong", "2023-12-01", "INCOME", "VIB Bank", "Cong ty Tra");

    // 2. Hiển thị
    DataList<Transaction>& list = app.getList();
    cout << "Tong so giao dich: " << list.size() << endl;

    for (int i = 0; i < list.size(); i++) {
        Transaction t = list[i];
        cout << "[" << t.id << "] "
            << t.date.toString() << " | "
            << t.wallet << " | "        // <--- In ra tên ví
            << t.category << " : "
            << t.amount << endl;
    }

    return 0;
}