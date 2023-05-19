#include <iostream>
#include <chrono>
#include <stdio.h>
#include <unistd.h>
#include <cmath> 
#include <ros/ros.h>
#include <std_msgs/String.h>
using namespace std;

/*Using chrono as timer to calculate moving distance in pre-known speed scenario*/
#define REDUCTION_RATIO 0.1 //gear's ratio
#define BASE_SPEED 40 //RPM
#define DIAMETER 0.35 //diameter of wheel, meter

/*keycode, value = 0x"deci - 4"*/
#define KEYCODE_RIGHT 0x43
#define KEYCODE_LEFT 0x44
#define KEYCODE_UP 0x41 
#define KEYCODE_DOWN 0x42
/// ///

class command{
public:
    command();
    void publisher(std_msgs::String direction);
private:
    ros::NodeHandle nh_;
    ros::Publisher command_pub_;
};

command::command(){
    ROS_INFO("Starting ros command streaming");
    command_pub_ = nh_.advertise<std_msgs::String>("motor_command", 1);
}
void command::publisher(std_msgs::String direction){
    cout<<"Publishing message: "<<direction.data<<endl;
    command_pub_.publish(direction);
}

double time_to_dist(double time){
    return M_PI * DIAMETER * BASE_SPEED * time * REDUCTION_RATIO / 60;
}



/// ///
int main(int argc, char **argv){
    ros::init(argc, argv, "teleop_amr", ros::init_options::AnonymousName);
    command com;
    std_msgs::String direction;
    sleep(1);
    direction.data = "FWD";
    double time = 3.0;
    com.publisher(direction);
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    sleep(3);
    direction.data = "STOP";
    com.publisher(direction);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    direction.data = "BWD";
    com.publisher(direction);
    sleep(1);
    direction.data = "LEFT";
    com.publisher(direction);
    sleep(1);
    direction.data = "RIGHT";
    com.publisher(direction);
    sleep(1);
    direction.data = "CW";
    com.publisher(direction);
    sleep(1);
    direction.data = "CCW";
    com.publisher(direction);
    sleep(1);
    return 0;
}