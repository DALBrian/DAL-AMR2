#include <iostream>
#include <stdio.h>
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>

using namespace std;
class odom_move{
public:
    odom_move();
    // ~odom_move();
    void callback(const nav_msgs::Odometry& msg);
private:
    ros::NodeHandle nh;
    ros::Publisher pub, pub2;
    ros::Subscriber sub;
    bool published = false;
    float dx, dy; // Increasement
    std::string frame;
};
odom_move::odom_move(){
    ROS_INFO("STARTING odom to move program");
    pub = nh.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1);
    pub2 = nh.advertise<geometry_msgs::PoseStamped>("move_base/current_goal", 5);
    ROS_INFO("Sleep for 3 secs");
    ros::Duration(3).sleep();
    if (ros::ok()){
        cout<<"Line 1"<<endl;
        sub = nh.subscribe("/scanmatch_odom", 1, &odom_move::callback, this);
        cout<<"Line 2"<<endl;
        nh.param<float>("dx", dx, 0.0);
        nh.param<float>("dy", dy, 0.0);
        nh.param<std::string>("frame", frame, "map");
        ROS_INFO("The increasement dx = %f, dy = %f", dx, dy);
    }
}
void odom_move::callback(const nav_msgs::Odometry& msg){
    ROS_INFO("STARTING callback");
    cout<<"Line 3"<<endl;
    //Get current position
    float position_x = msg.pose.pose.position.x;
    float position_y = msg.pose.pose.position.y;
    float position_z = msg.pose.pose.position.z;
    float quat_w = msg.pose.pose.orientation.w;
    float quat_x = msg.pose.pose.orientation.x;
    float quat_y = msg.pose.pose.orientation.y;
    float quat_z = msg.pose.pose.orientation.z;
    cout<<"Now position x: "<<position_x<<" y: "<<position_y<<" z: "<<position_z<<endl;
    cout<<"Quaternon w: "<<quat_w<<" x: "<<quat_x<<" y: "<<quat_y<<" z: "<<quat_z<<endl;
    //Publish to navigation goal
    geometry_msgs::PoseStamped pubmsg;
    pubmsg.header.frame_id = "map";
    pubmsg.pose.position.x = position_x + dx;
    pubmsg.pose.position.y = position_y + dy;
    pubmsg.pose.position.z = position_z;
    pubmsg.pose.orientation.w = quat_w;
    pubmsg.pose.orientation.x = quat_x;
    pubmsg.pose.orientation.y = quat_y;
    pubmsg.pose.orientation.z = quat_z;
    if (!published){
        ROS_INFO("Now publish increased position");
        pub.publish(pubmsg);
        pub2.publish(pubmsg);
        published = true;
    }
    
}
// odom_move::~odom_move(){ROS_INFO("DELETING ODOM_MOVE");}


int main(int argc, char** argv){
    ROS_INFO("SAY HIHI");
    ros::init(argc, argv, "odom_move");
    odom_move odom_move;
    ros::spin();
    return 0;
}