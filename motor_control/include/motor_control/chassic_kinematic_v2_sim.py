"""
Created on Mon Apr 11 17:16:43 2023

@author: Small Brian
@email: dalbobo3122@gmail.com
"""
class driver_kinekatic():

    def __init__(self):
        print("Motor initialized successful!")
    def fwd(self, speed = 45):
        print("FORWARD", "SPEED = ",speed)
            
    def bwd(self, speed = 45):
        print("BACKWARD", "SPEED = ",speed)
            
    def left(self, speed = 45):
        print("LEFT", "SPEED = ",speed)
       
    def right(self, speed = 45):
        print("RIGHT", "SPEED = ",speed)
        
    def rot_cw(self, speed = 45):
        print("CW", "SPEED = ",speed)
        
    def rot_ccw(self, speed = 45):
        print("CCW", "SPEED = ",speed)
    def left_fwd(self, speed = 45):
        print("LEFT_FORWARD", "SPEED = ",speed)
    def right_fwd(self, speed = 45):
        print("RIGHT_FWD", "SPEED = ",speed)
    def left_bwd(self, speed = 45):
        print("LEFT_BACKWARD", "SPEED = ",speed)
    def right_bwd(self, speed = 45):
        print("RIGHT_BACKWARD", "SPEED = ",speed)
    def stop(self):
        print("STOP")
        