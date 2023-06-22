#include "ros/ros.h"
#include <tcp_ros/ArmComm.h>
#include <iostream>
using namespace std;
/*Publishing arm moving data in (X, Y, Z, A, B, C) format
    @author: Small Brian
    @date: 2023/05/25
*/

int main(int argc, char **argv){
    ros::init(argc, argv, "FakeArmPublisher");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<tcp_ros::ArmComm>("ArmCommand", 10);
    
    ros::Rate loop_rate(0.08);
    int index = 0, count = 0;
    while(ros::ok()){
        tcp_ros::ArmComm msg;
        // msg.x = (double)count;
        // msg.y = (double)count;
        // msg.z = (double)count;
        ros::Duration(1).sleep(); // Sleep before publishing, otherwise arm will miss the first command.
        if (index == 0){
            msg.stepmode = "0"; // fixed to 8 to move axis 4, in case of danger
            msg.x = 815.0; msg.y = -0.7313 - 200; msg.z = 1111.0;
            msg.a = 179.985; msg.b = -0.040; msg.c = 179.99732;
            msg.j1 = 0.02; msg.j2 = -110.88; msg.j3 = 143.0; 
            msg.j4 = 0.0; msg.j5 = 105.8; msg.j6 = 0.01; 
            msg.isOut = -10;
            cout<<"Sending1"<<endl;
        }else if(index == 1){
            msg.stepmode = "0"; // fixed to 8 to move axis 4, in case of danger
            msg.x = 815.0; msg.y = -0.7313 - 400; msg.z = 1111.0;
            msg.a = 179.985; msg.b = -0.040; msg.c = 179.99732;
            msg.j1 = 0.02; msg.j2 = -113.88; msg.j3 = 130.34; 
            msg.j4 = -0.01; msg.j5 = 105.79; msg.j6 = 0.00; 
            msg.isOut = -10;
            cout<<"Sending2"<<endl;
        }else if (index == 2){
            msg.stepmode = "0"; // fixed to 8 to move axis 4, in case of danger
            msg.x = 815.0; msg.y = -0.7313 - 600; msg.z = 1111.0;
            msg.a = 179.985; msg.b = -0.040; msg.c = 179.99732;
            msg.j1 = 0.03; msg.j2 = -102.24; msg.j3 = 106.58;
            msg.j4 = 0.06; msg.j5 = 82.95; msg.j6 = 0.03; 
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