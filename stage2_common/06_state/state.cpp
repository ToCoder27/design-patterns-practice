#include <iostream>
#include <string>

using namespace std;

// ============================================================
// 状态机模式（State Pattern）
// ============================================================
// 一句话理解：对象的行为取决于它的状态，状态变了行为也变
// 类比：自动售货机：待机时只能投币，投币后可以选商品
// ============================================================

class VendingMachine;

// ------------------------------------------------------------
// 【状态接口】
// ------------------------------------------------------------Error while generating Documentation.
class VendingState {
protected:
    VendingMachine* machine;
    
public:
    virtual ~VendingState() = default;
    void setMachine(VendingMachine* m) { machine = m; }
    virtual void insertCoin(int value) = 0;
    virtual void selectProduct(int id) = 0;
    virtual void dispense() = 0;
    virtual const char* getName() = 0;
};

// ------------------------------------------------------------
// 【上下文】自动售货机
// ------------------------------------------------------------
class VendingMachine {
private:
    VendingState* state;
    int balance;
    int selectedProduct;

public:
    VendingMachine() : balance(0), selectedProduct(-1), state(nullptr) {
        cout << "[售货机] 初始化，进入待机状态\n" << endl;
    }
    
    ~VendingMachine() { delete state; }
    
    void setState(VendingState* newState) {
        delete state;
        state = newState;
        if (state) state->setMachine(this);
        cout << "[售货机] 状态 -> " << state->getName() << endl;
    }
    
    void insertCoin(int value) { if (state) state->insertCoin(value); }
    void selectProduct(int id) { if (state) state->selectProduct(id); }
    void dispense() { if (state) state->dispense(); }
    
    void addBalance(int v) { balance += v; }
    void setBalance(int v) { balance = v; }
    int getBalance() const { return balance; }
    void setSelectedProduct(int id) { selectedProduct = id; }
    int getSelectedProduct() const { return selectedProduct; }
    
    // 工厂方法：创建各个状态
    VendingState* createIdleState();
    VendingState* createCoinInsertedState();
    VendingState* createDispensingState();
};

// ============================================================
// 【具体状态 1】待机状态
// ============================================================
class IdleState : public VendingState {
public:
    void insertCoin(int value) override {
        cout << "[投币] 投入：" << value << "元" << endl;
        machine->addBalance(value);
        cout << "[余额] " << machine->getBalance() << "元" << endl;
        machine->setState(machine->createCoinInsertedState());
    }
    void selectProduct(int) override { cout << "[待机] 请先投币" << endl; }
    void dispense() override { cout << "[待机] 无效操作" << endl; }
    const char* getName() override { return "待机状态"; }
};

// ============================================================
// 【具体状态 2】已投币状态
// ============================================================
class CoinInsertedState : public VendingState {
public:
    void insertCoin(int value) override {
        cout << "[投币] 投入：" << value << "元" << endl;
        machine->addBalance(value);
        cout << "[余额] " << machine->getBalance() << "元" << endl;
    }
    
    void selectProduct(int id) override {
        cout << "[选择] 商品 ID: " << id << endl;
        if (machine->getBalance() >= 5) {
            cout << "[验证] 余额充足" << endl;
            machine->setBalance(machine->getBalance() - 5);
            machine->setSelectedProduct(id);
            machine->setState(machine->createDispensingState());
        } else {
            cout << "[验证] 余额不足！" << endl;
        }
    }
    void dispense() override { cout << "[已投币] 请选择商品" << endl; }
    const char* getName() override { return "已投币状态"; }
};

// ============================================================
// 【具体状态 3】出货中状态
// ============================================================
class DispensingState : public VendingState {
public:
    void insertCoin(int) override { cout << "[出货中] 请勿投币" << endl; }
    void selectProduct(int) override { cout << "[出货中] 请稍候" << endl; }
    
    void dispense() override {
        cout << "[出货] 商品 ID " << machine->getSelectedProduct() << " 已出货" << endl;
        if (machine->getBalance() > 0) {
            cout << "[找零] 找零：" << machine->getBalance() << "元" << endl;
        }
        machine->setBalance(0);
        machine->setState(machine->createIdleState());
    }
    const char* getName() override { return "出货中状态"; }
};

// ============================================================
// 【工厂方法实现】
// ============================================================
VendingState* VendingMachine::createIdleState() {
    return new IdleState();
}
VendingState* VendingMachine::createCoinInsertedState() {
    return new CoinInsertedState();
}
VendingState* VendingMachine::createDispensingState() {
    return new DispensingState();
}

// ============================================================
// 【主函数】
// ============================================================
int main() {
    cout << "=== 状态机模式：自动售货机 ===" << endl;
    cout << "关键：每个状态独立封装，状态转换清晰\n" << endl;
    
    VendingMachine machine;
    machine.setState(machine.createIdleState());
    
    cout << "=== 场景 1: 正常购买 ===" << endl;
    machine.insertCoin(5);
    machine.insertCoin(5);
    machine.selectProduct(1);
    machine.dispense();
    
    cout << "\n=== 场景 2: 余额不足 ===" << endl;
    machine.insertCoin(2);
    machine.selectProduct(1);
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
