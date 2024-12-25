#include  <fdsc_utils/highState.hpp>
#include <std_msgs/UInt16MultiArray.h>

namespace FDSC
{
        highState::highState() {
            // 订阅 foot_force_data 话题
            foot_force_subscriber = nh.subscribe("/foot_force_data", 10, &highState::footForceCallback, this);
        }
        
        void highState::dataToBmsState(const std::vector<uint8_t>& data) {

        void highState::dataToImu(const std::vector<uint8_t>& data) {

        void highState::dataToMotorState(const std::vector<uint8_t>& data,int index) {
        
        
        void highState::parseData(const std::vector<uint8_t>& data) {
                // we do not have this in go 1 air
                for (int i = 0; i < 4; i++) {
                    //footForce[i] = hex_to_u16_i(data, 869 + i * 2, 871 + i * 2);
                    footForce[i] = footForce[i];
                    footForceEst[i] = hex_to_u16_i(data, 877 + i * 2, 879 + i * 2);
                }
        }
        void highState::footForceCallback(const std_msgs::UInt16MultiArray::ConstPtr& msg) {
            // 将接收到的 foot_force 数据更新到 footForce
            if (msg->data.size() == 4) {
                for (int i = 0; i < 4; ++i) {
                    footForce[i] = msg->data[i]; // 更新 footForce
                }
            }else {
                ROS_WARN("Invalid foot force data received.");
            }
        }


        
}





#include <ros/ros.h>
#include <fdsc_utils/free_dog_sdk_h.hpp>

int main(int argc, char** argv) {
    std::string settings = "HIGH_WIRED_DEFAULTS";//"SIM_DEFAULTS";//HIGH_WIFI_DEFAULTS
    FDSC::UnitreeConnection conn(settings);
    conn.startRecv();
    FDSC::highCmd hcmd;
    FDSC::highState hstate;

    
    ros::init(argc, argv, "serial_reader_node");
    ros::NodeHandle nh;
    ros::spin();

    // Send an empty command to tell the dog the receive port and initialize the connection
    std::vector<uint8_t> cmd_bytes = hcmd.buildCmd(false);
    conn.send(cmd_bytes);
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Some time to collect packets

    int i = 0;
    int max_iter = 14;
    while (true) {
        std::vector<std::vector<uint8_t>> dataall = conn.getData();
        if (dataall.size()!=0)
        {
            std::cout<<"reveive pkg len: "<<dataall.size()<<std::endl;
            // for (auto data:dataall)
            {
                
                std::cout<<SetForeRED << "------------------Foot Force info(No data In Go1 Pro): -------------------" << std::endl;
                std::cout<<"Footforce: ";
                for (auto data_f : hstate.footForce)
                {
                    std::cout<<float(data_f)<<" ";
                }
                std::cout<<std::endl;
                             
            }
            
        }

    }

    return 0;
}










#ifndef _FDSC_HIGHSTATE_H_
#define _FDSC_HIGHSTATE_H_
#include <iostream>
#include <vector>
#include <array>
#include <serial/serial.h>
#include <ros/ros.h>
#include <std_msgs/UInt16MultiArray.h>
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>

namespace FDSC{
    
    class highState {
        public:
            highState();
            
            
            void dataToBmsState(const std::vector<uint8_t>& data);
            void dataToImu(const std::vector<uint8_t>& data);
            void dataToMotorState(const std::vector<uint8_t>& data,int index);
            void parseData(const std::vector<uint8_t>& data);
            void footForceCallback(const std_msgs::UInt16MultiArray::ConstPtr& msg);
        
        private:
            ros::Subscriber foot_force_subscriber;
            ros::NodeHandle nh;
        };
} //END FDSC
#endif












