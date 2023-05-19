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

/// @brief writing a TCP server (receiver)
class tcp_connect{
public:
    tcp_connect();
    void tcp_receive(std_msgs::String word);
private:
    //ROS variables
    ros::NodeHandle nh_;
    ros::Publisher tcp_pub_;
    //connection variable
    const char* host = "192.168.1.43";
    int port = 7000;
    
    int sock_fd, new_fd;
    socklen_t addrlen;
    struct sockaddr_in my_addr, client_addr;
    int status;
    char indata[1024] = {0}, outdata[1024] = {0};
    int on = 1;
};
tcp_connect::tcp_connect(){
    //ROS 
    ROS_INFO("Starting ros command streaming");
    tcp_pub_ = nh_.advertise<std_msgs::String>("tcp_send", 1);
    //TCP
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1){
        perror("socket creat error");
        exit(1);
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on,  sizeof(int)) == -1){
        perror("setsockopt error");
        exit(1);
    }
    my_addr.sin_family = AF_INET;
    inet_aton(host, &my_addr.sin_addr);
    my_addr.sin_port = htons(port);

    status = bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if (status == -1){
        perror("Binding error");
        exit(1);
    }
    status = listen(sock_fd, 5);
    if (status == -1) {
        perror("Listening error");
        exit(1);
    }

    printf("waiting for connection \n");
    addrlen = sizeof(client_addr);
}

void tcp_connect::tcp_receive(std_msgs::String word){




}

int main(int argc, char **argv){
    ros::init(argc, argv, "tcp_server");
    tcp_connect tcp_connect;
    tcp_connect.tcp_receive();
    ros::spin();


    return 0;
}