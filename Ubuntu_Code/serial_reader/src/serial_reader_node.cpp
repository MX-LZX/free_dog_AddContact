#include <ros/ros.h>
#include <std_msgs/UInt16MultiArray.h>
#include <serial/serial.h>
#include <vector>
#include <sstream>
#include <string>

class SerialReaderNode {
public:
    SerialReaderNode() {
        // 初始化 ROS 节点和发布器
        ros::NodeHandle nh;
        serial_pub = nh.advertise<std_msgs::UInt16MultiArray>("foot_force_data", 10);

        // 初始化串口
        try {
            serial_port.setPort("/dev/ttyUSB0");  // 请根据您的设备调整串口名称
            serial_port.setBaudrate(115200);
            serial::Timeout timeout = serial::Timeout::simpleTimeout(1000);
            serial_port.setTimeout(timeout);
            serial_port.open();
        } catch (const serial::IOException& e) {
            ROS_ERROR("Unable to open port: %s", e.what());
        }
    }

    void readAndPublishData() {
        std_msgs::UInt16MultiArray foot_force_msg;
        while (ros::ok()) {
            if (serial_port.available()) {
                // 读取一行串口数据
                std::string line = serial_port.readline(100, "\n");
                ROS_INFO("Raw serial data: %s", line.c_str());

                std::stringstream ss(line);
                uint16_t value;
                foot_force_msg.data.clear();
                
                // 假设串口数据是以逗号分隔的四个数字（uint16_t 格式）
                for (int i = 0; i < 4; ++i) {
                    if (ss >> value) {
                        foot_force_msg.data.push_back(value);
                        if (ss.peek() == ',') ss.ignore();  // 跳过逗号
                    } else {
                        ROS_ERROR("Invalid serial data format.");
                        break;
                    }
                }
                
                // 只在接收到完整的 4 个数据时才发布消息
                if (foot_force_msg.data.size() == 4) {
                    serial_pub.publish(foot_force_msg);
                }
            }
            ros::spinOnce();
        }
    }

private:
    serial::Serial serial_port;
    ros::Publisher serial_pub;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "serial_reader_node");
    SerialReaderNode serial_reader;
    serial_reader.readAndPublishData();
    return 0;
}

