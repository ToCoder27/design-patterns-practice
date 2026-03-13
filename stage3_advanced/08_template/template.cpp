#include <iostream>
#include <string>

using namespace std;

// ============================================================
// 模板方法模式（Template Method Pattern）
// ============================================================
// 一句话理解：定义流程骨架，具体步骤由子类实现
// 类比：做菜食谱：步骤固定（洗→切→炒），但食材可以不同
// ============================================================

// ------------------------------------------------------------
// 【抽象类】传感器基类（模板方法）
// ------------------------------------------------------------
class SensorBase {
protected:
    string name;
    bool initialized = false;
    
    // 钩子方法：可选步骤，子类可以重写
    virtual void warmUp() { /* 默认不需要预热 */ }
    virtual float applyFilter(float value) { return value; }

public:
    SensorBase(const string& n) : name(n) {}
    virtual ~SensorBase() = default;
    
    // ========== 模板方法（定义流程骨架）==========
    // final 防止子类修改流程
    float read() {
        cout << "\n[" << name << "] 开始数据采集" << endl;
        
        // 步骤 1：初始化
        if (!initialized) {
            initialize();
            initialized = true;
        }
        
        // 步骤 2：预热（可选）
        warmUp();
        
        // 步骤 3：读取原始数据（子类实现）
        float raw = readRawData();
        cout << "[" << name << "] 原始数据：" << raw << endl;
        
        // 步骤 4：数据转换（子类实现）
        float value = convertRawToValue(raw);
        cout << "[" << name << "] 转换后：" << value << endl;
        
        // 步骤 5：滤波（可选）
        value = applyFilter(value);
        
        // 步骤 6：输出
        cout << "[" << name << "] 最终：" << value << endl;
        return value;
    }
    
    // ========== 抽象方法（必须由子类实现）==========
    virtual void initialize() = 0;
    virtual float readRawData() = 0;
    virtual float convertRawToValue(float raw) = 0;
};

// ============================================================
// 【具体类】DHT11 传感器
// ============================================================
class DHT11Sensor : public SensorBase {
public:
    DHT11Sensor() : SensorBase("DHT11") {}
    
    void initialize() override {
        cout << "[DHT11] 初始化单总线..." << endl;
    }
    
    float readRawData() override {
        cout << "[DHT11] 读取 40bit 数据..." << endl;
        return 250.0f;
    }
    
    float convertRawToValue(float raw) override {
        return raw / 10.0f;  // DHT11 转换公式
    }
};

// ============================================================
// 【具体类】SHT30 传感器（需要预热和滤波）
// ============================================================
class SHT30Sensor : public SensorBase {
private:
    float filterBuf[3] = {0};
    int filterIdx = 0;
    
public:
    SHT30Sensor() : SensorBase("SHT30") {}
    
    void initialize() override {
        cout << "[SHT30] 初始化 I2C..." << endl;
    }
    
    // 重写钩子方法：SHT30 需要预热
    void warmUp() override {
        cout << "[SHT30] 预热 100ms..." << endl;
    }
    
    float readRawData() override {
        cout << "[SHT30] I2C 读取数据..." << endl;
        return 8294.0f;
    }
    
    float convertRawToValue(float raw) override {
        return -45.0f + 175.0f * raw / 65535.0f;
    }
    
    // 重写钩子方法：SHT30 有滤波
    float applyFilter(float value) override {
        filterBuf[filterIdx % 3] = value;
        filterIdx++;
        return (filterBuf[0] + filterBuf[1] + filterBuf[2]) / 3.0f;
    }
};

// ============================================================
// 【主函数】
// ============================================================
int main() {
    cout << "=== 模板方法模式：传感器数据采集 ===" << endl;
    cout << "关键：模板方法定义流程骨架，子类实现具体步骤\n" << endl;
    
    DHT11Sensor dht11;
    SHT30Sensor sht30;
    
    cout << "=== DHT11 采集 ===" << endl;
    dht11.read();
    
    cout << "\n=== SHT30 采集（需要预热和滤波）===" << endl;
    sht30.read();
    
    cout << "\n=== SHT30 再次采集（跳过初始化）===" << endl;
    sht30.read();
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
