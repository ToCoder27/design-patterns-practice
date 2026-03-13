/**
 * @file strategy.cpp
 * @brief 策略模式示例 - 传感器驱动切换
 * 
 * 一句话理解：一个接口，多种实现，运行时切换
 * 类比：手机铃声模式（静音/振动/响铃），同一个接口，不同实现
 */

#include <iostream>
#include <memory>
#include <string>

using namespace std;

/**
 * @interface SensorStrategy
 * @brief 传感器策略接口
 * 
 * 定义统一的接口，所有具体传感器都必须实现
 */
class SensorStrategy {
public:
    virtual ~SensorStrategy() = default;
    
    /** @brief 初始化传感器 */
    virtual bool init() = 0;
    
    /** @brief 读取温度 */
    virtual float read_temperature() = 0;
    
    /** @brief 获取传感器名称 */
    virtual const char* get_name() = 0;
};

class mydevStrategy : public SensorStrategy {
    public:
        bool init() override {
            cout << "[mydev] 初始化..." << endl;
            return true;
        }

        float read_temperature() override {
            cout << "[mydev] 读取温度..." << endl;
            return 24.8f;
        }

        const char* get_name() override { return "mydev (自定义)"; }
};

/**
 * @class DHT11Strategy
 * @brief DHT11 传感器策略（低成本）
 */
class DHT11Strategy : public SensorStrategy {
public:
    bool init() override {
        cout << "[DHT11] 初始化单总线..." << endl;
        return true;
    }
    float read_temperature() override {
        cout << "[DHT11] 读取温度..." << endl;
        return 25.0f;
    }
    const char* get_name() override { return "DHT11 (低成本)"; }
};

/**
 * @class DHT22Strategy
 * @brief DHT22 传感器策略（高精度）
 */
class DHT22Strategy : public SensorStrategy {
public:
    bool init() override {
        cout << "[DHT22] 初始化单总线（高精度）..." << endl;
        return true;
    }
    float read_temperature() override {
        cout << "[DHT22] 读取温度（精度±0.5°C）..." << endl;
        return 25.5f;
    }
    const char* get_name() override { return "DHT22 (高精度)"; }
};

/**
 * @class SHT30Strategy
 * @brief SHT30 传感器策略（I2C，工业级）
 */
class SHT30Strategy : public SensorStrategy {
public:
    bool init() override {
        cout << "[SHT30] 初始化 I2C 总线..." << endl;
        return true;
    }
    float read_temperature() override {
        cout << "[SHT30] I2C 读取温度..." << endl;
        return 25.3f;
    }
    const char* get_name() override { return "SHT30 (工业级)"; }
};

/**
 * @class EnvironmentMonitor
 * @brief 环境监控器（上下文）
 * 
 * 持有并使用策略，客户端通过它来操作
 */
class EnvironmentMonitor {
private:
    /** @brief 当前策略 */
    shared_ptr<SensorStrategy> strategy;

public:
    /**
     * @brief 设置/切换策略
     * @param new_strategy 新的策略对象
     */
    void set_strategy(shared_ptr<SensorStrategy> new_strategy) {
        strategy = new_strategy;
        cout << ">> 切换到：" << strategy->get_name() << endl;
    }
    
    /**
     * @brief 读取所有数据
     */
    void read_all() {
        if (!strategy) {
            cout << "错误：未设置策略！" << endl;
            return;
        }
        strategy->init();
        float temp = strategy->read_temperature();
        cout << "【读数】温度：" << temp << "°C\n" << endl;
    }
};

/**
 * @brief 主函数 - 演示运行时切换策略
 * @return int 退出码
 */
int main() {
    cout << "=== 策略模式演示：传感器驱动切换 ===" << endl;
    cout << "关键：同一个接口，运行时切换不同实现\n" << endl;
    
    EnvironmentMonitor monitor;
    
    // 创建各种策略
    auto dht11 = make_shared<DHT11Strategy>();
    auto dht22 = make_shared<DHT22Strategy>();
    auto sht30 = make_shared<SHT30Strategy>();
    auto mydev = make_shared<mydevStrategy>();

    // 场景 1：使用 DHT11
    cout << "--- 场景 1: 家用模式 ---" << endl;
    monitor.set_strategy(dht11);
    monitor.read_all();
    
    // 场景 2：切换到 DHT22
    cout << "--- 场景 2: 实验室模式 ---" << endl;
    monitor.set_strategy(dht22);
    monitor.read_all();
    
    // 场景 3：切换到 SHT30
    cout << "--- 场景 3: 工业模式 ---" << endl;
    monitor.set_strategy(sht30);
    monitor.read_all();
    
    // 场景 4: 切换到自定义 mydev
    cout << "--- 场景 4: 自定义设备 ---" << endl;
    monitor.set_strategy(mydev);
    monitor.read_all();

    cout << "=== 演示结束 ===" << endl;
    return 0;
}
