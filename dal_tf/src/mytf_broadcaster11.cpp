#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv){
    ros::init(argc, argv, "dal_amr_tf_IMU_Laser_publisher");
    ros::NodeHandle nh;
    ros::Rate r(100);
    tf::TransformBroadcaster bc;
    while(nh.ok()){
        bc.sendTransform(tf::StampedTransform(
            tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(818, -236.99999, 485.51)),
            ros::Time::now(), "base_link", "IMU_link"));
        bc.sendTransform(tf::StampedTransform(
            tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(765, -739.99999, 487.51)),
            ros::Time::now(), "base_link", "laser_link"));
        r.sleep();
    }
    return 0;
}