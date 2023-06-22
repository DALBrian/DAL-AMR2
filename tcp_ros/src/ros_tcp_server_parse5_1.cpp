#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <thread>
#include <sstream>
#include <ros/ros.h>
#include <std_msgs/String.h>
/*Customized msg type.*/
#include <tcp_ros/arm.h> 
#include <tcp_ros/ArmComm.h>
#include <tcp_ros/ArmCoord.h>
using namespace std;
/**
 * @brief Create a ros node which is a ros publisher public info gets from TCP. 
 * @details Add axis control and arm axis limitation check.
 * @author Small Brian
 * @date 2023/05/27
 * TODO: Software arm limitation check befroe send; if isFinish false, save it and send next time, don't skip
 **/
class TCPConnect{
public:
    TCPConnect(string ip, int port);
    ~TCPConnect();
    void checkaccept();
    void tcpclose();
    void tcprecv();
    void sendtoArm(const tcp_ros::ArmComm& msg);
    // void tcpsend(string &msg);
    void dataparse();
    void start();
    void checkAngle(vector<double> &angle);
private:
    /*For socket(TCP/IP)*/
    int sock_fd, new_fd;
    socklen_t addrlen;
    struct sockaddr_in my_addr, client_addr;
    int status;
    int on = 1;
    size_t index = 0;
    /*For ROS*/
    ros::NodeHandle nh_;
    ros::Subscriber ArmSub;
    ros::Publisher ArmPub;
    bool isUpdate = false; 
    bool isInitialized = false; //This program is initialized or not
    bool isPublished = false; //Arm callback data is published to ROS topic or not
    bool isValid = false; //Arm callback data is valid or not
    bool isOpen = false; //Arm connection in this PC is open or not
    bool isFinish = true; //Arm movement is finish or not
    // stringstream received_msg;
    std::string received_msg_;
    vector<double> armmsg;
    tcp_ros::ArmCoord arm_coord;
    std::string subtopic, pubtopic;
    vector<double> axisAng;
}; 
TCPConnect::TCPConnect(string ip, int port){
    /*TCP connection*/
    ROS_INFO("TCP server info: %s, %i .", ip.c_str(), port);
    const char* host = ip.c_str();
    // const char* host = "127.0.0.1";
    int port_ = port;
    my_addr.sin_family = AF_INET;
    inet_aton(host, &my_addr.sin_addr);
    my_addr.sin_port = htons(port_);
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1){
        perror("Socket creat error");
        isOpen = false;
        exit(1);
    }
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on,  sizeof(int)) == -1){
        perror("Setsockopt error");
        isOpen = false;
        exit(1);
    }
    if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1){
        perror("Binding error");
        isOpen = false;
        exit(1);
    }
    if (listen(sock_fd, 5) == -1) {
        perror("Listening error");
        isOpen = false;
        exit(1);
    }
    addrlen = sizeof(client_addr);
    /*ROS*/
    nh_.param("subtopic", subtopic, std::string("ArmCommand"));
    nh_.param("pubtopic", pubtopic, std::string("ArmCoord"));
    ArmPub = nh_.advertise<tcp_ros::ArmCoord>(pubtopic, 10);
}
TCPConnect::~TCPConnect(){
    ROS_INFO("Closing TCP Connection to ARM");
    if (isOpen == true){
        string isout = "TRUE";
        string packet = "<Sensor><Status><IsOut>" + isout + "</IsOut></Status><StepMode>" + "0" + "</StepMode><SX>" + "0" + 
        "</SX><SY>" + "0" + "</SY><SZ>" + "0" + "</SZ><SA>" + "0" + "</SA><SB>" + "0"+ "</SB><SC>" + "0" + "</SC></Sensor>";
        send(new_fd, packet.c_str(), strlen(packet.c_str()), 0);
    }
    isOpen = false; 
    close(sock_fd);}

void TCPConnect::tcprecv(){
    while (true) {
        printf("Waiting for connection \n");
        new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addrlen);
        
        printf("Connected by %s:%d\n", inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));
        while (true) {
            char indata[1024] = {};
            isOpen = true;
            int nbytes = recv(new_fd, indata, sizeof(indata), 0);
            // cout<<"Received msg: "<<indata<<endl; //For debug
            if (nbytes <= 0){
                close(new_fd);
                cout<<"Received msg size: "<<nbytes<<endl; //For debug
                isOpen = false;
                printf("client closed.\n");
                break; // break this while(the inner while), back to waiting for accept.
            }else{
                std::stringstream received_msg;
                received_msg << indata;
                received_msg_ = received_msg.str();
                size_t check = received_msg_.find("joint", 0);
                if(check != -1){
                    //ros publish infomation and change mutex status
                    dataparse();
                    if(isValid && !isPublished){
                        // ROS_INFO("PUBLISHING MSG");
                        arm_coord.a1 = armmsg[0]; arm_coord.a2 = armmsg[1]; arm_coord.a3 = armmsg[2]; 
                        arm_coord.a4 = armmsg[3]; arm_coord.a5 = armmsg[4]; arm_coord.a6 = armmsg[5]; 
                        arm_coord.x = armmsg[6]; arm_coord.y = armmsg[7]; arm_coord.z = armmsg[8]; 
                        arm_coord.a = armmsg[9]; arm_coord.b = armmsg[10]; arm_coord.c = armmsg[11]; 
                        arm_coord.isFinish = 10.0;
                        ArmPub.publish(arm_coord);
                        armmsg.clear();
                        isPublished = true;
                        
                    }
                }
            }
        }
    }
}
void TCPConnect::dataparse(){
    // cout<<"  message from arm: "<<input<<endl;
    bool check_sum1, check_sum2;
    std::string j1 = "<joint1>"; //start mark
    std::string j11 = "</joint1>"; // end mark
    std::string a1, a2, a3, a4, a5, a6;
    size_t p1, p2, p3, p4, p5, p6;
    size_t p11, p22, p33, p44, p55, p66;
    size_t l1 = j1.length();
    p1 = received_msg_.find("<joint1>", 0); p11 = received_msg_.find("</joint1>", 0);
    p2 = received_msg_.find("<joint2>", 0); p22 = received_msg_.find("</joint2>", 0);
    p3 = received_msg_.find("<joint3>", 0); p33 = received_msg_.find("</joint3>", 0);
    p4 = received_msg_.find("<joint4>", 0); p44 = received_msg_.find("</joint4>", 0);
    p5 = received_msg_.find("<joint5>", 0); p55 = received_msg_.find("</joint5>", 0);
    p6 = received_msg_.find("<joint6>", 0); p66 = received_msg_.find("</joint6>", 0);
    if (p1 != -1 && p66 != -1){
        // ROS_WARN("Check1 pass!");
        a1 = received_msg_.substr(p1 + l1, p11 - p1 - l1);
        a2 = received_msg_.substr(p2 + l1, p22 - p2 - l1);
        a3 = received_msg_.substr(p3 + l1, p33 - p3 - l1);
        a4 = received_msg_.substr(p4 + l1, p44 - p4 - l1);
        a5 = received_msg_.substr(p5 + l1, p55 - p5 - l1);
        a6 = received_msg_.substr(p6 + l1, p66 - p6 - l1);
        // cout<<"a1: "<<a1<<"a2: "<<a2<<"a3: "<<a3<<"a4: "<<a4<<"a5: "<<a5<<"a6: "<<a6<<endl;//For debug
        check_sum1 = true;
    }else{check_sum1 = false;}
    
    size_t px, py, pz, pa, pb, pc;
    size_t pxx, pyy, pzz, paa, pbb, pcc, pisfinish;
    size_t lpx, lpisfinish, pisfinishh;
    std::string x, y, z, a, b, c, isFinish_;
    std::string PX = "<PX>"; lpx = PX.length();
    std::string PISFINISH = "<isFinish>"; lpisfinish = PISFINISH.length();

    px = received_msg_.find("<PX>", 0); pxx = received_msg_.find("</PX>", 0);
    py = received_msg_.find("<PY>", 0); pyy = received_msg_.find("</PY>", 0);
    pz = received_msg_.find("<PZ>", 0); pzz = received_msg_.find("</PZ>", 0);
    pa = received_msg_.find("<PA>", 0); paa = received_msg_.find("</PA>", 0);
    pb = received_msg_.find("<PB>", 0); pbb = received_msg_.find("</PB>", 0);
    pc = received_msg_.find("<PC>", 0); pcc = received_msg_.find("</PC>", 0);
    pisfinish = received_msg_.find("<IsFinish>", 0); pisfinishh = received_msg_.find("</IsFinish", 0);
    if(px != -1 && pcc != -1){
        // ROS_WARN("Check2 pass!");
        x = received_msg_.substr(px + lpx, pxx - px - lpx);
        y = received_msg_.substr(py + lpx, pyy - py - lpx);
        z = received_msg_.substr(pz + lpx, pzz - pz - lpx);
        a = received_msg_.substr(pa + lpx, paa - pa - lpx);
        b = received_msg_.substr(pb + lpx, pbb - pb - lpx);
        c = received_msg_.substr(pc + lpx, pcc - pc - lpx);
        isFinish_ = received_msg_.substr(pisfinish + lpisfinish, pisfinishh - pisfinish - lpisfinish);
        // ROS_INFO("isFINISH = %s", isFinish_.c_str()); //For debug
        double test = stoi(isFinish_);
        if (test > 0 ){ isFinish = true; 
        /*ROS_INFO("isFinish is true!");*/} //isFinish > 0 True , isFinish < 0 False
        else if(test < 0){isFinish = false; /*ROS_WARN("isFinish is false!");*/}
        // cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<" a: "<<a<<" b: "<<b<<" c: "<<c<<endl; //For debug
        check_sum2 = true;
    }else{check_sum2 = false;}
    if(check_sum1 && check_sum2){
        // ROS_WARN("Check parsing all pass!");//For debug
        armmsg.push_back(std::stof(a1)); armmsg.push_back(std::stof(a2)); armmsg.push_back(std::stof(a3)); 
        armmsg.push_back(std::stof(a4)); armmsg.push_back(std::stof(a5)); armmsg.push_back(std::stof(a6));
        armmsg.push_back(std::stof(x)); armmsg.push_back(std::stof(y)); armmsg.push_back(std::stof(z)); 
        armmsg.push_back(std::stof(a)); armmsg.push_back(std::stof(b)); armmsg.push_back(std::stof(c)); 
        isValid = true;
        isPublished = false;
    }else{isValid = false;}
}
void TCPConnect::sendtoArm(const tcp_ros::ArmComm& msg){
    /*Add packet format as below
    ("<Sensor><Status><IsOut>TRUE</IsOut></Status><StepMode>0</StepMode><SX>" + Common.XP2[0].ToString() + 
    "</SX><SY>" + Common.XP2[1].ToString() + "</SY><SZ>" + Common.XP2[2].ToString() + "</SZ><SA>" + 
    Common.XP2[3].ToString() + "</SA><SB>" + Common.XP2[4].ToString() + "</SB><SC>" + 
    Common.XP2[5].ToString() + "</SC></Sensor>");
    */
    ROS_INFO("Receive message from the topic: %s", subtopic.c_str());
    std::string sx, sy, sz, sa, sb, sc, isOut, stepmode;
    std::string sa1, sa2, sa3, sa4, sa5, sa6;
    double sx_, sy_, sz_, sa_, sb_, sc_;
    double sa1_, sa2_, sa3_, sa4_, sa5_, sa6_;
    double isOut_;
    sx_ = msg.x; sy_ = msg.y; sz_ = msg.z;
    sa_ = msg.a; sb_ = msg.b; sc_ = msg.c;
    sa1_ = msg.j1; sa2_ = msg.j2; sa3_ = msg.j3; 
    sa4_ = msg.j4; sa5_ = msg.j5; sa6_ = msg.j6; 
     axisAng.push_back(sa1_); axisAng.push_back(sa2_); axisAng.push_back(sa3_);
    axisAng.push_back(sa4_); axisAng.push_back(sa5_); axisAng.push_back(sa6_);
    checkAngle(axisAng);
    sx = to_string(sx_); sy = to_string(sy_); sz = to_string(sz_);
    sa = to_string(sa_); sb = to_string(sb_); sc = to_string(sc_);
    sa1 = to_string(axisAng[0]); sa2 = to_string(axisAng[1]); sa3 = to_string(axisAng[2]);
    sa4 = to_string(axisAng[3]); sa5 = to_string(axisAng[4]); sa6 = to_string(axisAng[5]);
    isOut_ = msg.isOut;
    stepmode = msg.stepmode;
    if (isOut_ > 0){isOut = "TRUE";} //Always find trubole using boolean in ROS msg, so use double instead
    else{isOut = "FALSE";}
    string packet = "<Sensor><Status><IsOut>" + isOut + "</IsOut></Status><StepMode>" + stepmode + "</StepMode><SX>" + sx + 
        "</SX><SY>" + sy + "</SY><SZ>" + sz + "</SZ><SA>" + sa + "</SA><SB>" + sb+ "</SB><SC>" + sc + "</SC><SA1>"+ sa1 + "</SA1><SA2>" +
        sa2 + "</SA2><SA3>" + sa3 + "</SA3><SA4>" + sa4 + "</SA4><SA5>" + sa5 + "</SA5><SA6>" + sa6 + "</SA6></Sensor>"; 
    if (isOpen == true && isFinish == true){
        //  ROS_INFO("IsFinish is TRUE! "); //For debug
        ROS_INFO("DATA SEND TO ARM: %s", packet.c_str()); // For debug
        send(new_fd, packet.c_str(), strlen(packet.c_str()), 0);
    }else if(isFinish == false){ROS_WARN("IsFinish is False, wait for arm finish current movement.");}
    else{ROS_WARN("Cannot send msg to arm, ISOpen is false");}
          
}
void TCPConnect::checkAngle(vector<double> &angle){
    vector<int> exceedAxis;
    for (size_t i = 0; i < angle.size(); i++){
        switch (i){
            case 0:
                if (angle[i] < -30){angle[i] = -29; exceedAxis.push_back(i);}
                else if(angle[i] > 185 ){angle[i] = 184; exceedAxis.push_back(i);}
                break;
            case 1:
                if (angle[i] < -116){angle[i] = -115; exceedAxis.push_back(i);}
                else if(angle[i] > 60 ){angle[i] = 59; exceedAxis.push_back(i);}
                break;
            case 2:
                if (angle[i] < 74.55){angle[i] = 73; exceedAxis.push_back(i);}
                else if(angle[i] > 160 ){angle[i] = 159; exceedAxis.push_back(i);}
                break;
            case 3:
                if (angle[i] < -175){angle[i] = -174; exceedAxis.push_back(i);}
                else if(angle[i] > 175 ){angle[i] = 174; exceedAxis.push_back(i);}
                break;
            case 4:
                if (angle[i] < -125){angle[i] = -124; exceedAxis.push_back(i);}
                else if(angle[i] > 125 ){angle[i] = 124; exceedAxis.push_back(i);}
                break;  
            case 5: 
                if (angle[i] < -350){angle[i] = -349; exceedAxis.push_back(i);}
                else if(angle[i] > 350 ){angle[i] = 349; exceedAxis.push_back(i);} 
                break;  
        }
        
    }
    for (size_t i = 0; i < exceedAxis.size(); i++){
        ROS_WARN("Axis %i exceed limitation, set to limitation value.", exceedAxis[i]);
    }
}


void TCPConnect::start(){
    ROS_INFO("Start parallel processing sending and receiving to arm.");
    while(ros::ok()){
        std::thread t1(&TCPConnect::tcprecv, this);
        ArmSub = nh_.subscribe(subtopic, 10, &TCPConnect::sendtoArm, this);
        ros::spin();
    }
}

int main(int argc, char** argv){
    ros::init(argc, argv, "ROS-ARM_Server");
    ROS_INFO("ROS-ARM Server is started!");
    ros::NodeHandle nh;
    std::string arm_ip, my_ip;
    int arm_port;
    nh.param("arm_ip", arm_ip, std::string("192.168.1.27"));
    nh.param("my_ip", my_ip, std::string("192.168.1.43"));
    nh.param("arm_port", arm_port, int(7000));
    std::cout<<"arm_IP: "<<arm_ip<<std::endl;
    std::cout<<"my_ip: "<<my_ip<<std::endl;
    std::cout<<"arm_port: "<<arm_port<<std::endl;
    TCPConnect tcp(my_ip, arm_port);
    tcp.start();
    return 0;
}