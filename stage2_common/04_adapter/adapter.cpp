#include <iostream>
#include <string>

using namespace std;

// ============================================================
// 适配器模式（Adapter Pattern）
// ============================================================
// 一句话理解：接口转换，让不兼容的东西能一起用
// 类比：USB 转 Type-C 转接头，让旧充电器能给新手机充电
// ============================================================

// ------------------------------------------------------------
// 【目标接口】系统期望的显示屏接口
// ------------------------------------------------------------
// 这是系统原本使用的接口标准
class DisplayTarget {
public:
    virtual ~DisplayTarget() = default;
    virtual bool init() = 0;
    virtual void clear() = 0;
    virtual void showText(int x, int y, const string& text) = 0;
    virtual void refresh() = 0;
};

// ============================================================
// 【被适配者】新屏幕的原有接口（与系统不兼容）
// ============================================================
// 这是新买的 TFT 屏，接口与系统期望的不一样
class ILI9341_TFT {
private:
    int width, height;
public:
    ILI9341_TFT(int w, int h) : width(w), height(h) {}
    
    // TFT 的接口与系统期望的不同
    bool tft_init_spi() {
        cout << "[TFT] 初始化 SPI，分辨率 " << width << "x" << height << endl;
        return true;
    }
    void tft_fillRect(int x, int y, int w, int h, int color = 0) {
        cout << "[TFT] 填充矩形 (" << x << "," << y << ") " << w << "x" << h << " 颜色=" << color << endl;
    }
    void tft_setCursor(int x, int y) {
        cout << "[TFT] 设置光标 (" << x << "," << y << ")" << endl;
    }
    void tft_print(const string& text) {
        cout << "[TFT] 打印：\"" << text << "\"" << endl;
    }
    void tft_display() {
        cout << "[TFT] 刷新显示" << endl;
    }
};

// ============================================================
// 【适配器】让新屏幕兼容旧接口
// ============================================================
// 关键：实现目标接口，内部调用被适配者方法
class ILI9341Adapter : public DisplayTarget {
private:
    ILI9341_TFT* tft;  // 持有被适配对象
public:
    ILI9341Adapter(int w, int h) { tft = new ILI9341_TFT(w, h); }
    ~ILI9341Adapter() { delete tft; }
    
    // 实现目标接口：转换调用
    bool init() override {
        cout << "[适配器] init() -> tft_init_spi()" << endl;
        return tft->tft_init_spi();
    }
    void clear() override {
        cout << "[适配器] clear() -> tft_fillRect(全黑)" << endl;
        tft->tft_fillRect(0, 0, 320, 240, 0);
    }
    void showText(int x, int y, const string& text) override {
        cout << "[适配器] showText() -> setCursor + print" << endl;
        tft->tft_setCursor(x, y);
        tft->tft_print(text);
    }
    void refresh() override {
        cout << "[适配器] refresh() -> tft_display()" << endl;
        tft->tft_display();
    }
};

// ============================================================
// 【客户端】原有系统，只依赖 DisplayTarget 接口
// ============================================================
// 这段代码完全不需要修改！
class SystemUI {
private:
    DisplayTarget* display;
public:
    SystemUI(DisplayTarget* disp) : display(disp) {}
    
    void boot() {
        cout << "\n=== 系统启动 ===" << endl;
        display->init();
        display->clear();
        display->showText(10, 20, "System Boot");
        display->showText(10, 40, "Version 1.0");
        display->refresh();
    }
};

// ============================================================
// 【主函数】
// ============================================================
int main() {
    cout << "=== 适配器模式：TFT 屏兼容 OLED 接口 ===" << endl;
    cout << "关键：现有系统代码完全不用改\n" << endl;
    
    // 创建适配器（包装 TFT 屏）
    ILI9341Adapter adapter(320, 240);
    
    // 创建系统 UI（不需要修改！）
    SystemUI ui(&adapter);
    ui.boot();
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
