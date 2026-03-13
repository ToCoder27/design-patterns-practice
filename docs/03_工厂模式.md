# 03. 工厂模式（Factory Pattern）

## 一句话理解

> **封装对象创建逻辑，客户端只管用**
> 
> 就像餐厅点菜：你只管说"来份宫保鸡丁"，不用知道厨房怎么做。

---

## 要解决的问题

**场景**：系统支持多种按键（普通/触摸/旋钮），客户端不想直接 `new` 具体类。

**传统写法的问题**：
```cpp
// ❌ 问题：客户端耦合具体类，加新类型要改客户端
void setup() {
    btn1 = new NormalButton(10);
    btn2 = new TouchButton(3);
    btn3 = new RotaryButton(5, 6);
}
```

---

## 工厂模式解法

```cpp
// 1. 定义产品接口
class Button {
    virtual bool init() = 0;
};

// 2. 创建工厂
class ButtonFactory {
public:
    enum class Type { NORMAL, TOUCH, ROTARY };
    
    static unique_ptr<Button> create(Type type, int pin) {
        switch (type) {
            case Type::NORMAL: return make_unique<NormalButton>(pin);
            case Type::TOUCH:  return make_unique<TouchButton>(pin);
            case Type::ROTARY: return make_unique<RotaryButton>(pin, pin+1);
        }
    }
};

// 3. 客户端只传类型
panel.addButton(ButtonFactory::Type::TOUCH, 3);
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **产品接口** | `Button` | 定义统一接口 |
| **具体产品** | `NormalButton` 等 | 实现不同功能 |
| **工厂** | `ButtonFactory` | 封装创建逻辑 |

---

## 编译运行

```bash
cd stage1_fundamentals/03_factory
mkdir build && cd build
cmake .. && make
./factory
```

---

## 工厂模式的演变

### 简单工厂（本示例）
```cpp
ButtonFactory::create(Type::TOUCH, pin);
```

### 工厂方法（进阶）
```cpp
TouchButtonFactory factory;
factory.createButton(pin);
```

### 抽象工厂（高级）
```cpp
GUIFactory* factory = getFactory("Windows");
factory->createButton();
```

---

## 练习

1. 添加一个 `LEDButton` 类型（带 LED 指示的按键）
2. 实现从配置文件读取按键类型并创建
