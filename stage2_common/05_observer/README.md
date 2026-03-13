# 05. 观察者模式（Observer Pattern）

## 一句话理解

> **一对多依赖，一个对象变化，自动通知所有依赖它的对象**
> 
> 就像微信公众号：公众号发文，所有粉丝都会收到推送。

---

## 要解决的问题

**场景**：传感器数据变化时，需要通知多个模块（显示屏、报警器、日志）。

**传统写法的问题**：
```cpp
// ❌ 问题：传感器知道所有模块，紧耦合
void sensor_update(float temp) {
    display_show(temp);
    if (temp > 40) alarm_beep();
    logger_write(temp);
    // 加一个新模块要改这里
}
```

---

## 观察者模式解法

```cpp
// 1. 观察者接口
class Observer {
    virtual void update(float temp, float humidity) = 0;
};

// 2. 被观察者（维护观察者列表）
class WeatherStation {
    vector<Observer*> observers;
    
    void attach(Observer* obs) { observers.push_back(obs); }
    
    void notify() {
        for (auto obs : observers) {
            obs->update(temp, humidity);
        }
    }
};

// 3. 具体观察者
class Display : public Observer {
    void update(float temp, float humidity) override {
        // 更新显示
    }
};

// 4. 注册观察者
station->attach(&display);
station->attach(&alarm);
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **被观察者** | `WeatherStation` | 维护观察者列表，数据变化时通知 |
| **观察者接口** | `Observer` | 定义更新接口 |
| **具体观察者** | `Display` 等 | 实现具体响应逻辑 |

---

## 编译运行

```bash
cd stage2_common/05_observer
mkdir build && cd build
cmake .. && make
./observer
```

---

## 关键要点

1. **解耦**：被观察者不知道具体观察者是谁
2. **动态添加**：运行时可以添加/移除观察者
3. **自动通知**：数据变化自动通知所有观察者

---

## 练习

1. 添加一个 `CloudUploader` 观察者，上传数据到云端
2. 实现观察者优先级，重要的先通知
