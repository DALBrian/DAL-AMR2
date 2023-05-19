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

#include <tcp_ros/arm.h>
using namespace std;

/**
 * @author Small Brian
 * @date 2023/04/20
 **/
class TCPConnect{
public:
    TCPConnect();
    ~TCPConnect();
    void checkaccept();
    void tcpclose();
    void tcprecv(std::stringstream& command);
    void dataparse(std::string&i nput);
    void dataparse(const tcp_ros::arm& arm);
    Matrix4d dataparse(const tcp_ros::arm& arm);
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
    string input;
    int nbytes = recv(new_fd, indata, sizeof(indata), 0);
            if (nbytes <= 0){
                close(new_fd);
                printf("Client closed.\n");
                checkaccept();
            }else{
                // printf("recv: %s\n", indata);
                command << indata;
                input = command.str();
                size_t check = input.find("joint", 0);
                if (check != -1) {dataparse(input);}
                sprintf(outdata, "echo %s", indata);
                send(new_fd, outdata, strlen(outdata), 0);

            } 
}
void TCPConnect::dataparse(std::string &msg){
    cout<<"msg: "<<msg<<endl;
    std::string j1 = "<joint1>"; //start mark
    std::string j11 = "</joint1>"; // end mark
    std::string a1, a2, a3, a4, a5, a6;
    size_t p1, p2, p3, p4, p5, p6;
    size_t p11, p22, p33, p44, p55, p66;
    size_t l1 = j1.length();
    p1 = msg.find("<joint1>", 0); p11 = msg.find("</joint1>", 0);
    p2 = msg.find("<joint2>", 0); p22 = msg.find("</joint2>", 0);
    p3 = msg.find("<joint3>", 0); p33 = msg.find("</joint3>", 0);
    p4 = msg.find("<joint4>", 0); p44 = msg.find("</joint4>", 0);
    p5 = msg.find("<joint5>", 0); p55 = msg.find("</joint5>", 0);
    p6 = msg.find("<joint6>", 0); p66 = msg.find("</joint6>", 0);
    a1 = msg.substr(p1 + l1, p11 - p1 - l1);
    a2 = msg.substr(p2 + l1, p22 - p2 - l1);
    a3 = msg.substr(p3 + l1, p33 - p3 - l1);
    a4 = msg.substr(p4 + l1, p44 - p4 - l1);
    a5 = msg.substr(p5 + l1, p55 - p5 - l1);
    a6 = msg.substr(p6 + l1, p66 - p6 - l1);
    cout<<"a1: "<<a1<<"a2: "<<a2<<"a3: "<<a3<<"a4: "<<a4<<"a5: "<<a5<<"a6: "<<a6<<endl;
    
    size_t px, py, pz, pa, pb, pc;
    size_t pxx, pyy, pzz, paa, pbb, pcc;
    size_t lpx, lpxx;
    std::string x, y, z, a, b, c;
    std::string PX = "<PX>"; lpx = PX.length();

    px = msg.find("<PX>", 0); pxx = msg.find("</PX>", 0);
    py = msg.find("<PY>", 0); pyy = msg.find("</PY>", 0);
    pz = msg.find("<PZ>", 0); pzz = msg.find("</PZ>", 0);
    pa = msg.find("<PA>", 0); paa = msg.find("</PA>", 0);
    pb = msg.find("<PB>", 0); pbb = msg.find("</PB>", 0);
    pc = msg.find("<PC>", 0); pcc = msg.find("</PC>", 0);

    x = msg.substr(px + lpx, pxx - px - lpx);
    y = msg.substr(py + lpx, pyy - py - lpx);
    z = msg.substr(pz + lpx, pzz - pz - lpx);
    a = msg.substr(pa + lpx, paa - pa - lpx);
    b = msg.substr(pb + lpx, pbb - pb - lpx);
    c = msg.substr(pc + lpx, pcc - pc - lpx);
    cout<<"x: "<<x<<"y: "<<y<<"z: "<<z<<"a: "<<a<<"b: "<<b<<"c: "<<c<<endl;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "ros_tcp_server");
    ROS_INFO("This is ros_tcp_server");
    ros::NodeHandle nh_;
    ros::Publisher command_pub = nh_.advertise<std_msgs::String>("arm_command", 1000);
    ros::Rate loop_rate(1);
    TCPConnect tcp;
    tcp.checkaccept();
    double number[12] {0};
    while (ros::ok()){
        std_msgs::String msg;
        std::stringstream command; 
        tcp_ros::arm arm_coord;
        tcp.tcprecv(command);
        // cout<<"This is pointer command: "<<command.str()<<endl;
        msg.data = command.str();
        command_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}