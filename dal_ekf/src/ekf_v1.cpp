#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ros/ros.h>
// #include "ekf.h"
// #include "readIMU.h"
#include "../include/dal_ekf/ekf.h"
#include "../include/dal_ekf/readIMU.h"

#define gyro_std 0.01
#define acc_std  0.01
#define odom_std  0.01
using namespace std;
/*
    @brief: Extended Kalman Filter for Brian's Thesis
    @author: Small Brian
    @date: 2023/06/25
    @TODO: Record a standard rosbag data and test this program. / Parameters tuning.
*/

KalmanFilter::KalmanFilter(){
    std::cout << "Starting  class KalmanFilter" << std::endl;
    m_state = VectorXd(4);
    m_covariance = MatrixXd(4,4);
    dt = 0.1;
    isInitialized = true;
}
KalmanFilter::~KalmanFilter(){std::cout << "Killing  class KalmanFilter" << std::endl;}
void KalmanFilter::setState(const VectorXd& state){m_state = state;}
void KalmanFilter::setCovariance(const MatrixXd& covariance){m_covariance = covariance;}
void KalmanFilter::predict(vector <double> imu_data){
    /*
        @brief: Frequently update sensors, use IMU in this case
        IMU data: psi_dot
        IMU data(not integrate yet): linear_acc_x, linear_acc_y, angular_vel_x, angular_vel_y, angular_vel_z
    */
    if(isInitialized){
        VectorXd state = m_state;
        double x = state(0), y = state(1), psi = state(2), v = state(3);
        //Update state
        double x_new = x + dt * v * cos(psi);
        double y_new = y + dt * v * sin(psi);
        double psi_new = psi + dt * imu_data[3];
        double v_new = v;
        m_state << x_new, y_new, psi_new, v_new;
        //F matrix
        MatrixXd F = Matrix4d::Zero();
        F << 1, 0, -dt * v * sin(psi), dt * cos(psi),
            0, 1, dt * v * cos(psi), dt * sin(psi),
            0, 0, 1, 0,
            0, 0, 0, 1;
        //Q matrix
        MatrixXd Q = Matrix4d::Zero();
        Q(2, 2) = dt * dt * gyro_std * gyro_std;
        Q(3, 3) = dt * dt * acc_std * acc_std;

        m_covariance = F * m_covariance * F.transpose() + Q;
    }else{
        cout << "Kalman filter is not initialized!" << endl;
    }
}
void KalmanFilter::odom_update(vector <double> odom_data){
    /*
        @brief: Slowly update sensor, like odom from hector_slam or amcl
        Odom data: X, Y
        Odom data(not integrate yet): pose(quatenion): x, y, z, w
    */
    
    VectorXd state = m_state;
    MatrixXd cov = m_covariance;
    //Measurement matrix
    VectorXd z = Vector2d::Zero();
    z << odom_data[0], odom_data[1];
    //H matrix
    MatrixXd H = MatrixXd(2, 4);
    H << 1, 0, 0, 0,
        0, 1, 0, 0;
    //R matrix
    MatrixXd R = Matrix2d::Zero();
    R(0, 0) = odom_std * odom_std;
    R(1, 1) = R(0, 0);
    //z_hat
    VectorXd z_hat = H * state;
    VectorXd y = z - z_hat;
    //S matrix
    MatrixXd S = H * cov * H.transpose() + R;
    //Kalman gain
    MatrixXd K = cov * H.transpose() * S.inverse();

    m_state = state + K * y;;
    m_covariance = (Matrix4d::Identity() - K * H) * cov;


}


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "my_ekf");
    ROS_INFO("Starting my_ekf node");
    /*Test basic function*/
    KalmanFilter kf;
    kf.predict({0.1, 0.2, 0.3, 0.4, 0.5});
    kf.odom_update({0.1, 0.2, 0.3, 0.4});
    std::cout<<"Running EKF program" << std::endl;
    return 0;
}