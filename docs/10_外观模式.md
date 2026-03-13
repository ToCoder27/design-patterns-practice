# 10. 外观模式（Facade Pattern）

## 一句话理解

> **提供一个统一简化的接口，隐藏子系统的复杂性**
> 
> 就像电视遥控器：按一个"观影模式"按钮，自动关灯、拉窗帘、开音响，不用逐个操作。

---

## 要解决的问题

**场景**：智能家居、复杂子系统、硬件抽象层等。

**传统写法的问题**：
```cpp
// ❌ 问题：客户端代码复杂，需要知道所有子系统
void userComeHome() {
    // 开窗帘
    curtainMotor.enable();
    curtainMotor.open();
    delay(5000);
    curtainMotor.disable();
    
    // 开灯
    gpio_set(LED_PIN, HIGH);
    pwm_setBrightness(80);
    
    // 开空调
    i2c_write(AC_ADDR, POWER_ON);
    i2c_write(AC_ADDR, TEMP_26);
}
```

---

## 外观模式解法

```cpp
// 1. 子系统（复杂，客户端不知道）
class LightSystem { void powerOn(); void setBrightness(int); };
class AirConditioner { void powerOn(); void setTemperature(float); };

// 2. 外观类（简化接口）
class SmartHomeFacade {
private:
    LightSystem light;
    AirConditioner ac;
public:
    void comeHome() {
        light.powerOn();
        light.setBrightness(80);
        ac.powerOn();
        ac.setTemperature(26);
    }
};

// 3. 客户端代码简洁
smartHome.comeHome();
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **外观类** | `SmartHomeFacade` | 提供简化接口，委托给子系统 |
| **子系统** | `LightSystem` 等 | 实现具体功能，不知道外观存在 |

---

## 编译运行

```bash
cd stage3_advanced/10_facade
mkdir build && cd build
cmake .. && make
./facade
```

---

## 关键要点

1. **简化接口**：外观提供比子系统更简单的接口
2. **解耦**：客户端不需要知道子系统细节
3. **场景模式**：可以定义"回家模式"、"睡眠模式"等

---

## 练习

1. 添加一个"阅读模式"：灯光 100%、空调 25°C、窗帘半开
2. 为外观类添加定时功能
