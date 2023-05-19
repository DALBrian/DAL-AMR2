#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <ros/ros.h>
#include <std_msgs/String.h>
using namespace std;

/**
 * @brief Create a ros node which is a ros publisher public info gets from TCP
 * @details For the scenario that get the KUKA's arm coordinate and publish to ROS.
 * @author Small Brian
 * @date 2023/03/17
*/
class TCPConnect{
public:
    TCPConnect();
    ~TCPConnect();
    void checkaccept();
    void tcprecv(std::stringstream &command);
    void tcpclose();
private:
    const char* host = "0.0.0.0";
    int port = 7000;
    int sock_fd, new_fd;
    socklen_t addrlen;
    struct sockaddr_in my_addr, client_addr;
    int status;
    int on = 1;
    bool updated;
};
TCPConnect::TCPConnect(){
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1){
        perror("Socket creat error");
        exit(1);
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on,  sizeof(int)) == -1){
        perror("Setsockopt error");
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
}
TCPConnect::~TCPConnect(){
    ROS_INFO("TCP server closing TCP connection");
    close(sock_fd);
}
void TCPConnect::checkaccept(){
    cout<<"Check connection status"<<endl;
    printf("Waiting for connection \n");
    addrlen = sizeof(client_addr);
        new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addrlen);
        printf("Connection established by %s:%d\n", inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));
            
    
}
void TCPConnect::tcprecv(std::stringstream &command){
    char indata[1024] = {0}, outdata[1024] = {"Message received"};
    int nbytes = recv(new_fd, indata, sizeof(indata), 0);
            if (nbytes <= 0){
                close(new_fd);
                printf("Client closed.\n");
                checkaccept();
            }
            printf("recv: %s\n", indata);
            command << indata;
            // sprintf(outdata, "echo %s", indata);
            send(new_fd, outdata, strlen(outdata), 0);


}

int main(int argc, char **argv){
    ros::init(argc, argv, "ros_tcp_server");
    ROS_INFO("This is ros_tcp_server");
    ros::NodeHandle nh_;
    ros::Publisher command_pub = nh_.advertise<std_msgs::String>("arm_command", 1000);
    ros::Rate loop_rate(1);
    TCPConnect tcp;
    tcp.checkaccept();
    while (ros::ok()){
        std_msgs::String msg;
        std::stringstream command; 
        tcp.tcprecv(command);
        cout<<"This is pointer command: "<<command.str()<<endl;
        msg.data = command.str();
        command_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}