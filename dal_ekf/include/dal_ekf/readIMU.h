#include <iostream>
#include <string>
#include <vector>
class pf_imu{
public:
    pf_imu();
    ~pf_imu();
    void imu_callback();
private:
    std::string imu_IP = "192.168.1.149";
    std::string imu_port = "502";
};

class ros_imu{
public:
    ros_imu();
    ~ros_imu();
    void imu_callback();
private:
    std::string imu_port = "/dev/ttyUSB1";
};