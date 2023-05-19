// g++ cpp-linux-tcp-socket-client.cpp -o client
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
const char* host = "192.168.1.43";
int port = 5001;
/*
 @brief: Because arm's message is too long to terminal input, so use a fake tcp_client to imitate arm's msg input.
*/
int main()
{
    int sock_fd;
    struct sockaddr_in serv_name;
    int status;
    char indata[1024] = {0}, outdata[1024] = {0};

    // create a socket
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
    std::string j1 ="<joint1>11.234</joint1>", j2 = "<joint2>22.234</joint2>", j3 = "<joint3>33.234</joint3>",
        j4 = "<joint4>44.234</joint4>", j5 = "<joint5>55.234</joint5>", j6 = "<joint6>66.234</joint6>",
        px = "<PX>77.234</PX>", py = "<PY>88.234</PY>", pz = "<PZ>99.234</PZ>", 
        pa = "<PA>1.23</PA>", pb = "<PB>2.34</PB>", pc = "<PC>3.234</PC>";
        std::string msg = j1+j2+j3+j4+j5+j6+px+py+pz+pa+pb+pc;
    size_t index = 0;
    while (1) {
        cout<<"Index of while: "<<index <<endl; index += 1;
        cout<<"Send fake arm message to server: "<<endl;
        // std::cin>>outdata;
        strcpy(outdata, msg.c_str());
        printf("send: %s\n", outdata);
        send(sock_fd, outdata, strlen(outdata), 0); // send data to server
        int nbytes = recv(sock_fd, indata, sizeof(indata), 0); // received data from server, wait until get respond.
        if (nbytes <= 0) {
            close(sock_fd);
            printf("server closed connection.\n");
            break;
        }
        // printf("recv: %s\n", indata);
        // sleep(0.5);
    }
    

    return 0;
}