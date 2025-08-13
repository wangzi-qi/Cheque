#include <iostream>
#include <string>
using namespace std;

class Cheque
{
private:
    std::string fullname;  // 客户姓名
    long acctnum;          // 账号
    double balance;       // 当前结余

protected:
    const std::string& FullName() const { return fullname; }
    void balance_change(double amt) {balance += amt; }
    long AcctNum() const { return acctnum; }
    

public:
    Cheque(const std::string& a = "Nobody", long an = -1, double bal = 0.0);  // 创建账户
    virtual void Deposit(double amt);  // 存款
    virtual void Withdraw(double amt); // 取款
    virtual void ViewAcct() const;      // 显示账户信息
    double Balance() const { return balance; }
    virtual ~Cheque();
};

Cheque::Cheque(const std::string& a , long an, double bal)
{
    fullname = a;
    acctnum = an;
    balance = bal;
}

void Cheque:: Deposit(double amt)
{
    balance+=amt;
}

 void Cheque::Withdraw(double amt)
{
    if(amt<=balance)
        balance-=amt;
}

void Cheque:: ViewAcct() const
{
    cout<<"fullname:"<<fullname<<endl;
    cout<<"account number: "<<acctnum<<endl;
    cout<<"balance: $"<<balance<<endl;
}

Cheque::~Cheque()
{
    cout<<"Cheque destroyed!"<<endl;
}

class ChequeBase:public Cheque
{
    public:
        ChequeBase(const std::string& a = "Nobody", long an = -1, double bal = 0.0):Cheque(a,an,bal){}
        ~ChequeBase();
}; 
    ChequeBase::~ChequeBase()
    {
        cout<<"ChequeBase destroyed!"<<endl;
    }

class ChequePlus:public Cheque
{
    private:
        double o_limit;
        double o_rate;
        double total_o;
    public:
        ChequePlus(const std::string& a = "Nobody", long an = -1, double bal = 0.0,double c=0.0,double d=0.0,double e=0.0):Cheque(a,an,bal),o_limit(c),o_rate(d),total_o(e){}
        void Deposit(double amt) override;  // 存款
        void Withdraw(double amt) override; // 取款
        void ViewAcct()  const override;      // 显示账户信息
        ~ChequePlus();
};

 void ChequePlus::Deposit(double amt)
 {
   total_o=(amt >= total_o) ? 0.0:(total_o-amt);
   if(amt >= total_o)
   {
        Cheque::Deposit(amt-total_o);
        total_o=0.0;
   }
    else
   {
        total_o-=amt;
   }
 } // 存款

 void ChequePlus::Withdraw(double amt)
 {
    if(Balance()>=0)
    {
        if(amt>Balance())
        {
            int a=(amt-Balance())*(1+o_rate);
            if((total_o+a)<=o_limit)
               {
                total_o=a;
                balance_change(-total_o-Balance());
               }
            else
            throw out_of_range("total overdraft is over limit!");
        }  
        else
        balance_change(-amt);
    }
    else 
    {
        int a=amt*(1+o_rate);
        if((total_o+a)<=o_limit)
        {
         total_o+=a;
         balance_change(-total_o);
        }
        else
         throw out_of_range("total overdraft is over limit!");

    }

 } // 取款

 void ChequePlus::ViewAcct() const{
    cout<<"fullname: "<<FullName()<<endl;
    cout<<"account number: "<<AcctNum()<<endl;  
    cout<<"balance: $"<<Balance()<<endl;
    cout<<"total overdraft: $"<<total_o<<endl;
    cout<<"overdraft limit: $"<<o_limit<<endl;
    cout<<"overdraft number rate: "<<(o_rate*100)<<'%'<<endl;
 }

 ChequePlus::~ChequePlus()
 {
    cout<<"ChequePlus destroyed!"<<endl;
 }

 int main()
 {
    cout<<"branch1"<<endl;
    Cheque *chptr;
    chptr=new ChequeBase("xiao wang",123456789,100);
    chptr ->Deposit(200);
    chptr ->ViewAcct();
    chptr ->Withdraw(35.5);
    chptr ->ViewAcct();
    delete  chptr;

    chptr = new ChequePlus("xiao zhang",586412369,1000,5000,0.1);
    chptr -> Deposit(2000);
    chptr ->ViewAcct();
    chptr ->Withdraw(4000);
    chptr ->ViewAcct();
    chptr -> Deposit(2000);
    chptr ->ViewAcct();
    chptr ->Withdraw(3000);
    chptr ->ViewAcct();
    delete chptr;
    return 0;
 }