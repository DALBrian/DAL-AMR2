"""
Created on Mon Apr 16  2023

@author: Small Brian
@email: dalbobo3122@gmail.com
To test inverse kinematic, use print to test it without open motor.
"""
class driver_kinekatic():
    def __init__(self, driver_connection):
        self.master = driver_connection.master
    def stop(self):
        print("STOP")
    def motor1(self, speed =0):
        print("Motor1: ", speed)
    def motor2(self, speed =0):
        print("Motor2: ", speed)
    def motor3(self, speed =0):
        print("Motor3: ", speed)
    def motor4(self, speed =0):
        print("Motor4: ", speed)
    
    
        