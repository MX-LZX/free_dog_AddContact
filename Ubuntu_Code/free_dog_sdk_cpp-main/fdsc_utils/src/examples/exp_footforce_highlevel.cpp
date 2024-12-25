#include <ros/ros.h>
#include <fdsc_utils/free_dog_sdk_h.hpp>

int main(int argc, char** argv) {
    
    ros::init(argc, argv, "highstate_node");
    ros::NodeHandle nh;
    FDSC::highState hstate;
    //ros::Rate loop_rate(100); // 设置循环频率为10Hz，避免 CPU 占用过高
    ros::AsyncSpinner spinner(4);
    spinner.start();

    
    while (ros::ok()) {
        std::cout<<SetForeGRN << "------------------Foot Force info: -------------------" << std::endl;
        std::cout<<"Footforce: ";
        for (auto data_f : hstate.footForce)
        {
            std::cout<<float(data_f)<<" ";
        }
        std::cout<<std::endl;
                
    }
    
    return 0;
}
