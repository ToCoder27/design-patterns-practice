#include <iostream>
#include <memory>
#include <string>

using namespace std;

// base class
class base {
    public:
        virtual void func() = 0;
};

class mydevStrategy : public base {
    public:
        void func() override {
            cout << "mydev func" << endl;
        }
};

class mydevStrategy2 : public base {
    public:
        void func() override {
            cout << "mydev func2" << endl;
        }
};

// 使用策略的类
class demo {
    private:
        shared_ptr<base> strategy;
    public:
        void set_strategy(shared_ptr<base> new_strategy) {
            strategy = new_strategy;
        }

        void execute() {
            if (strategy) {
                strategy->func();
            } else {
                cout << "No strategy set!" << endl;
            }
        }
};


int main() {
    demo d;
    auto s1 = make_shared<mydevStrategy>();
    auto s2 = make_shared<mydevStrategy2>();

    d.set_strategy(s1);
    d.execute();

    d.set_strategy(s2);
    d.execute();

    return 0;
}