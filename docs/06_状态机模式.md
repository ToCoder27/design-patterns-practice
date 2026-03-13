# 06. 状态机模式（State Pattern）

## 一句话理解

> **对象的行为取决于它的状态，状态变了行为也变**
> 
> 就像自动售货机：待机时只能投币，投币后可以选商品，出货时不能操作。

---

## 要解决的问题

**场景**：自动售货机、通信协议、电机控制等有状态转换的系统。

**传统写法的问题**：
```cpp
// ❌ 问题：满屏 if-else，状态转换逻辑分散
void insert_coin(int value) {
    if (state == STATE_IDLE) {
        balance += value;
        state = STATE_COIN_INSERTED;
    } else if (state == STATE_COIN_INSERTED) {
        balance += value;
    } else if (state == STATE_DISPENSING) {
        printf("出货中，无法投币\n");
    }
}
```

---

## 状态机模式解法

```cpp
// 1. 状态接口
class VendingState {
    virtual void insertCoin() = 0;
    virtual void selectProduct() = 0;
};

// 2. 具体状态
class IdleState : public VendingState {
    void insertCoin() override {
        // 投币逻辑
        // 转换到 CoinInsertedState
    }
};

class CoinInsertedState : public VendingState {
    void selectProduct() override {
        // 选择商品逻辑
        // 转换到 DispensingState
    }
};

// 3. 上下文持有状态
class VendingMachine {
    shared_ptr<VendingState> state;
    void setState(shared_ptr<VendingState> s) { state = s; }
};
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **上下文** | `VendingMachine` | 持有当前状态，委托操作 |
| **状态接口** | `VendingState` | 定义状态行为接口 |
| **具体状态** | `IdleState` 等 | 实现具体状态行为 |

---

## 编译运行

```bash
cd stage2_common/06_state
mkdir build && cd build
cmake .. && make
./state
```

---

## 关键要点

1. **状态封装**：每个状态的行为封装在独立类中
2. **状态转换**：状态自己决定何时转换到下一个状态
3. **消除 if-else**：用多态代替条件判断

---

## 练习

1. 添加一个 `FaultState`（故障状态）
2. 实现 WiFi 连接状态机（断开→扫描→认证→连接）
