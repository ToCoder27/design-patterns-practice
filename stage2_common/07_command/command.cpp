#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

// ============================================================
// 命令模式（Command Pattern）
// ============================================================
// 一句话理解：把操作封装成对象，可以排队、撤销、重做
// 类比：餐厅点菜：顾客点单（命令）→ 服务员记录 → 厨师做菜
// ============================================================

// 前向声明
class Light;
class Fan;

// ------------------------------------------------------------
// 【命令接口】
// ------------------------------------------------------------
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual string getDescription() = 0;
};

// ============================================================
// 【接收者】灯光
// ============================================================
class Light {
private:
    bool isOn = false;
public:
    void turnOn() { isOn = true; cout << "  [灯光] 打开" << endl; }
    void turnOff() { isOn = false; cout << "  [灯光] 关闭" << endl; }
    bool isOnState() const { return isOn; }
};

// ============================================================
// 【接收者】风扇
// ============================================================
class Fan {
private:
    bool isOn = false;
    int speed = 0;
public:
    void turnOn() { isOn = true; speed = 3; cout << "  [风扇] 打开（" << speed << "档）" << endl; }
    void turnOff() { isOn = false; speed = 0; cout << "  [风扇] 关闭" << endl; }
    bool isOnState() const { return isOn; }  // 添加公有访问方法
};

// ============================================================
// 【具体命令 1】开灯命令
// ============================================================
class LightOnCommand : public Command {
private:
    Light* light;
    bool wasOn;  // 记录之前状态，用于撤销
public:
    LightOnCommand(Light* l) : light(l), wasOn(false) {}
    
    void execute() override {
        wasOn = light->isOnState();  // 记录状态
        light->turnOn();
    }
    
    void undo() override {
        if (!wasOn) light->turnOff();
    }
    
    string getDescription() override { return "开灯"; }
};

// ============================================================
// 【具体命令 2】关灯命令
// ============================================================
class LightOffCommand : public Command {
private:
    Light* light;
    bool wasOn;
public:
    LightOffCommand(Light* l) : light(l), wasOn(false) {}
    
    void execute() override {
        wasOn = light->isOnState();
        light->turnOff();
    }
    
    void undo() override {
        if (wasOn) light->turnOn();
    }
    
    string getDescription() override { return "关灯"; }
};

// ============================================================
// 【具体命令 3】风扇开关命令
// ============================================================
class FanOnCommand : public Command {
private:
    Fan* fan;
    bool wasOn;
public:
    FanOnCommand(Fan* f) : fan(f), wasOn(false) {}
    
    void execute() override { wasOn = fan->isOnState(); fan->turnOn(); }
    void undo() override { if (!wasOn) fan->turnOff(); }
    string getDescription() override { return "开风扇"; }
};

// ============================================================
// 【具体命令 4】宏命令（组合多个命令）
// ============================================================
class MacroCommand : public Command {
private:
    vector<shared_ptr<Command>> commands;
public:
    void addCommand(shared_ptr<Command> cmd) { commands.push_back(cmd); }
    
    void execute() override {
        cout << "\n[宏命令] 执行多个操作..." << endl;
        for (auto& cmd : commands) cmd->execute();
    }
    
    void undo() override {
        cout << "\n[宏命令] 撤销..." << endl;
        for (int i = commands.size() - 1; i >= 0; i--) {
            commands[i]->undo();
        }
    }
    
    string getDescription() override { return "宏命令"; }
};

// ============================================================
// 【调用者】智能遥控器
// ============================================================
class SmartRemote {
private:
    vector<shared_ptr<Command>> history;
public:
    void pressButton(shared_ptr<Command> cmd) {
        cout << "\n[遥控器] " << cmd->getDescription() << endl;
        cmd->execute();
        history.push_back(cmd);
    }
    
    void undo() {
        if (!history.empty()) {
            cout << "\n[遥控器] 撤销" << endl;
            history.back()->undo();
            history.pop_back();
        }
    }
};

// ============================================================
// 【主函数】
// ============================================================
int main() {
    cout << "=== 命令模式：智能遥控器 ===" << endl;
    cout << "关键：命令封装操作，支持撤销和宏命令\n" << endl;
    
    // 创建接收者
    auto light = make_shared<Light>();
    auto fan = make_shared<Fan>();
    
    // 创建命令
    auto lightOn = make_shared<LightOnCommand>(light.get());
    auto lightOff = make_shared<LightOffCommand>(light.get());
    auto fanOn = make_shared<FanOnCommand>(fan.get());
    
    // 创建宏命令：回家模式
    auto homeMacro = make_shared<MacroCommand>();
    homeMacro->addCommand(lightOn);
    homeMacro->addCommand(fanOn);
    
    // 创建遥控器
    SmartRemote remote;
    
    // 场景 1：单独控制
    cout << "=== 场景 1: 单独控制 ===" << endl;
    remote.pressButton(lightOn);
    remote.pressButton(fanOn);
    
    // 场景 2：撤销
    cout << "\n=== 场景 2: 撤销 ===" << endl;
    remote.undo();
    remote.undo();
    
    // 场景 3：宏命令
    cout << "\n=== 场景 3: 宏命令（回家模式）===" << endl;
    remote.pressButton(homeMacro);
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
