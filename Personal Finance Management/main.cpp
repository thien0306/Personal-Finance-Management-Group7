#include <iostream>
#include <string>
#include <limits>
#include "FinanceManager.h"
#include <sstream>
#include "Date.h"
#include <iomanip>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nNhan Enter de quay ve Menu chinh...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    cout << setprecision(8);
    FinanceManager app;
    app.loadData();
    app.refreshNextIds();
    {
        cout << "Dang kiem tra cac giao dich dinh ky:" << endl;

        Date today;
        bool hasChanged = app.processRecurringTransactions(today);

        if (hasChanged) {
            cout << "[THONG BAO] He thong da tu dong cap nhat cac giao dich den han!" << endl;
            pauseScreen();
        }
        else {
            cout << " Khong co giao dich nao can xu ly hom nay." << endl;
        }
    }
    int choice = -1;
    while (choice != 0) {
        clearScreen();
        cout << "==========================================\n";
        cout << "      QUAN LY TAI CHINH CA NHAN    \n";
        cout << "==========================================\n";
        cout << "1. Quan Ly Vi \n";
        cout << "2. Quan Ly Danh Muc \n";
        cout << "3. Them Giao Dich \n";
        cout << "4. Xem Lich Su Giao Dich\n";
        cout << "5. Bao Cao Tai Chinh \n";
        cout << "6. Giao dich dinh ky\n";
        cout << "------------------------------------------\n";
        cout << "0. Luu & Thoat\n";
        cout << "==========================================\n";
        cout << "Nhap lua chon cua ban: ";

        if (!(cin >> choice)) {
            cout << "Loi: Vui long nhap so!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pauseScreen();
            continue;
        }
        cin.ignore();
        cout << "\n";
        switch (choice) {
        case 1: {
            int wChoice = -1;
            
            while (wChoice != 0) {//Vi
                clearScreen();
                cout << "--- QUAN LY VI ---\n";
                cout << "1. Xem danh sach vi\n";
                cout << "2. Them vi moi\n";
                cout << "0. Tro ve man hinh chinh\n";
                cout << "Chon: ";
                cin >> wChoice;
                cin.ignore();
                if (wChoice == 1) {
                    app.showWallets();
                }
                else if (wChoice == 2) {
                    string name;
                    double balance;
                    cout << "Nhap ten vi: "; getline(cin, name);
                    cout << "Nhap so du ban dau: "; cin >> balance;
                    app.addWallet(name, balance);
                    cout << "Da them vi thanh cong\n";
                }
                else if (wChoice == 0) {
                    break;
                }
                pauseScreen();
            }
            break;
        }

        case 2: {//Danh muc (Income/Expense)
            int cChoice = -1;
            while (cChoice != 0) {
                clearScreen();
                cout << "--- QUAN LY DANH MUC ---\n";
                cout << "1. Xem danh muc Thu nhap\n";
                cout << "2. Xem danh muc Chi tieu\n";
                cout << "3. Them danh muc Thu nhap\n";
                cout << "4. Them danh muc Chi tieu\n";
                cout << "5. Xoa danh muc thu nhap\n";
                cout << "6. Xoa danh muc chi tieu\n";
                cout << "------------------------\n";
                cout << "0. Quay ve Menu chinh\n";
                cout << "Chon: ";
                cin >> cChoice;
                cin.ignore();
                if (cChoice == 1) app.showIncomeCategory();
                else if (cChoice == 2) app.showExpenseCategory();
                else if (cChoice == 3) {
                    string name;
                    cout << "Ten danh muc Thu nhap moi: "; getline(cin, name);
                    app.addIncomeCategory(name);
                }
                else if (cChoice == 4) {
                    string name;
                    cout << "Ten danh muc Chi tieu moi: "; getline(cin, name);
                    app.addExpenseCategory(name);
                }
                else if (cChoice == 5) {
                    app.showIncomeCategory();
                    int id;
                    cout << "Nhap id cua category ma ban muon xoa: ";
                    cin >> id;
                    app.removeIncomeCategory(id);
                }
                else if (cChoice == 6) {
                    app.showExpenseCategory();
                    int id;
                    cout << "Nhap id cua category ma ban muon xoa: ";
                    cin >> id;
                    app.removeExpenseCategory(id);
                }
                else if (cChoice == 0) {
                    break;
                }
                pauseScreen();
            }
            break;
        }

        case 3: {//Them giao dich
            clearScreen();
            cout << "--- THEM GIAO DICH MOI ---\n";
            app.showWallets();
            int walletId;
            cout << "Nhap ID Vi: "; cin >> walletId;

            cout << "Loai giao dich (1: Thu nhap, 2: Chi tieu): ";
            int type; cin >> type;
            if (type == 1) app.showIncomeCategory();
            else app.showExpenseCategory();
            int catId;
            cout << "Nhap ID Danh muc: "; cin >> catId;
            double amount;
            cout << "Nhap so tien: "; cin >> amount;
            cin.ignore();
            string desc;
            cout << "Noi dung: "; getline(cin, desc);
            cout << "Nhap ngay thang nam (dd mm yyyy): \n";
            Date date;
            cin >> date;
            app.addTransaction(walletId, catId,(TransactionType) type, amount, desc, date);
            cout << "Giao dich da duoc luu\n";
            break;
        }

        case 4: { //Lich su giao dich
            clearScreen();
            Date d1, d2;
            cout << "       LICH SU GIAO DICH      \n";
            cout << "Hay nhap vao ngay bat dau (dd mm yyyy) (Nhap 0 0 0 de quay ve menu chinh): ";
            cin >> d1;
            if (d1.day == 0) break;
            cout << "Hay nhap vao ngay ket thuc (dd mm yyyy): ";
            cin >> d2;
            app.showTransactionFromDate(d1, d2);
            pauseScreen();
            break;
        }

        case 5: { //Thong ke
            int reportChoice = -1;
            while (reportChoice != 0) {
                clearScreen();
                cout << "\n----BAO CAO & PHAN TICH TAI CHINH----\n";
                cout << "1. Thong ke Tong quan (Theo ngay)\n";
                cout << "2. Thong ke chi tiet theo vi\n";
                cout << "3. Thong ke Thu nhap (Theo nam)\n";
                cout << "4. Thong ke Chi tieu (Theo nam)\n";
                cout << "5. Thong ke theo nam\n";
                cout << "------------------------------------------\n";
                cout << "0. Quay lai Menu chinh\n";
                cout << "==========================================\n";
                cout << "Chon loai bao cao: ";
                cin >> reportChoice;

                if (reportChoice == 0) break;
                cout << "\n";
                switch (reportChoice) {
                case 1: {
                    Date from, to;
                    cout << "\n--- THONG KE THEO KHOANG THOI GIAN ---\n";
                    cout << "Tu ngay (dd mm yyyy): "; 
                    cin >> from;
                    cout << "Den ngay (dd mm yyyy): ";
                    cin >> to;
                    app.statisticByDate(from, to);
                    break;
                }
                case 2: {
                    Date from, to;
                    cout << "--- THONG KE THEO VI (TRONG KHOANG TG) ---\n";
                    cout << "Tu ngay (dd mm yyyy): "; 
                    cin >> from;
                    cout << "Den ngay (dd mm yyyy): ";
                    cin >> to;
                    app.statisticByWallet(from, to);
                    break;
                }

                case 3: {
                    int year;
                    cout << "--- THONG KE THU NHAP THEO NAM ---\n";
                    cout << "Nhap nam can xem: "; cin >> year;
                    app.incomeBreakdownByYear(year);
                    break;
                }

                case 4: {
                    int year;
                    cout << "--- THONG KE CHI TIEU THEO NAM ---\n";
                    cout << "Nhap nam can xem: ";
                    cin >> year;
                    app.expenseBreakdownByYear(year);
                    break;
                }
                case 5: {
                    int year;
                    cout << "---THONG KE THEO NAM ---\n";
                    cout << "Nhap nam can xem: ";
                    cin >> year;
                    app.transactionInYear(year);
                    break;
                }
                default:
                    cout << "Lua chon khong hop le!\n";
                }
                cout << "\nNhan Enter de tiep tuc...";
                cin.ignore(); 
                cin.get();
            }
            break;
        }

        case 6: {//Giao dich dinh ki
            int recChoice = -1;
            while (recChoice != 0) {
                clearScreen();
                cout << "\n==========================================\n";
                cout << "       QUAN LY GIAO DICH DINH KY          \n";
                cout << "==========================================\n";
                cout << "1. Xem danh sach lich dinh ky\n";
                cout << "2. Them lich moi (Co ngay ket thuc)\n";
                cout << "3. Xoa lich dinh ky\n";
                cout << "------------------------------------------\n";
                cout << "0. Quay lai Menu chinh\n";
                cout << "==========================================\n";
                cout << "Chon chuc nang: ";
                cin >> recChoice;
                cin.ignore();

                cout << "\n";

                switch (recChoice) {
                case 1: {
                    clearScreen();
                    cout << "--- DANH SACH GIAO DICH DINH KI ---\n";
                    app.showRecurringTransaction();
                    pauseScreen();
                    break;
                }

                case 2: {
                    clearScreen();
                    cout << "--- THEM LICH DINH KY MOI ---\n";
                    app.showWallets();
                    int walletId; 
                    cout << "Nhap ID Vi: "; 
                    cin >> walletId;

                    int typeInt;
                    cout << "Loai (1: Thu nhap, 2: Chi tieu): "; 
                    cin >> typeInt;
                    TransactionType type =(TransactionType) typeInt;

                    if (typeInt == 1) app.showIncomeCategory();
                    else app.showExpenseCategory();
                    int cateId; 
                    cout << "Nhap ID danh muc: "; 
                    cin >> cateId;

                    double amount; 
                    cout << "So tien: ";
                    cin >> amount;
                    cin.ignore();

                    string desc; 
                    cout << "Mo ta: "; 
                    getline(cin, desc);

                    Date start;
                    cout << "Ngay bat dau (dd mm yyyy): ";
                    cin >> start;
                    cin.ignore();

                    Date end;
                    cout << "Ngay ket thuc (dd mm yyyy) (Nhan Enter de Vo Thoi Han): ";
                    string dateInput;
                    getline(cin, dateInput);

                    if (dateInput.empty()) {
                        end.setIndefinite();
                    }
                    else {
                        stringstream ss(dateInput);
                        ss >> end;
                    }
                    app.addRecurring(type, cateId, walletId, amount, start, end, desc);
                    pauseScreen();
                    break;
                }

                case 3: {
                    clearScreen();
                    app.showRecurringTransaction();
                    int id;
                    cout << "Nhap ID ban muon xoa: "; cin >> id;
                    app.removeRecurringTransaction(id);
                    pauseScreen();
                    break;
                }

                case 0: 
                    break;
                default: cout << "Lua chon khong hop le!\n";
                }

                if (recChoice != 0) {
                    cout << "\nNhan Enter de tiep tuc...";
                    cin.get();
                }
            }
            break;
        }

        case 0: {//Thoat
            cout << "Dang luu du lieu...\n";
            app.saveData();
            cout << "Tam biet!\n";
            break;
        }

        default:
            cout << "Lua chon khong hop le!\n";
        }

        if (choice != 0) {
            pauseScreen();
        }
    }

    return 0;
}