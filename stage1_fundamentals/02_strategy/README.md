# 02. 策略模式（Strategy Pattern）

## 一句话理解

> **一个接口，多种实现，运行时切换**
> 
> 就像手机铃声模式：静音、振动、响铃，同一个"来电提醒"接口，不同的实现策略。

---

## 要解决的问题

**场景**：系统要支持多种传感器（DHT11/DHT22/SHT30），运行时可能切换。

**传统写法的问题**：
```cpp
// ❌ 问题：满屏 if-else，加新传感器要改代码
float read_temp(int type) {
    if (type == DHT11) return dht11_read();
    else if (type == DHT22) return dht22_read();
    else if (type == SHT30) return sht30_read();
}
```

---

## 策略模式解法

```cpp
// 1. 定义策略接口
class SensorStrategy {
    virtual float read_temperature() = 0;
};

// 2. 实现具体策略
class DHT11Strategy : public SensorStrategy {
    float read_temperature() override { /* ... */ }
};

// 3. 上下文持有策略
class EnvironmentMonitor {
    shared_ptr<SensorStrategy> strategy;
    void set_strategy(shared_ptr<SensorStrategy> s) { strategy = s; }
};

// 4. 运行时切换
monitor.set_strategy(dht11);  // 或 dht22、sht30
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **策略接口** | `SensorStrategy` | 定义统一接口 |
| **具体策略** | `DHT11Strategy` 等 | 实现不同算法 |
| **上下文** | `EnvironmentMonitor` | 持有并切换策略 |

---

## 编译运行

```bash
cd stage1_fundamentals/02_strategy
mkdir build && cd build
cmake .. && make
./strategy
```

---

## 关键要点

1. **运行时切换**：策略可以在运行时动态更换
2. **开闭原则**：加新策略不用改现有代码
3. **依赖倒置**：上下文依赖抽象策略接口

---

## 练习

1. 添加一个 `BME280Strategy`（支持温度、湿度、气压）
2. 实现根据硬件 ID 自动选择策略
