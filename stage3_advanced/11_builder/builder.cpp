#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ============================================================
// 建造者模式（Builder Pattern）
// ============================================================
// 一句话理解：分步骤组装复杂对象
// 类比：电脑城组装电脑：CPU、内存、显卡、硬盘一步步安装
// ============================================================

// ------------------------------------------------------------
// 【产品】电脑类
// ------------------------------------------------------------
class Computer {
private:
    string cpu, memory, gpu, storage;
    
public:
    void setCpu(const string& c) { cpu = c; }
    void setMemory(const string& m) { memory = m; }
    void setGpu(const string& g) { gpu = g; }
    void setStorage(const string& s) { storage = s; }
    
    void showConfig() const {
        cout << "=== 电脑配置 ===" << endl;
        cout << "CPU:    " << cpu << endl;
        cout << "内存：   " << memory << endl;
        cout << "显卡：   " << gpu << endl;
        cout << "存储：   " << storage << endl;
    }
};

// ------------------------------------------------------------
// 【建造者接口】
// ------------------------------------------------------------
class ComputerBuilder {
public:
    virtual ~ComputerBuilder() = default;
    virtual void buildCpu() = 0;
    virtual void buildMemory() = 0;
    virtual void buildGpu() = 0;
    virtual void buildStorage() = 0;
    virtual Computer* getComputer() = 0;
};

// ------------------------------------------------------------
// 【具体建造者】游戏电脑建造者
// ------------------------------------------------------------
class GamingComputerBuilder : public ComputerBuilder {
private:
    Computer* computer;
    
public:
    GamingComputerBuilder() { computer = new Computer(); }
    ~GamingComputerBuilder() { delete computer; }
    
    void buildCpu() override {
        cout << "  [游戏电脑] 安装 i9-13900K CPU" << endl;
        computer->setCpu("Intel i9-13900K");
    }
    void buildMemory() override {
        cout << "  [游戏电脑] 安装 32GB DDR5 内存" << endl;
        computer->setMemory("32GB DDR5");
    }
    void buildGpu() override {
        cout << "  [游戏电脑] 安装 RTX 4080 显卡" << endl;
        computer->setGpu("NVIDIA RTX 4080");
    }
    void buildStorage() override {
        cout << "  [游戏电脑] 安装 2TB SSD" << endl;
        computer->setStorage("2TB NVMe SSD");
    }
    Computer* getComputer() override {
        Computer* result = computer;
        computer = new Computer();  // 重置
        return result;
    }
};

// ------------------------------------------------------------
// 【具体建造者】办公电脑建造者
// ------------------------------------------------------------
class OfficeComputerBuilder : public ComputerBuilder {
private:
    Computer* computer;
    
public:
    OfficeComputerBuilder() { computer = new Computer(); }
    ~OfficeComputerBuilder() { delete computer; }
    
    void buildCpu() override {
        cout << "  [办公电脑] 安装 i5-13400 CPU" << endl;
        computer->setCpu("Intel i5-13400");
    }
    void buildMemory() override {
        cout << "  [办公电脑] 安装 16GB DDR4 内存" << endl;
        computer->setMemory("16GB DDR4");
    }
    void buildGpu() override {
        cout << "  [办公电脑] 使用集成显卡" << endl;
        computer->setGpu("Intel UHD Graphics");
    }
    void buildStorage() override {
        cout << "  [办公电脑] 安装 512GB SSD" << endl;
        computer->setStorage("512GB NVMe SSD");
    }
    Computer* getComputer() override {
        Computer* result = computer;
        computer = new Computer();
        return result;
    }
};

// ------------------------------------------------------------
// 【指挥者】电脑城店员
// ------------------------------------------------------------
class ShopAssistant {
public:
    // 构建完整电脑
    void buildFullComputer(ComputerBuilder& builder) {
        builder.buildCpu();
        builder.buildMemory();
        builder.buildGpu();
        builder.buildStorage();
    }
    
    // 构建基础电脑（只装 CPU 和内存）
    void buildBasicComputer(ComputerBuilder& builder) {
        builder.buildCpu();
        builder.buildMemory();
    }
};

// ============================================================
// 【主函数】
// ============================================================
int main() {
    cout << "=== 建造者模式：组装电脑 ===" << endl;
    cout << "关键：分步骤组装，同样的流程可以创建不同的产品\n" << endl;
    
    ShopAssistant assistant;
    
    // 场景 1：组装游戏电脑
    cout << "=== 场景 1: 游戏电脑 ===" << endl;
    GamingComputerBuilder gamingBuilder;
    assistant.buildFullComputer(gamingBuilder);
    Computer* gamingPC = gamingBuilder.getComputer();
    gamingPC->showConfig();
    
    // 场景 2：组装办公电脑
    cout << "\n=== 场景 2: 办公电脑 ===" << endl;
    OfficeComputerBuilder officeBuilder;
    assistant.buildFullComputer(officeBuilder);
    Computer* officePC = officeBuilder.getComputer();
    officePC->showConfig();
    
    // 场景 3：自定义（只装 CPU 和内存）
    cout << "\n=== 场景 3: 自定义（基础配置）===" << endl;
    GamingComputerBuilder customBuilder;
    assistant.buildBasicComputer(customBuilder);
    Computer* customPC = customBuilder.getComputer();
    customPC->showConfig();
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
