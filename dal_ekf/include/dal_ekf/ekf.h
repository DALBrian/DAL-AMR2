#include <iostream>
#include <vector>
#include <Eigen/Dense>
using namespace std;
using Eigen::VectorXd;
using Eigen::Vector2d;
using Eigen::Vector4d;
using Eigen::MatrixXd;
using Eigen::Matrix2d;
using Eigen::Matrix4d;

class KalmanFilter{
public:
    KalmanFilter();
    ~KalmanFilter();
    void predict(vector <double> imu_data);
    void imu1_update();
    void imu2_update();
    void odom_update(vector <double> odom_data);
private:
    VectorXd m_state;
    MatrixXd m_covariance;
    bool isInitialized = false;
    double dt = 0.1;
    void setState(const VectorXd& state);
    void setCovariance(const MatrixXd& covariance);
};
