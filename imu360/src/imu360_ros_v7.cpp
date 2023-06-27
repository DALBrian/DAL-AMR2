#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "/usr/local/include/modbus/modbus.h"
// #include <modbus.h>
#include <chrono>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <imu360/imu2.h>
#include <algorithm>
#define g 9.81
/**
 * @brief P+F IMU integration with ICP-DAS GW-7328D via Modbus TCP/IP
 * @details Add hex and 2's complement transformation. Previous version is proved to read data correctly, but GW-7328D cannot perform 2's complement itself.
 * @author Small Brian
 * @date 2023/06/27
 *
 **/
using namespace std;
class modbus_read{
    public:
        modbus_read(string IP, string port);
        ~modbus_read();
        void read_input();
        void parse();
        void parse2();
        void test();
        void twocomplement();
    private:
        ros::NodeHandle nh_;
        ros::Publisher imu_pub_;
        ros::Publisher imu_pub2_;
        sensor_msgs::Imu imu_msg_;
        imu360::imu2 imu_msg2_;
        modbus_t* mb;
        string IP, port;
        int start_address = 0;
        int num_registers = 44;
        uint16_t msg[44];
        uint16_t msg_twos[44];
        bool isShow = false;
        double lin_acc_x, lin_acc_y, lin_acc_z;
        double ang_vel_x, ang_vel_y, ang_vel_z;
        double quat_x, quat_y, quat_z, quat_w;
        double temp1, temp2;
        char temp[1] = {0};
        
};
modbus_read::modbus_read(string IP, string port){
    mb = modbus_new_tcp(IP.c_str(), stoi(port));
    if (mb == nullptr){
        std::cerr << "Failed to create content." << std::endl;
        ROS_WARN("Failed to create content.");
    }
    if (modbus_connect(mb) == -1){
        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
        ROS_WARN("IMU Modbus connection failed.");
    }
    modbus_set_slave(mb, 1);
    ROS_INFO("IMU Modbus connection established.");
    // imu_pub_ = nh_.advertise<sensor_msgs::Imu>("imu2", 1);
    imu_pub2_ = nh_.advertise<imu360::imu2>("Imu2", 1);
    imu_msg_.header.frame_id = "IMU_link2";
}
modbus_read::~modbus_read(){
    modbus_close(mb);
    modbus_free(mb);
    ROS_INFO("IMU Modbus connection closed.");
}
void modbus_read::read_input(){
    
    int rc = modbus_read_input_registers(mb, start_address, num_registers, msg);
    if (rc == -1){
        std::cerr << "Failed to read input: " << modbus_strerror(errno) << "\n";
    }
    twocomplement();
    parse();
}
void modbus_read::parse(){
    
    imu_msg2_.header = "imu2_link";
    imu_msg2_.accX = msg_twos[5] / 1000.0 * g;
    imu_msg2_.accY = msg_twos[6] / 1000.0 * g;
    imu_msg2_.accZ = msg_twos[7] / 1000.0 * g;
    imu_msg2_.GyroX = msg_twos[10] / 100.0;
    imu_msg2_.GyroY = msg_twos[11] / 100.0;
    imu_msg2_.GyroZ = msg_twos[12] / 100.0;
    imu_msg2_.RotX = msg_twos[15];
    imu_msg2_.RotY = msg_twos[16];
    imu_msg2_.RotZ = msg_twos[17];
    imu_msg2_.GavX = msg_twos[20] / 1000.0;
    imu_msg2_.GavY = msg_twos[21] / 1000.0;
    imu_msg2_.GavZ = msg_twos[22] / 1000.0;
    imu_msg2_.LinX = msg_twos[25] / 1000.0;
    imu_msg2_.LinY = msg_twos[26] / 1000.0;
    imu_msg2_.LinZ = msg_twos[27] / 1000.0;
    imu_msg2_.Roll = msg_twos[30] / 100.0;
    imu_msg2_.Pitch = msg_twos[31] / 100.0;
    imu_msg2_.Yaw = msg_twos[32] / 100.0;
    imu_msg2_.X = msg_twos[35] / 1000.0;
    imu_msg2_.Y = msg_twos[36] / 1000.0;
    imu_msg2_.Z = msg_twos[37] / 1000.0;
    imu_msg2_.W = msg_twos[38] / 1000.0;
    imu_msg2_.temp1 = msg_twos[40] / 10.0;
    imu_msg2_.temp2 = msg_twos[41] / 10.0;

    imu_pub2_.publish(imu_msg2_);
    // cout<<"published"<<endl;
}
void modbus_read::parse2(){

    printf("say hi \n");
    for (int i = 0; i < sizeof(msg); i++){
        cout << "Before: " << i << ": " << msg[i] << endl;
        // cout << msg[i] << endl;
        }
    
    lin_acc_x = msg[25];
    imu_msg_.linear_acceleration.x = lin_acc_x;

    lin_acc_y = msg[26];
    imu_msg_.linear_acceleration.y = lin_acc_y;

    lin_acc_z = msg[27];
    imu_msg_.linear_acceleration.z = lin_acc_z;

    ang_vel_x = msg[10];
    imu_msg_.angular_velocity.x = ang_vel_x;

    // ang_vel_y = msg[11];
    // imu_msg_.angular_velocity.y = ang_vel_y;

    ang_vel_z = msg[12];
    imu_msg_.angular_velocity.z = ang_vel_z;

    quat_x = msg[35];
    imu_msg_.orientation.x = quat_x;

    quat_y = msg[36];
    imu_msg_.orientation.y = quat_y;

    quat_z = msg[37];
    imu_msg_.orientation.z = quat_z;

    quat_w = msg[38];
    imu_msg_.orientation.w = quat_w;
    for (int i = 35; i < sizeof(msg); i++){cout << "After: " << i << ": " << msg[i] << endl;}
    // memcpy(&temp1, &msg+40, 2);
    // cout<< "After memcpy temp1: " << temp1 << endl;
    // memcpy(&temp2, &msg+41, 2);
    // cout<< "After memcpy temp2: " << temp2 << endl;
    cout<<"temp1: "<< msg[40]<< endl;
    cout<<"temp2: "<< msg[41]<< endl;
    
    if (isShow == true){
        std::cout << "lin_acc_x: " << lin_acc_x << "\n";
        std::cout << "lin_acc_y: " << lin_acc_y << "\n";
        std::cout << "lin_acc_z: " << lin_acc_z << "\n";
        std::cout << "ang_vel_x: " << ang_vel_x << "\n";
        std::cout << "ang_vel_y: " << ang_vel_y << "\n";
        std::cout << "ang_vel_z: " << ang_vel_z << "\n";
        std::cout << "quat_x: " << quat_x << "\n";
        std::cout << "quat_y: " << quat_y << "\n";
        std::cout << "quat_z: " << quat_z << "\n";
        std::cout << "quat_w: " << quat_w << "\n";
        cout<<"published"<<endl;
    }
    imu_pub_.publish(imu_msg_);
    
}
void modbus_read::twocomplement(){
    printf("\n");
    for (int i = 0; i < 44; i++){
        msg_twos[i] = 0;
        if(msg[i] > 32767){
            msg_twos[i] = (~msg[i]) + 1;
        }else{msg_twos[i] = msg[i];}
        msg[i] = 0.0;
        cout << "Msg before 2's complement " << std::dec <<  i << ": " << 
        std::dec << msg[i] << endl;
        // cout << "Msg after 2's complement " << std::dec << i << ": " << 
        // std::hex << msg_twos[i] << endl;
        cout << "Msg after 2's complement " << std::dec << i << ": " << 
        std::dec << msg_twos[i] << endl;
    }
}
int main(int argc, char** argv){
    ros::init(argc, argv, "modbus_read");
    ROS_INFO("modbus_read node started.");
    ros::NodeHandle nh;
    std::string IP, port;
    nh.param<std::string>("IP", IP, "192.168.1.149");
    nh.param<std::string>("port", port, "502");
    modbus_read modbus(IP, port);
    while(ros::ok()){
        modbus.read_input();
        ros::spinOnce();
        // ros::spin();
    }
    return 0;
}