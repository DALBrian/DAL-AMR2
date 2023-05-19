"""
Created on Mon Apr 15 2023

@author: Small Brian
@email: dalbobo3122@gmail.com
Candicate for the final version.
"""
class chassic_kinematic():
    """
    This class is used to control the movement of the vehicle, 
    and the speed commands are the same for each motor.
    Motor speed vary from 0~3000(RPM), divided the reduction ratio(10) will be the true speed.
    Direction of rotation is positive when vehicle moving forward(when facing the vehicle, KUKA controller on the right)
    If one of the motor speed needs to be modified, change the code as the following example:
        ex: self.master.execute(modbus_location(motor number), 6, command(Pn610=610), output_value = speed)
            self.master.execute(1, 6, 610, output_value = motor_speed1)
    """
    def __init__(self, driver_connection):
        self.master = driver_connection.master
    def stop(self):
        for i in range(1, 5):
            self.master.execute(i, 6, 610, output_value = 0)
    def motor1(self, speed =0):
        print("motor1: ", speed)
        self.master.execute(1, 6, 610, output_value = speed)
    def motor2(self, speed =0):
        print("motor2: ", speed)
        self.master.execute(2, 6, 610, output_value = speed)
    def motor3(self, speed =0):
        print("motor3: ", speed)
        self.master.execute(3, 6, 610, output_value = speed)
    def motor4(self, speed =0):
        print("motor4: ", speed)
        self.master.execute(4, 6, 610, output_value = speed)
    
    
        