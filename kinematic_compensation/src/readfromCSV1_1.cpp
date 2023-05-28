#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <ros/ros.h>
#include <tcp_ros/ArmComm.h>

using namespace std;

std::map<int, std::vector<string>> readcsv(){
    int counter = 0;
    string filename = "/home/dal/my_ws/src/path.csv";
    std::map<int, std::vector<string>> csv_contents;
    auto ss = ostringstream{};
    std::vector<string> items;
    char delimiter =',';
    string record;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< filename << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    string file_contents = ss.str();
    istringstream sstream(file_contents);
    while (std::getline(sstream, record)){
        istringstream line(record);
        while (std::getline(line, record, delimiter)) {
            // cout<<"Record: "<<record.c_str()<<endl;
            items.push_back(record);
        }
        csv_contents[counter] = items;
        items.clear();
        cout<<"Counter: "<<counter <<endl;
        counter++;
    }
    std::vector<std::string> result = csv_contents[0];
    cout<<"Result vector element: "<<endl;
    // cout<<"Size: "<<result.size()<<endl; //for debug
    // for (const auto& ele : result){
    //     cout<<ele.c_str()<<endl;
    // }
    return csv_contents;
}


int main(int argc, char** argv){
    ros::init(argc, argv, "Moving-Coordinate Server");
    ROS_INFO("Start coordinate server");
    ros::NodeHandle nh;
    std::string topic1, topic2;
    nh.param("movebase_goal_topic", topic1, std::string("movebase/goal"));
    nh.param("arm_goal_topic", topic2, std::string("Arm_Comm"));
    cout<<"Movebase topic name: "<<topic1<<endl;
    cout<<"Arm topic name: "<<topic2<<endl;
    /*Treat csv path data*/
    std::map<int, std::vector<string>> csv_content = readcsv();
    std::vector<string> time, x1, x2;
    time = csv_content[0]; x1 = csv_content[1]; x2 = csv_content[2]; 
    /*Publish to ArmComm topic*/
    tcp_ros::ArmComm arm_msg;
    std::string pub_topic = "ArmCommand";
    ros::Publisher ArmPub = nh.advertise<tcp_ros::ArmComm>(pub_topic, 10);
    arm_msg.x = 0.0;


    ArmPub(arm_msg);
    return 0;
}