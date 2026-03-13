#include <iostream>
#include <string>

using namespace std;

// ============================================================
// 外观模式（Facade Pattern）
// ============================================================
// 一句话理解：提供一个统一简化的接口，隐藏子系统的复杂性
// 类比：电视遥控器：按"观影模式"，自动关灯、拉窗帘、开音响
// ============================================================

// ============================================================
// 【子系统 1】灯光系统
// ============================================================
class LightSystem {
public:
    void powerOn() { cout << "  [灯光] 打开电源" << endl; }
    void powerOff() { cout << "  [灯光] 关闭电源" << endl; }
    void setBrightness(int level) { cout << "  [灯光] 亮度：" << level << "%" << endl; }
    void setColor(int r, int g, int b) { cout << "  [灯光] 颜色：RGB(" << r << "," << g << "," << b << ")" << endl; }
};

// ============================================================
// 【子系统 2】空调系统
// ============================================================
class AirConditioner {
public:
    void powerOn() { cout << "  [空调] 打开压缩机" << endl; }
    void powerOff() { cout << "  [空调] 关闭压缩机" << endl; }
    void setTemperature(float temp) { cout << "  [空调] 温度：" << temp << "°C" << endl; }
    void setFanSpeed(int speed) { cout << "  [空调] 风速：" << speed << "档" << endl; }
    void enableEcoMode() { cout << "  [空调] 节能模式" << endl; }
};

// ============================================================
// 【子系统 3】窗帘系统
// ============================================================
class CurtainSystem {
public:
    void open() { cout << "  [窗帘] 打开窗帘" << endl; }
    void close() { cout << "  [窗帘] 关闭窗帘" << endl; }
    void setPosition(int percent) { cout << "  [窗帘] 位置：" << percent << "%" << endl; }
};

// ============================================================
// 【子系统 4】音响系统
// ============================================================
class AudioSystem {
public:
    void powerOn() { cout << "  [音响] 打开功放" << endl; }
    void powerOff() { cout << "  [音响] 关闭功放" << endl; }
    void setVolume(int level) { cout << "  [音响] 音量：" << level << endl; }
    void selectSource(const string& source) { cout << "  [音响] 音源：" << source << endl; }
};

// ============================================================
// 【外观类】智能家居控制器
// ============================================================
// 提供简化的统一接口，隐藏子系统复杂性
class SmartHomeFacade {
private:
    LightSystem light;
    AirConditioner ac;
    CurtainSystem curtain;
    AudioSystem audio;

public:
    // 场景模式：一键设置多个子系统
    void comeHome() {
        cout << "\n=== 回家模式 ===" << endl;
        curtain.open();
        light.powerOn();
        light.setBrightness(80);
        ac.powerOn();
        ac.setTemperature(26.0f);
        ac.setFanSpeed(2);
        audio.powerOn();
        audio.setVolume(30);
        audio.selectSource("Bluetooth");
    }
    
    void leaveHome() {
        cout << "\n=== 离家模式 ===" << endl;
        light.powerOff();
        ac.powerOff();
        curtain.close();
        audio.powerOff();
    }
    
    void sleep() {
        cout << "\n=== 睡眠模式 ===" << endl;
        curtain.close();
        light.setBrightness(0);
        ac.setTemperature(24.0f);
        ac.enableEcoMode();
        audio.powerOff();
    }
    
    // 简化控制接口
    void setLight(int brightness) {
        light.powerOn();
        light.setBrightness(brightness);
    }
    
    void setAC(float temperature) {
        ac.powerOn();
        ac.setTemperature(temperature);
        ac.enableEcoMode();
    }
};

// ============================================================
// 【主函数】
// ============================================================
int main() {
    cout << "=== 外观模式：智能家居控制系统 ===" << endl;
    cout << "关键：外观类提供简单接口，隐藏子系统复杂性\n" << endl;
    
    SmartHomeFacade smartHome;
    
    // 场景 1：回家
    cout << "【用户】我回来了！" << endl;
    smartHome.comeHome();
    
    // 场景 2：手动调节
    cout << "\n【用户】灯光调暗一点" << endl;
    smartHome.setLight(50);
    
    // 场景 3：睡觉
    cout << "\n【用户】晚安" << endl;
    smartHome.sleep();
    
    // 场景 4：离家
    cout << "\n【用户】出门了" << endl;
    smartHome.leaveHome();
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
