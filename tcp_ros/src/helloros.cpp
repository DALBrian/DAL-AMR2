#include <iostream>
#include <vector>
#include <ros/ros.h>
using namespace std;

int main(int argc, char **argv){
    ros::init(argc, argv, "HelloROSNode");
    cout<<"Welcome back, ROS"<<endl;
    // float x = "say hi hi ";
    ROS_INFO("Welcome back, ROS");
    ROS_WARN("Say hi hi ");
    ros::NodeHandle nh;
    ros::Rate loop_rate(0.5);
    std::string test;
    nh.param("test", test, std::string("Fail!"));
    cout<<"ROS param server test: "<<test<<endl;
    return 0;
}