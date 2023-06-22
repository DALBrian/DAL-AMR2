#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "/usr/include/modbus/modbus.h"
#include <chrono>
#define g 9.80665

using namespace std;
class modbus_read{
    public:
        modbus_read(string IP, string port);
        ~modbus_read();
        void read_input();
        void parse();
    private:
        modbus_t* mb;
        string IP, port;
        int start_address = 0;
        int num_registers = 44;
        uint16_t msg[44];
        // std::vector <uint16_t> msg;
        double lin_acc_x, lin_acc_y, lin_acc_z;
        double ang_vel_x, ang_vel_y, ang_vel_z;
        double quat_x, quat_y, quat_z, quat_w;
};
modbus_read::modbus_read(string IP, string port){
    mb = modbus_new_tcp(IP.c_str(), stoi(port));
    if (mb == nullptr){
        std::cerr << "Failed to create content." << std::endl;
    }
    if (modbus_connect(mb) == -1){
        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
    }
    modbus_set_slave(mb, 1);
}
modbus_read::~modbus_read(){
    modbus_close(mb);
    modbus_free(mb);
}
void modbus_read::read_input(){
    
    int rc = modbus_read_input_registers(mb, start_address, num_registers, msg);
    if (rc == -1){
        std::cerr << "Failed to read input" << modbus_strerror(errno) << "\n";
    }
    for (int i = 0; i < num_registers; i++){
        std::cout<< "Reading: "<< i << ": " << msg[i] << std::endl;
        // msg.push_back(input_registers[i]);
    }
    parse();
}
void modbus_read::parse(){
    for (int i = 0; i < num_registers; i++){
        switch (i){
            case (25):
                lin_acc_x = msg[i] / 1000.0 * g;
                std::cout << "lin_acc_x: " << lin_acc_x << "\n";
                break;
            case (26):
                lin_acc_y = msg[i] / 1000.0 * g;
                std::cout << "lin_acc_y: " << lin_acc_y << "\n";
                break;
            case (27):
                lin_acc_z = msg[i] / 1000.0 * g;
                std::cout << "lin_acc_z: " << lin_acc_z << "\n";
                break;
            case (10):
                ang_vel_x = msg[i] / 100.0 / 180.0 * M_PI;
                std::cout << "ang_vel_x: " << ang_vel_x << "\n";
                break;
            case (11):
                ang_vel_y = msg[i] / 100.0 / 180.0 * M_PI;
                std::cout << "ang_vel_y: " << ang_vel_y << "\n";
                break;
            case (12):
                ang_vel_z = msg[i] / 100.0 / 180.0 * M_PI;
                std::cout << "ang_vel_z: " << ang_vel_z << "\n";
                break;
            case (35):
                quat_x = msg[i] / 1000.0;
                std::cout << "quat_x: " << quat_x << "\n";
                break;
            case (36):
                quat_y = msg[i] / 1000.0;
                std::cout << "quat_y: " << quat_y << "\n";
                break;
            case (37):
                quat_z = msg[i] / 1000.0;
                std::cout << "quat_z: " << quat_z << "\n";
                break;
            case (38):
                quat_w = msg[i] / 1000.0;
                std::cout << "quat_w: " << quat_w << "\n";
                break;
        }
    }
}

int main(){
    
    modbus_read modbus("192.168.1.149", "502");
    modbus.read_input();
    return 0;
}