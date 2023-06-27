import rospy
import numpy as np
import pandas as pd
import threading
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
folder = "/home/dal/data/KF_standarlized/"
filename = 'M1'
#%%
#@author: Small Brian
#@date: 20230627
#@brief: This is a program to extract data from ROS topic and save it to CSV file when ctrl+c to exit this program.



class save_data():
    def __init__(self):
        rospy.init_node('Topic_to_CSV', anonymous=True)
        print("Start extracting informaion.")
        self.record = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],])
        print("Initla shape: ", np.shape(self.record))
        rospy.Subscriber('/handsfree/imu', Imu, self.get_IMU_data)
        rospy.Subscriber('/scanmatch_odom', Odometry, self.get_odom_data)

        rospy.spin()        
    def get_IMU_data(self, data):
        # print("IMU: ", data) #for debug
        self.imustamp = data.header.stamp 
        # print("Time: ", self.imustamp) #for debug
        self.imuacc_x = data.linear_acceleration.x
        # print(self.imuacc_x) #for debug
        self.imuacc_y = data.linear_acceleration.y
        self.imuacc_z = data.linear_acceleration.z
        self.imuori_x = data.orientation.x
        # print(self.imuori_x) #for debug
        self.imuori_y = data.orientation.y
        self.imuori_z = data.orientation.z
        self.imuori_w = data.orientation.w
        self.odomstamp = 0
        self.odompos_x = 0
        self.odompos_y = 0
        self.odompos_z = 0
        self.odomori_x = 0
        self.odomori_y = 0
        self.odomori_z = 0
        self.odomori_w = 0
        self.msg = np.array([[self.imustamp, self.imuacc_x, self.imuacc_y, self.imuacc_z, 
                       self.imuori_x, self.imuori_y, self.imuori_z, self.imuori_w, self.odompos_x,
                        self.odompos_y, self.odompos_z, self.odomori_x, self.odomori_y, self.odomori_z, 
                        self.odomori_w], ])
        print("Msg Shape: ", np.shape(self.msg))
        print("Record Shape: ", np.shape(self.record))
        self.record = np.concatenate((self.record, self.msg))
        # print("IMU msg: :", self.msg)
    def get_odom_data(self, data):
        # RPS_INFO("odom callback")
        # print("Odom: ", data) #fordebug
        self.odomstamp = data.header.stamp
        self.odompos_x = data.pose.pose.position.x
        self.odompos_y = data.pose.pose.position.y
        self.odompos_z = data.pose.pose.position.z
        self.odomori_x = data.pose.pose.orientation.x
        self.odomori_y = data.pose.pose.orientation.y
        self.odomori_z = data.pose.pose.orientation.z
        self.odomori_w = data.pose.pose.orientation.w
        self.imustamp = 0
        self.imuacc_x = 0
        self.imuacc_y = 0
        self.imuacc_z = 0
        self.imuori_x = 0
        self.imuori_y = 0
        self.imuori_z = 0
        self.imuori_w = 0
        self.msg = np.array([[self.odomstamp, self.imuacc_x, self.imuacc_y, self.imuacc_z, 
                       self.imuori_x, self.imuori_y, self.imuori_z, self.imuori_w, self.odompos_x,
                        self.odompos_y, self.odompos_z, self.odomori_x, self.odomori_y, self.odomori_z, 
                        self.odomori_w], ])
        self.record = np.concatenate((self.record, self.msg))
        # print("Record: ", self.record)
    def save_to_csv(self):
        print("Start saving data to csv.")
        df = pd.DataFrame(self.record)
        df.to_csv(folder + filename + '.csv')
        print("Done! Save file to: ", folder + filename + ".csv")
    def __del__(self):
        print("Destructor called.")
        self.save_to_csv()

if __name__ == '__main__':
    print("Start rostopic to csv.")
    save_data()