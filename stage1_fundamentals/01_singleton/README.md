# 01. 单例模式（Singleton Pattern）

## 一句话理解

> **全局只有一个实例，谁都用它，不会冲突**
> 
> 就像公司只有一个财务部，所有部门都要通过它处理财务。

---

## 要解决的问题

**场景**：UART 串口、I2C 总线、Flash 等硬件资源，全局只能初始化一次。

**传统写法的问题**：
```cpp
// ❌ 问题：多个模块分别初始化，配置冲突
void logger_init() { UART_Init(115200); }
void command_init() { UART_Init(9600); }   // 冲突！
void transfer_init() { UART_Init(57600); } // 又变了！
```

---

## 单例模式解法

```cpp
class UARTManager {
private:
    static UARTManager* instance;  // 唯一实例
    UARTManager() {}               // 私有构造
    
public:
    static UARTManager* getInstance() {  // 全局访问点
        if (instance == nullptr) {
            instance = new UARTManager();
        }
        return instance;
    }
};

// ✓ 所有模块获取同一个实例
UARTManager::getInstance()->init(115200);
```

---

## 核心三要素

| 要素 | 代码 | 作用 |
|------|------|------|
| **私有构造** | `UARTManager() private` | 防止外部 `new` |
| **静态实例** | `static instance` | 保存唯一实例 |
| **全局访问点** | `static getInstance()` | 统一入口 |

---

## 编译运行

```bash
cd stage1_fundamentals/01_singleton
mkdir build && cd build
cmake .. && make
./singleton
```

---

## 关键要点

1. **防止拷贝**：删除拷贝构造和赋值运算符
2. **线程安全**：C++11 可用局部静态变量
3. **生命周期**：注意析构顺序问题

---

## 练习

1. 用 C++11 局部静态变量改写 `getInstance()`
2. 实现一个 `FlashManager` 单例
