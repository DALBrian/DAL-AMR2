"""
Created on Mon Apr 11 17:16:43 2023

@author: Small Brian
@email: dalbobo3122@gmail.com
"""
class driver_kinekatic():
    """
    No inverse kinematic calculation here.
    Use basic command to control robot.
    """
    def __init__(self, driver_connection):
        self.master = driver_connection.master
        print("Current motor speed: ", output_value = self.basespeed)

    def fwd(self, speed = 45):
        for i in range(1, 5):
            self.master.execute(i, 6, 610, output_value = speed)
            
    def bwd(self, speed = 45):
        for i in range(1, 5):
            self.master.execute(i, 6, 610, output_value = speed * -1)
            
    def left(self, speed = 45):
        for i in [1, 2]:
            self.master.execute(i, 6, 610, output_value = speed)
        for i in [3, 4]:
            self.master.execute(i, 6, 610, output_value = speed * -1)
       
    def right(self, speed = 45):
        for i in [1, 2]:
            self.master.execute(i, 6, 610, output_value = speed * -1)
        for i in [3, 4]:
            self.master.execute(i, 6, 610, output_value = speed)
        
    def rot_cw(self, speed = 45):
        for i in [1, 3]:
            self.master.execute(i, 6, 610, output_value = speed * -1)
        for i in [2, 4]:
            self.master.execute(i, 6, 610, output_value = speed)
        
    def rot_ccw(self, speed = 45):
        for i in [1, 3]:
            self.master.execute(i, 6, 610, output_value = speed)
        for i in [2, 4]:
            self.master.execute(i, 6, 610, output_value = speed * -1)
    def left_fwd(self, speed = 45):
        for i in [1, 2]:
            self.master.execute(i, 6, 610, output_value = speed)
        for i in [3, 4]:
            self.master.execute(i, 6, 610, output_value = 0)
    def right_fwd(self, speed = 45):
        for i in [1, 2]:
            self.master.execute(i, 6, 610, output_value = 0)
        for i in [3, 4]:
            self.master.execute(i, 6, 610, output_value = speed)
    def left_bwd(self, speed = 45):
        for i in [1, 2]:
            self.master.execute(i, 6, 610, output_value = 0)
        for i in [3, 4]:
            self.master.execute(i, 6, 610, output_value = speed * -1)
    def right_bwd(self, speed = 45):
        for i in [1, 2]:
            self.master.execute(i, 6, 610, output_value = speed * -1)
        for i in [3, 4]:
            self.master.execute(i, 6, 610, output_value = 0)
    def stop(self):
        for i in range(1, 5):
            self.master.execute(i, 6, 610, output_value = 0)
        