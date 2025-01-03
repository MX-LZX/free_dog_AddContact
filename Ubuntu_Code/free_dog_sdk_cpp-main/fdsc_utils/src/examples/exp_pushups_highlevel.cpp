#include <ros/ros.h>
#include <fdsc_utils/free_dog_sdk_h.hpp>
void show_joint_info(const std::vector<FDSC::MotorState> & mobj)
{
    for (int i = 0; i < 12; i++)
    {
        std::cout<<"Joint: "<<i<<"   Motor Mode: "<< mobj[i].mode <<" Motor NTC: "<<mobj[i].temperature<<" ";
        std::cout<<" q: "<<mobj[i].q<<" dq: "<<mobj[i].dq<<" ddq: "<<mobj[i].ddq<<" TauEst: "<<mobj[i].tauEst<<std::endl;
    }

}
int main(int argc, char** argv) {
    std::string settings = "HIGH_WIRED_DEFAULTS";//"SIM_DEFAULTS";//HIGH_WIFI_DEFAULTS
    
    ros::init(argc, argv, "highstate_node");
    ros::NodeHandle nh;
    
    FDSC::UnitreeConnection conn(settings);
    conn.startRecv();
    FDSC::highCmd hcmd;
    FDSC::highState hstate;

    // Send an empty command to tell the dog the receive port and initialize the connection
    std::vector<uint8_t> cmd_bytes = hcmd.buildCmd(false);
    conn.send(cmd_bytes);
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Some time to collect packets
    
    //ros::Rate loop_rate(100); // 设置循环频率为10Hz，避免 CPU 占用过高
    ros::AsyncSpinner spinner(4);
    spinner.start();

    int i = 0;
    int max_iter = 14;
    while (ros::ok()) {
        std::vector<std::vector<uint8_t>> dataall = conn.getData();
        if (dataall.size()!=0)
        {
            std::cout<<"reveive pkg len: "<<dataall.size()<<std::endl;
            // for (auto data:dataall)
            {
                /***std::vector<uint8_t> data = dataall.at(dataall.size()-1);
                std::cout<<"loop: "<<i<<" datalen: " <<std::dec<<data.size()<<std::endl;
                std::cout << "=============================" << std::endl;
                hstate.parseData(data);
                std::cout << SetForeGRN <<"------------------HardSoft Version info: -------------------" << std::endl;
                std::cout << "SN: " << "\t";     FDSC::byte_print(hstate.SN,false);std::cout << FDSC::decode_sn(hstate.SN)<< std::endl;
                std::cout << "Version: " << "\t";FDSC::byte_print(hstate.version,false);std::cout<< FDSC::decode_version(hstate.version) << std::endl;
                std::cout << SetForeGRN <<"------------------IMU info: -------------------" << std::endl;
                std::cout << "IMU Quaternion: ";    FDSC::pretty_show_vector(hstate.imu_quaternion);std::cout << std::endl;
                std::cout << "IMU Gyroscope: ";     FDSC::pretty_show_vector(hstate.imu_gyroscope)  ;std::cout<< std::endl;
                std::cout << "IMU Accelerometer: "; FDSC::pretty_show_vector(hstate.imu_accelerometer);std::cout << std::endl;
                std::cout << "IMU RPY: ";           FDSC::pretty_show_vector(hstate.imu_rpy) ;
                std::cout<<  "IMU Temperature: " << float(hstate.temperature_imu) << std::endl;
                // std::vector<uint8_t> imu_data(data.begin()+22,data.begin()+75);
                // FDSC::show_byte_data(data,8);
                // std::cout<<std::endl;
                std::cout<<SetForeGRN << "------------------Joint info: -------------------" << std::endl;
                show_joint_info(hstate.motorState);
                // BMS do not have any data from UDP data
                std::cout<<SetForeRED << "------------------BMS info(No data): -------------------" << std::endl;
                std::cout << "SOC: " << int(hstate.SOC) << std::endl;
                // std::cout << "Overall Voltage: " << FDSC::getVoltage(hstate.cell_vol) << "mV" << std::endl;
                std::cout << "Current: " << hstate.current << "mA" << std::endl;
                std::cout << "Cycles: " << hstate.cycle << std::endl;
                std::cout << "Temps BQ: " << hstate.BQ_NTC[0] << "°C, " << hstate.BQ_NTC[1] << "°C" << std::endl;
                std::cout << "Temps MCU: " << hstate.MCU_NTC[0] << "°C, " << hstate.MCU_NTC[1] << "°C" << std::endl;**/
                std::cout<<SetForeGRN << "------------------Foot Force info(No data In Go1 Pro): -------------------" << std::endl;
                std::cout<<"Footforce: ";
                for (auto data_f : hstate.footForce)
                {
                    std::cout<<float(data_f)<<" ";
                }
                std::cout<<std::endl;
                /***std::cout<<"FootforceEst: ";
                for (auto data_f : hstate.footForceEst)
                {
                    std::cout<<float(data_f)<<" ";
                }
                std::cout<<std::endl;
                
                //ros::spinOnce();  // 调用 ros::spinOnce()，处理任何订阅者的回调
                //loop_rate.sleep(); // 按照设定频率休眠

                std::cout<<SetForeGRN << "------------------Motion Info: -------------------" << std::endl;
                std::cout<<"Robotmode: "<<int(hstate.Robotmode)<<std::endl;// 
                std::cout<<"Progress: " <<hstate.progress<<std::endl;// TODO do not know this?
                std::cout<<"GaitType: "<<int(hstate.gaitType) <<std::endl;
                std::cout<<"footRaiseHeight: "<<hstate.footRaiseHeight <<std::endl;
                std::cout<<"BodyHeight: "<<hstate.bodyHeight <<std::endl;
                std::cout<<"Body Pos: [x,y,z] "<<hstate.position[0]<<" "<< hstate.position[1]<<" "<<hstate.position[2]<<std::endl;
                std::cout<<"Body Vel: [vx,vy,vz] "<<hstate.velocity[0]<<" "<< hstate.velocity[1]<<" "<<hstate.velocity[2]<<std::endl;
                std::cout<<"YawSpeed: "<<hstate.yawSpeed <<std::endl;
                std::cout<<"rangeObs: [o1,o2,o3,o4] "<<hstate.rangeObstacle[0]<<" "<< hstate.rangeObstacle[1]<<" "<<hstate.rangeObstacle[2]<<" "<<hstate.rangeObstacle[3]<<std::endl;
                for (int i=0;i<4;i++)
                {
                    std::cout<<"footpose2body: "<<"Leg: "<<i<<" "<<hstate.footPosition2Body[i].x<<" "<<hstate.footPosition2Body[i].y<<" "<<hstate.footPosition2Body[i].z<<std::endl;
                    std::cout<<"footspeed2body: "<<"Leg: "<<i<<" "<<hstate.footSpeed2Body[i].x<<" "<<hstate.footSpeed2Body[i].y<<" "<<hstate.footSpeed2Body[i].z<<std::endl;
                }
                std::cout << "=============================" << std::endl;**/
            }
            if (i == 0) {
                hcmd.robotmode_= FDSC::ROBOTModeHigh::FORCE_STAND;
            } else if (i == 11) {
                hcmd.robotmode_= FDSC::ROBOTModeHigh::DAMPING;
            }else {
                if (i % 2 == 0) {
                    hcmd.robotmode_= FDSC::ROBOTModeHigh::STAND_DOWN;
                    std::cout<<FOREBLU<<"In stand down mode "<<RESETTEXT<<std::endl;
                } else {
                    hcmd.robotmode_= FDSC::ROBOTModeHigh::STAND_UP;
                    std::cout<<SetForeGRN<<"In stand up mode "<<RESETTEXT<<std::endl;

                }
                cmd_bytes = hcmd.buildCmd(false);
                conn.send(cmd_bytes);
                if (i > max_iter) {
                    hcmd.robotmode_= FDSC::ROBOTModeHigh::DAMPING;
                    cmd_bytes = hcmd.buildCmd(false);
                    conn.send(cmd_bytes);
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            ++i;
        }

    }
    
    return 0;
}
