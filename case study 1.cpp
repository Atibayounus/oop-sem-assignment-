#include <iostream>
#include <fstream>
using namespace std;

// ================= BASE CLASS =================
class Transaction {
protected:
    string name;
    string phone;
    double amount;

public:

    // Default Constructorr
    Transaction() {
        name = "Unknown";
        phone = "0300-0000000";
        amount = 100;
    }

    // Parameterized Constructorr
    Transaction(string n, string p, double a) {
        setName(n);
        setPhone(p);
        setAmount(a);
    }

    // Overloaded Constructor
    Transaction(string n) {
        name = n;
        phone = "0300-0000000";
        amount = 100;
    }

    // Destructor
    virtual ~Transaction() {
        ofstream file("audit.txt", ios::app);
        file << "Transaction deleted for " << name << endl;
        file.close();
    }

    // Setters
    void setName(string n) {
        name = n;
    }

    void setAmount(double a) {
        if (a <= 0)
            amount = 100;
        else
            amount = a;
    }

    void setPhone(string p) {
        if (p.length() == 12 && p[0] == '0' && p[1] == '3' && p[4] == '-')
            phone = p;
        else
            phone = "0300-0000000";
    }

    // Getters
    string getName() {
        return name;
    }

    double getAmount() {
        return amount;
    }

    virtual double commission() = 0;
    virtual string type() = 0;
};

// ================= MOBILE LOAD =================
class MobileLoad : public Transaction {
private:
    string network;

public:
    MobileLoad() : Transaction() {
        network = "Jazz";
    }

    MobileLoad(string n, string p, double a, string net)
        : Transaction(n, p, a) {
        network = net;
    }

    MobileLoad(string n) : Transaction(n) {
        network = "Jazz";
    }

    double commission() {
        double c = amount * 0.025;

        if (c < 2)
            return 2;

        return (int)c;
    }

    string type() {
        return "Mobile Load";
    }
};

// ================= EASYPAISA =================
class EasyPaisa : public Transaction {
private:
    string direction;

public:
    EasyPaisa() : Transaction() {
        direction = "Cash In";
    }

    EasyPaisa(string n, string p, double a, string d)
        : Transaction(n, p, a) {
        direction = d;
    }

    EasyPaisa(string n) : Transaction(n) {
        direction = "Cash Out";
    }

    double commission() {
        if (amount <= 1000)
            return 10;
        else if (amount <= 5000)
            return 25;
        else
            return 25 + (amount - 5000) * 0.003;
    }

    string type() {
        return "EasyPaisa";
    }
};

// ================= SIM REPLACEMENT =================
class SIMReplacement : public Transaction {
public:

    SIMReplacement() : Transaction() {}

    SIMReplacement(string n, string p)
        : Transaction(n, p, 250) {}

    SIMReplacement(string n) : Transaction(n) {
        amount = 250;
    }

    double commission() {
        return 150;
    }

    string type() {
        return "SIM Replacement";
    }
};

// ================= DAILY LEDGER =================
class DailyLedger {
private:
    Transaction* arr[50];
    int count;

public:

    DailyLedger() {
        count = 0;
    }

    void add(Transaction* t) {
        arr[count] = t;
        count++;
    }

    void show() {
        double total = 0;

        cout << "\n===== DAILY RECORD =====\n";

        for (int i = 0; i < count; i++) {
            cout << i + 1 << ". "
                 << arr[i]->type()
                 << "  Amount: " << arr[i]->getAmount()
                 << "  Commission: " << arr[i]->commission()
                 << endl;

            total += arr[i]->commission();
        }

        cout << "\nTotal Commission = " << total << endl;
    }

    void closeDay() {
        ofstream file("receipt.txt");

        double total = 0;
        double cash = 0;

        file << "Daily Receipt\n";

        for (int i = 0; i < count; i++) {
            file << arr[i]->type()
                 << " Amount: "
                 << arr[i]->getAmount()
                 << endl;

            total += arr[i]->commission();
            cash += arr[i]->getAmount();
        }

        file << "\nCash Drawer = " << cash << endl;
        file << "Total Commission = " << total << endl;

        file.close();

        cout << "\nDay Closed. Receipt Saved.\n";
    }

    ~DailyLedger() {
        for (int i = 0; i < count; i++) {
            delete arr[i];
        }
    }
};

// ================= MAIN =================
int main() {

    DailyLedger d;

    // Three constructorss
    Transaction* t1 = new MobileLoad();
    Transaction* t2 = new EasyPaisa("Ali", "0301-1234567", 6000, "Cash In");
    Transaction* t3 = new SIMReplacement("Bilal");

    d.add(t1);
    d.add(t2);
    d.add(t3);

    d.show();

    d.closeDay();

    return 0;
}


