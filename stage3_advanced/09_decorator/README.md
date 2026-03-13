# 09. 装饰器模式（Decorator Pattern）

## 一句话理解

> **给对象动态添加功能，像"套娃"一样层层包装**
> 
> 就像给手机加配件：保护套→钢化膜→指环扣，每层都增加功能。

---

## 要解决的问题

**场景**：传感器数据需要多种处理（滤波、校准、单位转换），可以灵活组合。

**传统写法的问题**：
```cpp
// ❌ 问题：函数太长，难以维护，加新功能要改代码
float process(float raw) {
    float filtered = (raw + prev1 + prev2) / 3;  // 滤波
    float calibrated = (filtered + 0.5) * 1.02;   // 校准
    return calibrated * 9/5 + 32;                 // 单位转换
}
```

---

## 装饰器模式解法

```cpp
// 1. 组件接口
class SensorData {
    virtual float getValue() = 0;
};

// 2. 具体组件
class RawSensor : public SensorData {
    float getValue() override { return raw_value; }
};

// 3. 装饰器基类（持有组件）
class SensorDecorator : public SensorData {
protected:
    shared_ptr<SensorData> wrapped;
public:
    float getValue() override { return wrapped->getValue(); }
};

// 4. 具体装饰器
class FilterDecorator : public SensorDecorator {
    float getValue() override {
        float raw = wrapped->getValue();
        return filter(raw);  // 添加滤波功能
    }
};

// 5. 动态组合
auto sensor = make_shared<RawSensor>(25.0f);
auto filtered = make_shared<FilterDecorator>(sensor);
auto calibrated = make_shared<CalibrateDecorator>(filtered);
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **组件接口** | `SensorData` | 定义核心方法 |
| **具体组件** | `RawSensor` | 被装饰的原始对象 |
| **装饰器基类** | `SensorDecorator` | 持有组件，默认委托 |
| **具体装饰器** | `FilterDecorator` 等 | 添加具体功能 |

---

## 编译运行

```bash
cd stage3_advanced/09_decorator
mkdir build && cd build
cmake .. && make
./decorator
```

---

## 关键要点

1. **动态添加**：运行时决定添加哪些功能
2. **灵活组合**：可以像套娃一样层层包装
3. **开闭原则**：加新功能不用改现有代码

---

## 练习

1. 添加一个 `MedianFilter`（中值滤波）装饰器
2. 实现一个 `LogDecorator`，记录每次数据访问
