#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

// ============================================================
// 观察者模式（Observer Pattern）
// ============================================================
// 一句话理解：一对多依赖，一个对象变化，自动通知所有依赖它的对象
// 类比：微信公众号，公众号发文，所有粉丝都会收到推送
// ============================================================

// ------------------------------------------------------------
// 【观察者接口】
// ------------------------------------------------------------
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(float temp, float humidity) = 0;
    virtual string getName() = 0;
};

// ------------------------------------------------------------
// 【被观察者】气象站（温湿度传感器）
// ------------------------------------------------------------
// 维护观察者列表，数据变化时通知所有观察者
class WeatherStation {
private:
    vector<shared_ptr<Observer>> observers;  // 观察者列表
    float temperature, humidity;

public:
    WeatherStation() : temperature(0), humidity(0) {
        cout << "[气象站] 初始化完成\n" << endl;
    }
    
    // 注册观察者（订阅）
    void attach(shared_ptr<Observer> obs) {
        observers.push_back(obs);
        cout << "[气象站] 新观察者：" << obs->getName() << endl;
    }
    
    // 移除观察者（取消订阅）
    void detach(shared_ptr<Observer> obs) {
        for (auto it = observers.begin(); it != observers.end(); ++it) {
            if (*it == obs) {
                observers.erase(it);
                cout << "[气象站] 观察者离开：" << obs->getName() << endl;
                break;
            }
        }
    }
    
    // 通知所有观察者
    void notify() {
        cout << "\n[气象站] 数据变化，通知所有观察者..." << endl;
        for (auto& obs : observers) {
            obs->update(temperature, humidity);
        }
    }
    
    // 测量值变化
    void measurementsChanged(float temp, float hum) {
        cout << "\n[气象站] 测量值更新：温度=" << temp << "°C, 湿度=" << hum << "%" << endl;
        temperature = temp;
        humidity = hum;
        notify();
    }
};

// ============================================================
// 【具体观察者 1】显示屏
// ============================================================
class Display : public Observer {
private:
    string name;
public:
    Display(const string& n = "显示屏") : name(n) {}
    void update(float temp, float humidity) override {
        cout << "  [" << name << "] 显示：" << temp << "°C, " << humidity << "%" << endl;
    }
    string getName() override { return name; }
};

// ============================================================
// 【具体观察者 2】温度报警器
// ============================================================
class Alarm : public Observer {
private:
    float tempMax, tempMin;
    string name;
public:
    Alarm(float maxT = 40.0f, float minT = 0.0f, const string& n = "报警器")
        : tempMax(maxT), tempMin(minT), name(n) {}
    
    void update(float temp, float humidity) override {
        if (temp > tempMax) {
            cout << "  [" << name << "] ⚠️  高温警报！" << temp << "°C" << endl;
        } else if (temp < tempMin) {
            cout << "  [" << name << "] ⚠️  低温警报！" << temp << "°C" << endl;
        } else {
            cout << "  [" << name << "] 温度正常：" << temp << "°C" << endl;
        }
    }
    string getName() override { return name; }
};

// ============================================================
// 【具体观察者 3】数据日志器
// ============================================================
class DataLogger : public Observer {
private:
    int recordCount;
    string name;
public:
    DataLogger(const string& n = "日志器") : recordCount(0), name(n) {}
    
    void update(float temp, float humidity) override {
        recordCount++;
        cout << "  [" << name << "] 记录 #" << recordCount << ": T=" << temp << "°C" << endl;
    }
    string getName() override { return name; }
};

// ============================================================
// 【主函数】
// ============================================================
int main() {
    cout << "=== 观察者模式：温湿度监控系统 ===" << endl;
    cout << "关键：被观察者和观察者解耦，新增观察者无需修改传感器代码\n" << endl;
    
    // 创建被观察者
    auto station = make_shared<WeatherStation>();
    
    // 创建观察者
    auto display = make_shared<Display>("LCD 显示屏");
    auto alarm = make_shared<Alarm>(35.0f, 10.0f, "温度报警器");
    auto logger = make_shared<DataLogger>("数据日志器");
    
    // 注册观察者
    cout << "=== 注册观察者 ===" << endl;
    station->attach(display);
    station->attach(alarm);
    station->attach(logger);
    
    // 正常测量
    cout << "\n=== 正常测量（25°C）===" << endl;
    station->measurementsChanged(25.0f, 60.0f);
    
    // 高温警报
    cout << "\n=== 高温测量（38°C）===" << endl;
    station->measurementsChanged(38.0f, 45.0f);
    
    // 移除报警器
    cout << "\n=== 移除报警器 ===" << endl;
    station->detach(alarm);
    station->measurementsChanged(40.0f, 40.0f);
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
