#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// 【抽象策略】：定义“处理信号”的接口
// 输入 raw_val，返回处理后的结果 (true/false)
class FilterStrategy {
    public:
        virtual bool process(bool raw_val) = 0;
        virtual ~FilterStrategy() = default; // 虚析构函数好习惯
};

// 【具体策略 1】：直通模式 (无状态)
// 就像你的 Demo，不需要任何成员变量
class DirectStrategy : public FilterStrategy {
    public:
        bool process(bool raw_val) override {
            // 直接透传，不做处理
            return raw_val; 
        }
};

// 【具体策略 2】：稳定滤波模式 (有状态！！！)
// 这是一个“带干粮”的策略，它需要记录历史
class StableFilterStrategy : public FilterStrategy {
    private:
        int valid_count = 0;      // 【状态】：当前连续有效的次数
        int threshold = 3;        // 【配置】：需要连续多少次才算数

    public:
        StableFilterStrategy(int th) : threshold(th) {}

        bool process(bool raw_val) override {
            if (raw_val == true) {
                valid_count++;
                // 只有连续计数达到阈值，才返回 true
                if (valid_count >= threshold) {
                    return true;
                }
            } else {
                // 如果中间断了，计数器清零（这就叫状态重置）
                valid_count = 0;
            }
            return false;
        }
};

// 【上下文】：使用策略的传感器类
class SensorContext {
    private:
        shared_ptr<FilterStrategy> filter;
    public:
        void set_strategy(shared_ptr<FilterStrategy> new_strategy) {
            filter = new_strategy;
        }

        void read_sensor(bool raw_input) {
            if (filter) {
                bool result = filter->process(raw_input);
                cout << "输入: " << raw_input << " -> 输出: " << result << endl;
            }
        }
};

int main() {
    SensorContext sensor;

    cout << "--- 场景1: 使用直通策略 (无状态) ---" << endl;
    sensor.set_strategy(make_shared<DirectStrategy>());
    sensor.read_sensor(1); // 只要有1，马上输出1
    sensor.read_sensor(0);

    cout << "\n--- 场景2: 使用滤波策略 (有状态) ---" << endl;
    // 注意：这里我们创建了一个对象，它内部自带了计数器 valid_count = 0
    sensor.set_strategy(make_shared<StableFilterStrategy>(3)); 

    sensor.read_sensor(1); // 第1次：内部计数=1，没到3，输出0
    sensor.read_sensor(1); // 第2次：内部计数=2，没到3，输出0
    sensor.read_sensor(0); // 抖动！：内部计数清零，输出0
    sensor.read_sensor(1); // 重新开始：内部计数=1，输出0
    sensor.read_sensor(1); // 内部计数=2
    sensor.read_sensor(1); // 第3次：内部计数=3！终于输出 1
    
    return 0;
}