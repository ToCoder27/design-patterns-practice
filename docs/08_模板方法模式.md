# 08. 模板方法模式（Template Method Pattern）

## 一句话理解

> **定义流程骨架，具体步骤由子类实现**
> 
> 就像做菜食谱：步骤是固定的（洗→切→炒），但食材和调料可以不同。

---

## 要解决的问题

**场景**：传感器数据采集、通信协议、设备初始化等有固定流程的场景。

**传统写法的问题**：
```cpp
// ❌ 问题：代码重复，流程分散在各处
void dht11_read() {
    init(); read_raw(); convert(); output();
}
void sht30_read() {
    init(); warmup(); read_raw(); convert(); filter(); output();
}
```

---

## 模板方法模式解法

```cpp
// 1. 抽象基类定义流程骨架
class SensorBase {
    float read() {  // 模板方法
        initialize();
        warmUp();        // 钩子
        float raw = readRawData();
        float val = convertRawToValue(raw);
        val = applyFilter(val);  // 钩子
        output(val);
    }
    
    virtual void initialize() = 0;      // 子类实现
    virtual float readRawData() = 0;    // 子类实现
    virtual float convertRawToValue(float) = 0;  // 子类实现
    virtual void warmUp() {}            // 钩子方法
    virtual float applyFilter(float v) { return v; }  // 钩子方法
};

// 2. 子类实现具体步骤
class DHT11Sensor : public SensorBase {
    void initialize() override { /* ... */ }
    float readRawData() override { /* ... */ }
    float convertRawToValue(float raw) override { /* ... */ }
};
```

---

## 核心要点

| 方法类型 | 方法 | 说明 |
|----------|------|------|
| **模板方法** | `read()` | 定义流程骨架 |
| **抽象方法** | `initialize()` 等 | 子类必须实现 |
| **钩子方法** | `warmUp()` 等 | 可选实现，提供扩展点 |

---

## 编译运行

```bash
cd stage3_advanced/08_template
mkdir build && cd build
cmake .. && make
./template
```

---

## 关键要点

1. **流程固定**：模板方法定义流程，子类不能修改
2. **钩子方法**：提供扩展点，子类可选实现
3. **代码复用**：公共流程在基类实现

---

## 练习

1. 添加一个 BMP280 传感器（需要预热，有滤波）
2. 为模板添加一个 `selfTest()` 钩子方法
