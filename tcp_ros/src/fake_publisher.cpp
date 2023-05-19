#include "ros/ros.h"
#include <tcp_ros/arm.h>
#include <tcp_ros/ArmComm.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv){
    ros::init(argc, argv, "FakeArmPublisher");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<tcp_ros::ArmComm>("ArmCommand", 10);
    
    ros::Rate loop_rate(0.5);
    int count = 0;
    while(ros::ok()){
        tcp_ros::ArmComm msg;
        // msg.x = (double)count;
        // msg.y = (double)count;
        // msg.z = (double)count;
        
        msg.stepmode = "8"; // fixed to 12 to move axis 6, in case of danger
        msg.x = 540; msg.y = 14; msg.z = 702;
        msg.a = 90; msg.b = 0; msg.c = 180;
        msg.isOut = -10;
        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        count++;
    }
    return 0;
}