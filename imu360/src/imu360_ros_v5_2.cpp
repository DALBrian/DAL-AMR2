#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "/usr/local/include/modbus/modbus.h"
// #include <modbus.h>
#include <chrono>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <algorithm>
#define g 9.81
/**
 * @brief P+F IMU integration with ICP-DAS GW-7328D via Modbus TCP/IP
 * @details First do 7328D I/O mapping, then read corresponding IMU data from 7328D
 * @author Small Brian
 * @date 2023/06/21
 * TODO: Haven't tested ROS yet; Individual function tested successful.
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
    private:
        ros::NodeHandle nh_;
        ros::Publisher imu_pub_;
        sensor_msgs::Imu imu_msg_;
        modbus_t* mb;
        string IP, port;
        int start_address = 0;
        int num_registers = 44;
        uint16_t msg[22];
        bool isShow = false;
        // std::vector <uint16_t> msg;
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
    imu_pub_ = nh_.advertise<sensor_msgs::Imu>("imu2", 1);
    imu_msg_.header.frame_id = "imu2_link";
}
modbus_read::~modbus_read(){
    modbus_close(mb);
    modbus_free(mb);
    ROS_INFO("IMU Modbus connection closed.");
}
void modbus_read::read_input(){
    
    int rc = modbus_read_input_registers(mb, start_address, num_registers, msg);
    if (rc == -1){
        std::cerr << "Failed to read input" << modbus_strerror(errno) << "\n";
    }

    for(int i = 0; i < sizeof(msg); i++){cout<<"Recv msg: "<<i<<": "<<msg[i]<<endl;}
    parse2();
}
void modbus_read::test(){
    cout << "sizeofmsg" << sizeof(msg) << endl;
    for (int i = 0; i < sizeof(msg); i++){
        cout << "Reading: " << i << ": " << msg[i] << endl;
        // printf("Reading: %d: %d\n", i, msg[i]);
        // printf("Reading: %d: %2x\n", i, msg[i]);
    }
}
void modbus_read::parse(){
    for (int i = 0; i < 41; i++){
        switch (i){
            case (25):
                // memcpy(&lin_acc_x, &msg[i], sizeof(lin_acc_x));
                lin_acc_x = (msg[i] / 1.0);
                imu_msg_.linear_acceleration.x = lin_acc_x;
                break;
            case (26):
                lin_acc_y = (msg[i] / 1.0);
                imu_msg_.linear_acceleration.y = lin_acc_y;
                break;
            case (27):
                lin_acc_z = (msg[i] / 1.0);
                imu_msg_.linear_acceleration.z = lin_acc_z;
                break;
            // case (15):
            //     ang_vel_x = msg[i] / 100.0 / 180.0 * M_PI;
                
            //     imu_msg_.angular_velocity.x = ang_vel_x;
            //     break;
            // case (11):
            //     ang_vel_y = msg[i] / 100.0 / 180.0 * M_PI;
                
            //     imu_msg_.angular_velocity.y = ang_vel_y;
            //     break;
            // case (12):
            //     ang_vel_z = msg[i] / 100.0 / 180.0 * M_PI;
                
            //     imu_msg_.angular_velocity.z = ang_vel_z;
            //     break;
            // case (35):
            //     quat_x = msg[i] / 1000.0;
                
            //     imu_msg_.orientation.x = quat_x;
            //     break;
            // case (36):
            //     quat_y = msg[i] / 1000.0;
                
            //     imu_msg_.orientation.y = quat_y;
            //     break;
            // case (37):
            //     quat_z = msg[i] / 1000.0;
                
            //     imu_msg_.orientation.z = quat_z;
            //     break;
            // case (38):
            //     quat_w = msg[i] / 1000.0;
                
            //     imu_msg_.orientation.w = quat_w;
            //     break;
            case(40):
                cout<<"temp1: "<< msg[i] / 10.0 << endl;
                cout<<"temp2: "<< msg[i+1] / 10.0 << endl;
        }
    }
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
void modbus_read::parse2(){
    cout<<"temp1: "<< msg[40]<< endl;
    cout<<"temp2: "<< msg[41]<< endl;
    lin_acc_x = msg[25];
    imu_msg_.linear_acceleration.x = msg[25];

    lin_acc_y = msg[26];
    imu_msg_.linear_acceleration.y = msg[26];

    lin_acc_z = msg[27];
    imu_msg_.linear_acceleration.z = msg[27];

    ang_vel_x = msg[10];
    imu_msg_.angular_velocity.x = msg[10];

    ang_vel_y = msg[11];
    imu_msg_.angular_velocity.y = msg[11];

    ang_vel_z = msg[12];
    imu_msg_.angular_velocity.z = msg[12];

    quat_x = msg[35];
    imu_msg_.orientation.x = msg[35];

    quat_y = msg[36];
    imu_msg_.orientation.y = msg[36];

    quat_z = msg[37];
    imu_msg_.orientation.z = msg[37];
    quat_w = msg[38];
    imu_msg_.orientation.w = msg[38];

    
    
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