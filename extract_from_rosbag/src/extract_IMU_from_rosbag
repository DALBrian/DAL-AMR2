    #!/usr/bin/env python3
'''
Extract IMU infomation from rosbag and save to csv
IMU = handsfreeIMU/A9
IMU topic = handsfree/imu and handsfree/mag
In your terminal
0. Change the arraysize & trigger to 99999
1. Python3 /run this program
2. Open another termianl and run "rosbag play <bag files>.bag"
3. Use index to get the IMU data size
4. Change array size and trigger to IMU data size
5. Run step 1 & 2, get the IMU data csv
Date: 20230322
Author: SmallBrian
'''

import rospy
import numpy as np
import pandas as pd
from sensor_msgs.msg import Imu
from sensor_msgs.msg import MagneticField
arraysize = round(205*295)
trigger = arraysize
filename = "IMU_static_noNorm"
folder = "/home/dal/data/IMU/0327/"
class listener():
    def __init__(self):
        print("Start extracting informaion.")
        self.IMU_data = np.zeros((arraysize+1, 14))
        self.IMU_index = 1
        self.MAG_index = 1
        rospy.Subscriber('handsfree/imu', Imu, self.get_IMU_data)
        rospy.Subscriber('handsfree/mag', MagneticField, self.get_MAG_data)
    def get_IMU_data(self, data):
        # ang_vel_x = data.angular_velocity.x
        # ang_vel_y = data.angular_velocity.y
        # ang_vel_z = data.angular_velocity.z
        # ori_x = data.orientation.x
        # ori_y = data.orientation.y
        # ori_z = data.orientation.z
        # ori_w = data.orientation.w
        # lin_acc_x = data.linear_acceleration.x
        # lin_acc_y = data.linear_acceleration.x
        # lin_acc_z = data.linear_acceleration.x

        self.IMU_data[self.IMU_index][0] = self.IMU_index
        self.IMU_data[self.IMU_index][1] = data.angular_velocity.x
        self.IMU_data[self.IMU_index][2] = data.angular_velocity.x
        self.IMU_data[self.IMU_index][3] = data.angular_velocity.z
        self.IMU_data[self.IMU_index][4] = data.orientation.x
        self.IMU_data[self.IMU_index][5] = data.orientation.y
        self.IMU_data[self.IMU_index][6] = data.orientation.z
        self.IMU_data[self.IMU_index][7] = data.orientation.w
        self.IMU_data[self.IMU_index][8] = data.linear_acceleration.x
        self.IMU_data[self.IMU_index][9] = data.linear_acceleration.y
        self.IMU_data[self.IMU_index][10] = data.linear_acceleration.z

        # print("IMU_data = ", self.IMU_data)
        
        print("Index = ", self.IMU_index)
        self.IMU_index += 1
        if self.IMU_index == trigger:
            self.IMU_to_CSV()

    def get_MAG_data(self, data):
        # mag_x = data.magnetic_field.x
        # mag_y = data.magnetic_field.y
        # mag_z = data.magnetic_field.z
        self.IMU_data[self.IMU_index][11] = data.magnetic_field.x
        self.IMU_data[self.IMU_index][12] = data.magnetic_field.y
        self.IMU_data[self.IMU_index][13] = data.magnetic_field.z
        self.MAG_index += 1
        # if self.MAG_index == trigger:
        #     self.MAG_to_CSV()

    def IMU_to_CSV(self):
        pd.DataFrame(self.IMU_data).to_csv(folder+ filename + ".csv")
        print("CSV file save to ", folder+ filename + ".csv","!")


    # def MAG_to_CSV(self):
    #     self.MAG_data[self.MAG_data != 0]
    #     pd.DataFrame(self.MAG_data).to_csv("/home/dal/data/MAG/22023-03-20-16-31-54.csv")
    #     print("CSV file save!")

if __name__ == '__main__':
    rospy.init_node("Extract_IMU_Data", anonymous=True)
    print("Starting node: Extract_IMU_Data")
    print("Record filename: ", filename, ".bag")
    listen = listener()
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    rospy.spin()