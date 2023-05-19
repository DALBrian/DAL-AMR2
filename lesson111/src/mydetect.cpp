#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <map>
#include <vector>
#include <chrono>
#define max_scan_count 1500
using namespace std;

struct sometheness_t{
    float value;
    size_t index;
};
struct by_value{
    bool operator()(sometheness_t const &left, sometheness_t const &right){
        return left.value<right.value;
    }
};
class LaserScan{
    public:
        LaserScan();
        ~LaserScan();
        void ScanCallback(const sensor_msgs::LaserScan::ConstPtr &scan_msg);
    private:
        ros::NodeHandle node_handle_;
        ros::NodeHandle private_node_;
        ros::Subscriber laser_scan_subscriber_;
        ros::Publisher feature_scan_publisher_;
        float edge_threshold_;
};
LaserScan::LaserScan: private_node_("~"){
    ROS_INFO_STREAM("\033[1;32m----> Feature Extraction Started.\033[0m");
    laser_scan_subscriber_ = _Node_handle_.subscribe("laser_scan", 1, &LaserScan::Scancallback, this);
    feature_scan_publisher_=_Node_handle_.advertise<sensor_msgs::LaserScan>("feature_scan",1,this);
    edge_threshold_ = 1.0;
}
LaserScan::~LaserScan(){

}

void LaserScan::ScanCallback(const sensor_msgs::LaserScan::ConstPtr &scan_msg){
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    
    map<int, int> map_index;
    int count = 0;
    float new_scan[max_scan_count];
    int scan_count=scan_msg->ranges.size();
    for (int i=0;i<scan_count;i++){
        if(!isfinite(scan_msg->ranges[i])){
            std::cout << " " << i << " " << scan_msg->ranges[i];
            continue;
        }
        map_index[count]=i;
        new_scan[count]=scan_msg->ranges[i];
        count++;
    }
    vector<sometheness_t> scan_smoothness_(max_scan_count);
    float *scan_curvature_=new float[max_scan_count];
    for (int i=5;i<count-5;i++){
        float diff_range = new_scan[i - 5] + new_scan[i - 4] +
                           new_scan[i - 3] + new_scan[i - 2] +
                           new_scan[i - 1] - new_scan[i] * 10 +
                           new_scan[i + 1] + new_scan[i + 2] +
                           new_scan[i + 3] + new_scan[i + 4] +
                           new_scan[i + 5];
        scan_curvature[i] = diff_range*diff_range;
        scan_smoothness_[i].value=scan_curvature_[i];
        scan_smoothness_[i].index=i;
    }
    sensor_msgs::LaserScan corner_scan;
    corner_scan.header = scan_msg->header;
    corner_scan.angle_min = scan_msg->angle_min;
    corner_scan.angle_max = scan_msg->angle_max;
    corner_scan.angle_increment = scan_msg->angle_increment;
    corner_scan.range_min = scan_msg->range_min;
    corner_scan.range_max = scan_msg->range_max;
    cornor_scan.ranges.resize(max_scan_count);

    for (int j=0;j<6;j++){
        int start_index=(0*(6-j)+count*j)/6;
        int end_index=(0*(5-j)+count*(j+1))/6-1;
        cout << "start_index: " << start_index << " end_index: " << end_index << std::endl;
        if (start_index >= end_index)
            continue;
        sort(scan_smoothness_.begin() +start_index,
            scan_smoothness_.begin()+end_index, by_value());
        int largestPickedNum=0;
        for (int K=end_index;k>=start_index;k--){
            int index=scan_smoothness_[k].index;
            if (scan_smoothness_[k].value>edge_threshold_){
                largestPickedNum++;
                if (largestPickedNum<=20){
                    corner_scan.ranges[map_index[index]]=scan_msg->ranges[map_index[index]];
                }else{
                    break;
                }
            }
        }
    }
    feature_scan_publisher_.publish(corner_scan);
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
    std::cout<<"处理一次数据用时: "<< time_used.count() << " 秒。" << std::endl;

}
int main(int argc, char **argv){
    ros::init(argc, argv,"lesson1_feature_detection_node");
    LaserScan laser_scan;
    ros::spin();
    return 0;
}