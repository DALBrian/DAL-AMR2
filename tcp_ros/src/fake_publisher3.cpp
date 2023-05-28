#include "ros/ros.h"
#include <tcp_ros/ArmComm.h>
#include <iostream>
using namespace std;
/*
    @brief: arm moving data in (j1 ,j2, j3, j4, j5, j6) format
    @author: Small Brian
    @date: 2023/05/25*/

int main(int argc, char **argv){
    ros::init(argc, argv, "FakeArmPublisher");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<tcp_ros::ArmComm>("ArmCommand", 1);
    
    ros::Rate loop_rate(0.05);
    int index = 0, count = 0;
    while(ros::ok()){
        tcp_ros::ArmComm msg;
        // msg.x = (double)count;
        // msg.y = (double)count;
        // msg.z = (double)count;
        ros::Duration(1).sleep(); // Sleep before publishing, otherwise arm will miss the first command.
        if (index == 0){
            msg.stepmode = "13"; // fixed to 8 to move axis 4, in case of danger
            msg.j1 = 0.02; msg.j2 = -110.0; msg.j3 = 143.0;
            msg.j4 = 0.0; msg.j5 = 105.8; msg.j6 = 0.0;
            msg.isOut = -10;
            cout<<"Sending1"<<endl;

        }else if(index == 1){
            msg.stepmode = "13"; // fixed to 8 to move axis 4, in case of danger
            msg.j1 = 0.02; msg.j2 = -113.88; msg.j3 = 130.34;
            msg.j4 = -0.01; msg.j5 = 122.79; msg.j6 = 0.0;
            msg.isOut = -10;
            cout<<"Sending2"<<endl;
        }else if (index == 2){
            msg.stepmode = "13"; // fixed to 8 to move axis 4, in case of danger
            msg.j1 = 0.03; msg.j2 = -102.24; msg.j3 = 106.58;
            msg.j4 =  0.06; msg.j5 = 82.95; msg.j6 = 0.03;
            msg.isOut = -10;
            cout<<"Sending3"<<endl;
        }else{
            msg.isOut = 10;
            cout<<"Sending4"<<endl;break;
        }

        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        index++;
    }
    return 0;
}