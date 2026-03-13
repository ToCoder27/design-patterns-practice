#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ============================================================
// 装饰器模式（Decorator Pattern）
// ============================================================
// 一句话理解：给对象动态添加功能，像"套娃"一样层层包装
// 类比：给手机加配件：保护套→钢化膜→指环扣，每层都增加功能
// ============================================================

// ------------------------------------------------------------
// 【组件接口】传感器数据接口
// ------------------------------------------------------------
class SensorData {
public:
    virtual ~SensorData() = default;
    virtual float getValue() = 0;
    virtual string getDescription() = 0;
};

// ------------------------------------------------------------
// 【具体组件】原始温度传感器
// ------------------------------------------------------------
class RawTemperatureSensor : public SensorData {
private:
    float raw_value;
public:
    RawTemperatureSensor(float value) : raw_value(value) {}
    
    float getValue() override { return raw_value; }
    string getDescription() override { return "原始传感器"; }
};

// ------------------------------------------------------------
// 【装饰器基类】持有并委托给组件
// ------------------------------------------------------------
class SensorDecorator : public SensorData {
protected:
    shared_ptr<SensorData> wrapped;  // 被装饰的对象
public:
    SensorDecorator(shared_ptr<SensorData> data) : wrapped(data) {}
    
    float getValue() override { return wrapped->getValue(); }
    string getDescription() override { return wrapped->getDescription(); }
};

// ------------------------------------------------------------
// 【具体装饰器 1】滑动平均滤波
// ------------------------------------------------------------
class MovingAverageFilter : public SensorDecorator {
private:
    float prev[3] = {0};
    int count = 0;
public:
    MovingAverageFilter(shared_ptr<SensorData> data) 
        : SensorDecorator(data) {}
    
    float getValue() override {
        float current = wrapped->getValue();
        // 滑动平均
        prev[0] = prev[1]; prev[1] = prev[2]; prev[2] = current;
        count = min(count + 1, 3);
        float sum = 0;
        for (int i = 0; i < count; i++) sum += prev[i];
        return sum / count;
    }
    
    string getDescription() override {
        return wrapped->getDescription() + " + [滑动平均滤波]";
    }
};

// ------------------------------------------------------------
// 【具体装饰器 2】温度校准
// ------------------------------------------------------------
class CalibrationDecorator : public SensorDecorator {
private:
    float offset, scale;
public:
    CalibrationDecorator(shared_ptr<SensorData> data, float off = 0.0f, float scl = 1.0f)
        : SensorDecorator(data), offset(off), scale(scl) {}
    
    float getValue() override {
        float raw = wrapped->getValue();
        return (raw + offset) * scale;  // 校准公式
    }
    
    string getDescription() override {
        return wrapped->getDescription() + " + [校准]";
    }
};

// ------------------------------------------------------------
// 【具体装饰器 3】单位转换（°C 转°F）
// ------------------------------------------------------------
class UnitConverterDecorator : public SensorDecorator {
public:
    UnitConverterDecorator(shared_ptr<SensorData> data)
        : SensorDecorator(data) {}
    
    float getValue() override {
        float celsius = wrapped->getValue();
        return celsius * 9.0f / 5.0f + 32.0f;  // C 转 F
    }
    
    string getDescription() override {
        return wrapped->getDescription() + " + [°C→°F]";
    }
};

// ============================================================
// 【主函数】
// ============================================================
void printSensorInfo(shared_ptr<SensorData> sensor) {
    cout << "描述：" << sensor->getDescription() << endl;
    cout << "数值：" << sensor->getValue() << endl << endl;
}

int main() {
    cout << "=== 装饰器模式：传感器数据过滤器 ===" << endl;
    cout << "关键：装饰器可以像'套娃'一样层层包装，灵活组合\n" << endl;
    
    // 创建核心对象
    auto raw = make_shared<RawTemperatureSensor>(25.0f);
    
    // 场景 1：原始数据
    cout << "=== 场景 1: 原始数据 ===" << endl;
    printSensorInfo(raw);
    
    // 场景 2：只加滤波
    cout << "=== 场景 2: + 滑动平均滤波 ===" << endl;
    auto filtered = make_shared<MovingAverageFilter>(raw);
    printSensorInfo(filtered);
    
    // 场景 3：滤波 + 校准
    cout << "=== 场景 3: + 滤波 + 校准 ===" << endl;
    auto calibrated = make_shared<CalibrationDecorator>(filtered, -0.5f, 1.02f);
    printSensorInfo(calibrated);
    
    // 场景 4：滤波 + 校准 + 单位转换
    cout << "=== 场景 4: + 滤波 + 校准 + 单位转换 ===" << endl;
    auto converted = make_shared<UnitConverterDecorator>(calibrated);
    printSensorInfo(converted);
    
    cout << "=== 演示结束 ===" << endl;
    return 0;
}
