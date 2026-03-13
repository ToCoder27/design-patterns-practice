# 07. 命令模式（Command Pattern）

## 一句话理解

> **把操作封装成对象，可以排队、撤销、重做**
> 
> 就像餐厅点菜：顾客点单（命令）→ 服务员记录（队列）→ 厨师做菜（执行）→ 可以退菜（撤销）。

---

## 要解决的问题

**场景**：智能遥控器、任务队列、Undo/Redo 功能。

**传统写法的问题**：
```cpp
// ❌ 问题：无法撤销，无法记录历史，无法实现宏命令
void pressButton(int id) {
    if (id == 1) light->turnOn();
    else if (id == 2) fan->turnOn();
    // 无法撤销，无法记录
}
```

---

## 命令模式解法

```cpp
// 1. 命令接口
class Command {
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// 2. 具体命令
class LightOnCommand : public Command {
private:
    Light* light;
    bool wasOn;  // 记录之前状态
public:
    void execute() override {
        wasOn = light->isOn();
        light->turnOn();
    }
    void undo() override {
        if (!wasOn) light->turnOff();
    }
};

// 3. 调用者（遥控器）
class Remote {
    vector<Command*> history;
    void pressButton(Command* cmd) {
        cmd->execute();
        history.push_back(cmd);  // 记录历史
    }
    void undo() {
        history.back()->undo();
        history.pop_back();
    }
};
```

---

## 核心要点

| 角色 | 类名 | 作用 |
|------|------|------|
| **命令接口** | `Command` | 定义执行和撤销接口 |
| **具体命令** | `LightOnCommand` 等 | 实现具体操作 |
| **接收者** | `Light` 等 | 执行实际工作 |
| **调用者** | `Remote` | 触发命令执行 |

---

## 编译运行

```bash
cd stage2_common/07_command
mkdir build && cd build
cmake .. && make
./command
```

---

## 关键要点

1. **撤销功能**：记录执行前状态，undo 时恢复
2. **历史记录**：可以记录所有执行过的命令
3. **宏命令**：组合多个命令一起执行

---

## 练习

1. 添加空调开关命令
2. 实现重做（Redo）功能
