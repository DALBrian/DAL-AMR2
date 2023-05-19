#include <iostream>
#include <stdio.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

using namespace std;
class kinematic{
public:
    void forward(geometry_msgs::Twist &input);
    void stop(geometry_msgs::Twist &input);
private:
    int i;
};
void kinematic::forward(geometry_msgs::Twist &input){
    input.linear.x = 0.5;
    input.linear.y = 0.0;
    input.angular.z = 0.0;
}
void kinematic::stop(geometry_msgs::Twist &input){
    input.linear.x = 0.0;
    input.linear.y = 0.0;
    input.angular.z = 0.0;
}
int main(int argc, char** argv){
    ROS_INFO("STARTING motor countdown");
    ros::init(argc, argv, "countdown_control_command");
    ros::Time::init();
    ros::NodeHandle nh;
    ros::Publisher move_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5);
    ros::Rate loop_rate(10);
    geometry_msgs::Twist msg;
    kinematic kin;
    msg.linear.x = 0.0;  msg.linear.y = 0.0;  msg.linear.z = 0.0; 
    msg.angular.x = 0.0;  msg.angular.y = 0.0;  msg.angular.z = 0.0; 
    
    if (ros::ok()){
        kin.stop(msg); // confirm stop
        move_pub.publish(msg); 
        
        ros::Duration d(5.0), period;   
        kin.forward(msg); 
        cout<<"msg: "<<msg<<endl;
        ros::Time begin = ros::Time::now();
        period = ros::Time::now() - begin;
        cout<<"Running period: "<<period.toSec()<<endl;
        move_pub.publish(msg);
        if (period.toSec() >= 5){
            kin.stop(msg);
            cout<<"msg: "<<msg<<endl;
            move_pub.publish(msg);
            period = ros::Time::now() - begin;
            cout<<"Running period: "<<period.toSec()<<endl;
        }





    }
    return 0;
}