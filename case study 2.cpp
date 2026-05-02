#include <iostream>
using namespace std;


//  making ABSTRACT CLASS / INTERFACE PAYABLE
class Payable
{
protected:
    string name;

public:
    Payable(string n)
    {
        name = n;
    }

    // now Pure virtual functions (Abstraction)
    virtual double calculateGrossPay() = 0;
    virtual double calculateTax() = 0;
    virtual string getCategory() = 0;

    string getName()
    {
        return name;
    }

    // Common function for all employees
    double calculateNetPay()
    {
        return calculateGrossPay() - calculateTax();
    }

    
    // METHOD OVERLOADING
    
    void generatePayslip(string month)
    {
        cout << "Payslip of " << name << " for " << month << endl;
    }

    void generatePayslip(string startDate, string endDate)
    {
        cout << "Payslip of " << name << " from "
             << startDate << " to " << endDate << endl;
    }

    virtual ~Payable() {}
};


// SHARED PARENT CLASS
class SlabTaxStaff : public Payable
{
public:
    SlabTaxStaff(string n) : Payable(n) {}

    // METHOD OVERRIDING
    double calculateTax()
    {
        double gross = calculateGrossPay();

        if (gross <= 50000)
            return 0;
        else if (gross <= 100000)
            return gross * 0.05;
        else
            return gross * 0.125;
    }
};


//  now i am doing requirements of CONSULTANT DOCTOR
// Rs.3500 per OPD consultation
// +40% of surgery fee
// Flat 15% tax

class ConsultantDoctor : public Payable
{
    int opd;
    double surgeryFee;

public:
    ConsultantDoctor(string n, int o, double s) : Payable(n)
    {
        opd = o;
        surgeryFee = s;
    }

    double calculateGrossPay()
    {
        return (opd * 3500) + (surgeryFee * 0.40);
    }

    double calculateTax()
    {
        return calculateGrossPay() * 0.15;
    }

    string getCategory()
    {
        return "Consultant Doctor";
    }
};


// Now Requirements of SALARIED DOCTOR
// Fixed salary + Rs.2000 per emergency night
// Slab tax

class SalariedDoctor : public SlabTaxStaff
{
    double salary;
    int nights;

public:
    SalariedDoctor(string n, double s, int ni)
        : SlabTaxStaff(n)
    {
        salary = s;
        nights = ni;
    }

    double calculateGrossPay()
    {
        return salary + (nights * 2000);
    }

    string getCategory()
    {
        return "Salaried Doctor";
    }
};


// Now requirements of SENIOR NURSE
// Fixed salary + Rs.800 night shift
// +1500 allowance if experience >5 years
// Slab tax

class SeniorNurse : public SlabTaxStaff
{
    double salary;
    int shifts;
    int years;

public:
    SeniorNurse(string n, double s, int sh, int y)
        : SlabTaxStaff(n)
    {
        salary = s;
        shifts = sh;
        years = y;
    }

    double calculateGrossPay()
    {
        double gross = salary + (shifts * 800);

        if (years > 5)
            gross = gross + 1500;

        return gross;
    }

    string getCategory()
    {
        return "Senior Nurse";
    }
};


// Now,JUNIOR NURSE
// Fixed salary only
// Flat 2.5% tax

class JuniorNurse : public Payable
{
    double salary;

public:
    JuniorNurse(string n, double s) : Payable(n)
    {
        salary = s;
    }

    double calculateGrossPay()
    {
        return salary;
    }

    double calculateTax()
    {
        return calculateGrossPay() * 0.025;
    }

    string getCategory()
    {
        return "Junior Nurse";
    }
};


// now, WARD BOY / HOUSEKEEPING
// Daily wage × days worked
// No tax
// One week bonus in Eid month

class WardBoy : public Payable
{
    double wage;
    int days;
    bool eidMonth;

public:
    WardBoy(string n, double w, int d, bool e)
        : Payable(n)
    {
        wage = w;
        days = d;
        eidMonth = e;
    }

    double calculateGrossPay()
    {
        double gross = wage * days;

        if (eidMonth == true)
            gross = gross + (wage * 7);

        return gross;
    }

    double calculateTax()
    {
        return 0;
    }

    string getCategory()
    {
        return "Ward Boy";
    }
};


// ADMINISTRATIVE STAFF
// Fixed salary
// Slab tax

class AdministrativeStaff : public SlabTaxStaff
{
    double salary;

public:
    AdministrativeStaff(string n, double s)
        : SlabTaxStaff(n)
    {
        salary = s;
    }

    double calculateGrossPay()
    {
        return salary;
    }

    string getCategory()
    {
        return "Administrative Staff";
    }
};



// now doing Runtime Polymorphism using Payable* array
// At least 10 employees

int main()
{
    Payable *staff[10];

    staff[0] = new ConsultantDoctor("Ali", 20, 300000);
    staff[1] = new ConsultantDoctor("Usman", 15, 200000);

    staff[2] = new SalariedDoctor("Hamza", 90000, 5);
    staff[3] = new SalariedDoctor("Bilal", 70000, 3);

    staff[4] = new SeniorNurse("Ayesha", 60000, 8, 7);
    staff[5] = new SeniorNurse("Maria", 55000, 5, 4);

    staff[6] = new JuniorNurse("Sara", 40000);
    staff[7] = new JuniorNurse("Hina", 42000);

    staff[8] = new WardBoy("Kashif", 1500, 26, true);

    staff[9] = new AdministrativeStaff("Noman", 65000);

    
    // Runtime Polymorphism Loop
    // One loop calculates all payroll
    
    for (int i = 0; i < 10; i++)
    {
        cout << "Name: " << staff[i]->getName() << endl;
        cout << "Category: " << staff[i]->getCategory() << endl;
        cout << "Gross Pay: " << staff[i]->calculateGrossPay() << endl;
        cout << "Tax: " << staff[i]->calculateTax() << endl;
        cout << "Net Pay: " << staff[i]->calculateNetPay() << endl;
        cout << "----------------------" << endl;
    }

    
    // Method Overloading Demo

    staff[0]->generatePayslip("April");
    staff[1]->generatePayslip("1-April", "30-April");

    // Memory Release
    for (int i = 0; i < 10; i++)
    {
        delete staff[i];
    }

    return 0;
}
