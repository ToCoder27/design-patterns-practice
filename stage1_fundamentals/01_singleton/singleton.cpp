/**
 * @file singleton.cpp
 * @brief 单例模式示例 - UART 串口管理器
 * 
 * 一句话理解：全局只有一个实例，谁都用它，不会冲突
 * 类比：公司只有一个财务部，所有部门都要通过它处理财务
 */

#include <iostream>
#include <string>

using namespace std;

/**
 * @class UARTManager
 * @brief UART 串口管理器（单例类）
 * 
 * 场景：嵌入式系统中，UART 硬件资源只能被初始化一次
 * 多个模块（日志、命令解析、数据传输）都要使用 UART
 * 
 * 核心三要素：
 * 1. 私有构造函数 - 防止外部 new
 * 2. 静态唯一实例 - 保存全局唯一对象
 * 3. 全局访问点 - getInstance() 提供统一入口
 */
class UARTManager {
private:
    /** @brief 静态成员：单例实例 */
    static UARTManager* instance;

    /** @brief 波特率配置 */
    int baudrate;
    
    /** @brief 端口名称 */
    string port_name;
    
    /** @brief 初始化标志 */
    bool is_initialized;

    /**
     * @brief 私有构造函数 - 防止外部创建实例
     */
    UARTManager() : baudrate(0), port_name(""), is_initialized(false) {
        cout << "[UARTManager] 构造函数被调用（只应执行一次）" << endl;
    }

public:
    /**
     * @brief 删除拷贝构造函数 - 防止拷贝
     */
    UARTManager(const UARTManager&) = delete;
    
    /**
     * @brief 删除赋值运算符 - 防止赋值
     */
    UARTManager& operator=(const UARTManager&) = delete;

    /**
     * @brief 全局访问点 - 获取唯一实例
     * @return UARTManager* 单例实例指针
     */
    static UARTManager* getInstance() {
        if (instance == nullptr) {
            instance = new UARTManager();
        }
        return instance;
    }

    /**
     * @brief 初始化 UART（只能调用一次）
     * @param port 端口名称，如 "UART1"
     * @param baud 波特率，如 115200
     * @return bool 成功返回 true，已初始化返回 false
     */
    bool init(const string& port, int baud) {
        if (is_initialized) {
            cout << "[UART] 警告：UART 已初始化，忽略重复初始化请求" << endl;
            return false;
        }
        
        port_name = port;
        baudrate = baud;
        is_initialized = true;
        cout << "[UART] 初始化：" << port_name << " @ " << baudrate << " bps" << endl;
        return true;
    }

    /**
     * @brief 发送数据
     * @param data 要发送的字符串
     */
    void send(const string& data) {
        if (!is_initialized) {
            cout << "[UART] 错误：未初始化！" << endl;
            return;
        }
        cout << "[UART] 发送：\"" << data << "\"" << endl;
    }

    /**
     * @brief 获取状态
     * @return bool 是否已初始化
     */
    bool isReady() const { return is_initialized; }

    /**
     * @brief 析构函数
     */
    ~UARTManager() {
        if (is_initialized) {
            cout << "[UART] 关闭串口：" << port_name << endl;
        }
    }
};

/** @brief 静态成员初始化 */
UARTManager* UARTManager::instance = nullptr;

// ============================================================
// 【客户端模块】演示多个模块共享 UART 资源
// ============================================================

/**
 * @brief 日志系统初始化
 */
void Logger_Init() {
    cout << "\n=== 日志系统初始化 ===" << endl;
    UARTManager::getInstance()->init("UART1", 115200);
}

/**
 * @brief 日志消息
 * @param msg 消息内容
 */
void Logger_Log(const string& msg) {
    UARTManager::getInstance()->send("[LOG] " + msg);
}

/**
 * @brief 命令解析器初始化
 */
void CommandParser_Init() {
    cout << "\n=== 命令解析器初始化 ===" << endl;
    UARTManager::getInstance()->init("UART1", 9600);  // 重复初始化，被阻止
}

/**
 * @brief 命令解析器处理命令
 */
void CommandParser_Process() {
    auto uart = UARTManager::getInstance();
    if (uart->isReady()) {
        uart->send("[CMD] 处理命令");
    }
}

/**
 * @brief 数据传输模块发送数据
 */
void DataTransfer_Send() {
    cout << "\n=== 数据传输模块 ===" << endl;
    UARTManager::getInstance()->send("[DATA] 发送数据包...");
}

/**
 * @brief 主函数 - 演示多个模块共享 UART 资源
 * @return int 退出码
 */
int main() {
    cout << "=== 单例模式演示：UART 串口管理器 ===" << endl;
    cout << "场景：多个模块共享同一个 UART 硬件资源\n" << endl;
    
    // 模块 1：日志系统先初始化
    Logger_Init();
    
    // 模块 2：命令解析器尝试初始化（会被阻止）
    CommandParser_Init();
    
    // 各模块使用 UART
    Logger_Log("系统启动完成");
    CommandParser_Process();
    DataTransfer_Send();
    
    // 验证实例唯一性
    cout << "\n=== 验证实例唯一性 ===" << endl;
    UARTManager* uart1 = UARTManager::getInstance();
    UARTManager* uart2 = UARTManager::getInstance();
    cout << "实例 1 地址：" << uart1 << endl;
    cout << "实例 2 地址：" << uart2 << endl;
    cout << "是否同一实例：" << (uart1 == uart2 ? "是 ✓" : "否 ✗") << endl;
    
    cout << "\n=== 演示结束 ===" << endl;
    return 0;
}
