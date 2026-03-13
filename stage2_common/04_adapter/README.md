# 04. 适配器模式（Adapter Pattern）

## 一句话理解

> **接口转换，让不兼容的东西能一起用**
> 
> 就像 USB 转 Type-C 转接头，让旧充电器能给新手机充电。

---

## 要解决的问题

**场景**：系统原本用 SSD1306 OLED，现在缺货要换 ILI9341 TFT，但接口完全不同。

**传统写法的问题**：
```cpp
// ❌ 问题：要修改所有调用处
void show_info() {
    oled.showText(0, 0, "Hello");  // 要改成 tft.tft_print()
}
```

---

## 适配器模式解法

```cpp
// 1. 目标接口（系统期望的）
class DisplayTarget {
    virtual void showText(int x, int y, string text) = 0;
};

// 2. 被适配者（新屏幕原有接口）
class ILI9341_TFT {
    void tft_print(string text);  // 接口不同
};

// 3. 适配器（转换接口）
class ILI9341Adapter : public DisplayTarget {
private:
    ILI9341_TFT* tft;
public:
    void showText(int x, int y, string text) override {
        tft->tft_setCursor(x, y);
        tft->tft_print(text);  // 内部调用 TFT 方法
    }
};

// 4. 现有代码不用改
ui.showText(0, 0, "Hello");
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **目标接口** | `DisplayTarget` | 客户端期望的接口 |
| **被适配者** | `ILI9341_TFT` | 需要适配的现有类 |
| **适配器** | `ILI9341Adapter` | 实现目标接口，包装被适配者 |

---

## 编译运行

```bash
cd stage2_common/04_adapter
mkdir build && cd build
cmake .. && make
./adapter
```

---

## 关键要点

1. **不修改现有代码**：这是适配器的核心价值
2. **对象适配器**：推荐用组合（持有被适配者）
3. **类适配器**：多重继承（较少用）

---

## 练习

1. 为 SSD1306_OLED 创建适配器
2. 实现一个传感器适配器，统一 DHT11 和 BMP280 接口
