#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
/*
    @brief: Broadcast the tf(transformation infomation) of IMU and Lidar. Based on URDF file: DAL_AMR_URDF_v9
    @author: Small Brian
    @date: 2023/05/23
*/
int main(int argc, char **argv){
    ros::init(argc, argv, "imu_lidar_tf");
    ROS_INFO("Start running rosTF and broadcast IMU and LiDAR link");
    std::cout<<"Start running rosTF and broadcast IMU and LiDAR link"<<std::endl;
    ros::NodeHandle nh;
    ros::Rate r(100);
    tf::TransformBroadcaster bc;
    while(nh.ok()){
        bc.sendTransform(tf::StampedTransform(
            tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.818000000000031, -0.236989872766201, 0.485510000000965)),
            ros::Time::now(), "base_link", "IMU_link"));
        bc.sendTransform(tf::StampedTransform(
            tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(-0.78509, -0.25886, 0.05725)),
            ros::Time::now(), "base_link", "IMU_link2"));
            /*If LiDAR installed 90 degree cw from x-positive-axis*/
        // bc.sendTransform(tf::StampedTransform(
        //     tf::Transform(tf::Quaternion(0, 0, 1, 1), tf::Vector3(0.76499999485128, -0.739989872766514, 0.567510000000008)),
        //     ros::Time::now(), "base_link", "laser_link"));
        /*If LiDAR installed along x-positive-axis*/
        bc.sendTransform(tf::StampedTransform(
            tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.76499999485128, -0.739989872766514, 0.567510000000008)),
            ros::Time::now(), "base_link", "laser_link"));
        bc.sendTransform(tf::StampedTransform(
            tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(-0.765000005148736, -0.733989872766535, 0.567509999999994)),
            ros::Time::now(), "base_link", "laser_link2"));
        r.sleep();
    }
    return 0;
}