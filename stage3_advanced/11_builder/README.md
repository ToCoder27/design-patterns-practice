# 11. 建造者模式（Builder Pattern）

## 一句话理解

> **分步骤组装复杂对象**
> 
> 就像电脑城组装电脑：CPU、内存、显卡、硬盘一步步安装，最终得到一台完整的电脑。

---

## 要解决的问题

**场景**：产品很复杂，需要分步骤组装，同样的流程可以创建不同的配置。

**传统写法的问题**：
```cpp
// ❌ 问题：构造函数参数太多，难以维护
Computer* createComputer(string cpu, string mem, string gpu, string storage, ...) {
    // 一大串参数，容易搞混
}
```

---

## 建造者模式解法

```cpp
// 1. 产品
class Computer {
    string cpu, memory, gpu, storage;
};

// 2. 建造者接口
class ComputerBuilder {
    virtual void buildCpu() = 0;
    virtual void buildMemory() = 0;
    virtual void buildGpu() = 0;
    virtual Computer* getComputer() = 0;
};

// 3. 具体建造者
class GamingComputerBuilder : public ComputerBuilder {
    void buildCpu() override { computer->setCpu("i9"); }
    void buildGpu() override { computer->setGpu("RTX4080"); }
};

// 4. 指挥者（可选）
class ShopAssistant {
    void buildFullComputer(ComputerBuilder& builder) {
        builder.buildCpu();
        builder.buildMemory();
        builder.buildGpu();
    }
};
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **产品** | `Computer` | 被组装的复杂对象 |
| **建造者接口** | `ComputerBuilder` | 定义组装步骤 |
| **具体建造者** | `GamingComputerBuilder` 等 | 实现具体组装方式 |
| **指挥者** | `ShopAssistant` | 控制组装顺序（可选） |

---

## 编译运行

```bash
cd stage3_advanced/11_builder
mkdir build && cd build
cmake .. && make
./builder
```

---

## 关键要点

1. **分步骤组装**：复杂对象的创建分解为多个步骤
2. **不同表示**：同样的流程可以创建不同配置的产品
3. **指挥者可选**：客户端可以直接控制建造者

---

## 练习

1. 添加一个 `ServerComputerBuilder`（服务器电脑）
2. 添加新的部件：电源、机箱、散热器
