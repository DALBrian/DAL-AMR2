#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <ros/ros.h>
#include <std_msgs/String.h>
using namespace std;
/**
 * @brief Create a ros node which is a ros subscriber receive ROS topic and transmit by TCP
 * @details For the scenario that another ros program wanna send a message to KUKA arm via TCP
 * @author Small Brian
 * @date 2023/03/17
*/
    
class TCPConnect{
public:
    TCPConnect();
    ~TCPConnect();
    void arm_commandCallback(const std_msgs::String::ConstPtr& msg);
private:
    const char* host = "192.168.1.43";
    int port = 7000;
    
    int sock_fd, new_fd;
    socklen_t addrlen;
    struct sockaddr_in my_addr, serv_name;
    int status;
    char indata[1024] = {0}, outdata[1024] = {0};
    int on = 1;
    bool updated;
};



TCPConnect::TCPConnect(){
    //TCP
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // server address
    serv_name.sin_family = AF_INET;
    inet_aton(host, &serv_name.sin_addr);
    serv_name.sin_port = htons(port);

    status = connect(sock_fd, (struct sockaddr *)&serv_name, sizeof(serv_name));
    if (status == -1) {
        perror("Connection error");
        exit(1);
    }

}
TCPConnect::~TCPConnect(){
    cout<<"Closing TCP connection"<<endl;
    ROS_INFO("TCP client closing TCP connection");
    close(sock_fd);
}
void TCPConnect::arm_commandCallback(const std_msgs::String::ConstPtr& msg){
    string received_messagel;
    received_messagel = msg->data.c_str();
    send(sock_fd, received_messagel.c_str(), strlen(received_messagel.c_str()), 0);
    cout<<"received message: "<<msg->data.c_str()<<endl;
    int nbytes = recv(sock_fd, indata, sizeof(indata), 0);
        if (nbytes <= 0) {
            close(sock_fd);
            printf("server closed connection.\n");
        }
}


int main(int argc, char **argv){
    // ros::Rate(1);
    ros::init(argc, argv, "ros_tcp_client");
    ros::NodeHandle nh;
    ROS_INFO("This is ros_tcp_client");
    // %Tag(SUBSCRIBER)%
    TCPConnect TCP;
    ros::Subscriber sub = nh.subscribe("arm_command", 1000, &TCPConnect::arm_commandCallback, &TCP); 
    //subscribe to chatter for using talker to test.
    // ros::Subscriber sub = nh.subscribe("arm_command", 1000, &TCPConnect::messageCallback, &TCP);
    ros::spin();
    return 0;
}