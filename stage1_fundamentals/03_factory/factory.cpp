#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ============================================================
// 工厂模式（Factory Pattern）
// ============================================================
// 一句话理解：封装对象创建逻辑，客户端只管用
// 类比：餐厅点菜，你只管说"来份宫保鸡丁"，不用知道厨房怎么做
// ============================================================

// ------------------------------------------------------------
// 【产品接口】抽象按键类
// ------------------------------------------------------------
class Button {
public:
    virtual ~Button() = default;
    virtual bool init() = 0;
    virtual bool isPressed() = 0;
    virtual const char* getName() = 0;
};

// ------------------------------------------------------------
// 【具体产品】普通机械按键
// ------------------------------------------------------------
class NormalButton : public Button {
private:
    int gpio_pin;
public:
    NormalButton(int pin) : gpio_pin(pin) {}
    bool init() override {
        cout << "  [NormalButton] 初始化 GPIO " << gpio_pin << endl;
        return true;
    }
    bool isPressed() override {
        cout << "  [NormalButton] 读取 GPIO " << gpio_pin << endl;
        return false;
    }
    const char* getName() override { return "普通按键"; }
};

// ------------------------------------------------------------
// 【具体产品】触摸按键
// ------------------------------------------------------------
class TouchButton : public Button {
private:
    int touch_channel;
public:
    TouchButton(int channel) : touch_channel(channel) {}
    bool init() override {
        cout << "  [TouchButton] 初始化触摸通道 " << touch_channel << endl;
        return true;
    }
    bool isPressed() override {
        cout << "  [TouchButton] 检测触摸通道 " << touch_channel << endl;
        return false;
    }
    const char* getName() override { return "触摸按键"; }
};

// ------------------------------------------------------------
// 【具体产品】旋钮编码器
// ------------------------------------------------------------
class RotaryButton : public Button {
private:
    int pin_a, pin_b;
public:
    RotaryButton(int a, int b) : pin_a(a), pin_b(b) {}
    bool init() override {
        cout << "  [RotaryButton] 初始化编码器 A=" << pin_a << ", B=" << pin_b << endl;
        return true;
    }
    bool isPressed() override {
        cout << "  [RotaryButton] 检测旋钮按下..." << endl;
        return false;
    }
    const char* getName() override { return "旋钮编码器"; }
};

// ============================================================
// 【工厂】简单工厂模式
// ============================================================
// 一个工厂类处理所有类型的创建
class ButtonFactory {
public:
    enum class Type { NORMAL, TOUCH, ROTARY };
    
    // 静态工厂方法：根据类型创建
    static unique_ptr<Button> create(Type type, int pin) {
        cout << "[工厂] 创建 ";
        switch (type) {
            case Type::NORMAL:
                cout << "普通按键 (GPIO=" << pin << ")" << endl;
                return make_unique<NormalButton>(pin);
            case Type::TOUCH:
                cout << "触摸按键 (通道=" << pin << ")" << endl;
                return make_unique<TouchButton>(pin);
            case Type::ROTARY:
                cout << "旋钮按键 (引脚=" << pin << ")" << endl;
                return make_unique<RotaryButton>(pin, pin + 1);
        }
        return nullptr;
    }
};

// ============================================================
// 【客户端】设备控制面板
// ============================================================
// 客户端只依赖工厂，不直接创建具体产品
class DevicePanel {
private:
    unique_ptr<Button> buttons[4];
    int button_count;

public:
    DevicePanel() : button_count(0) {}
    
    // 通过工厂添加按键：客户端只传类型
    void addButton(ButtonFactory::Type type, int pin) {
        if (button_count < 4) {
            cout << "\n添加按键 " << button_count << ":" << endl;
            buttons[button_count++] = ButtonFactory::create(type, pin);
            buttons[button_count - 1]->init();
        }
    }
    
    void poll() {
        cout << "\n=== 轮询按键 ===" << endl;
        for (int i = 0; i < button_count; i++) {
            cout << "按键 " << i << " (" << buttons[i]->getName() << "): ";
            buttons[i]->isPressed();
        }
    }
};

// ============================================================
// 【主函数】
// ============================================================
int main() {
    cout << "=== 工厂模式演示：按键驱动创建 ===" << endl;
    cout << "关键：客户端只传类型，工厂负责创建\n" << endl;
    
    DevicePanel panel;
    
    // 客户端只关心类型，不关心创建细节
    panel.addButton(ButtonFactory::Type::NORMAL, 10);
    panel.addButton(ButtonFactory::Type::TOUCH, 3);
    panel.addButton(ButtonFactory::Type::ROTARY, 5);
    panel.addButton(ButtonFactory::Type::NORMAL, 11);
    
    panel.poll();
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
