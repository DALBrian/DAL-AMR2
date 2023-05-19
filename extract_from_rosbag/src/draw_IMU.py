import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import time
filename = '2023-03-27-19-55-38'
foler = '/home/dal/data/IMU/0327/'
filepath = foler + filename +'.csv'
timestep = 1/295
timestep2 = pow(timestep, 2)
def read_csv(filepath):
    usecols = ['Index', 'linear_acceleration.x', 'linear_acceleration.y', 'linear_acceleration.z',
                'angular_velocity.x', 'angular_velocity.y', 'angular_velocity.z']
    csvdata = pd.read_csv(filepath, usecols=usecols)
    print(csvdata.head())
    datasize = csvdata.size / len(usecols)
    return csvdata, datasize
def get_vel_and_displacement(csvdata, datasize):
    dis = np.zeros((datasize, 4))
    vel = np.zeros((datasize, 4))
    for i in range(datasize):
        current_time = timestep * i + timestep
        vel[i][0] = current_time
        vel[i][1] = vel[i-1][1] + csvdata["linear_acceleration.x"][i] * timestep
        vel[i][2] = vel[i-1][2] + csvdata["linear_acceleration.y"][i] * timestep
        vel[i][3] = vel[i-1][3] + csvdata["linear_acceleration.z"][i] * timestep
        
         
    return vel, dis
        

#%%
if __name__ == '__main__':
    csvdata, datasize = read_csv(filepath)
    vel, dis = get_vel_and_displacement(csvdata, int(datasize))
    total_acc_x = [0]
    total_acc_y = [0]
    total_acc_z = [0]

    total_vel_x = [0]
    total_vel_y = [0]
    total_vel_z = [0]
    total_ang_x = [0]
    total_ang_y = [0]
    total_ang_z = [0]
    total_time = [0]
    for i in range(len(dis)):
        total_acc_x.append(csvdata["linear_acceleration.x"][i])
        total_acc_y.append(csvdata["linear_acceleration.y"][i])
        total_acc_z.append(csvdata["linear_acceleration.z"][i])
        total_vel_x.append(vel[i][1])
        total_vel_y.append(vel[i][2])
        total_vel_z.append(vel[i][3])
        total_time.append(vel[i][0])
#%%
fig1, ax1 = plt.subplots(nrows=1, ncols=1, sharex = False)
ax1.plot(total_time, total_acc_x, color="black", label="X_axis")
ax1.plot(total_time, total_acc_y, color="blue", label="Y_axis")
ax1.set_title('Acceleration X_Y-t figure', color="black")
ax1.set_ylabel("Acceleration (m/s2)", color="black")
ax1.set_xlabel("Time(s)")
ax1.set_ylim([-5, 5])
ax1.grid()
ax1.legend()
fig1.savefig(foler + filename + '-1' + '.png')

fig2, ax2 = plt.subplots(nrows=1, ncols=1, sharex = False)
ax2.plot(total_time, total_acc_z, color="black")
ax2.set_title('Acceleration Z-t figure', color="black")
ax2.set_ylabel("Acceleration (m/s2)", color="black")
ax2.set_xlabel("Time(s)")
ax2.set_ylim([0, 12])
ax2.grid()
fig2.savefig(foler + filename + '-2' + '.png')

fig3, ax3 = plt.subplots(nrows=1, ncols=1, sharex = False)
ax3.plot(total_acc_x, total_acc_y, color="black")
ax3.set_title('Accleration Y-X', color="black")
ax3.set_ylabel("Acceleration Y(m/s2)", color="black")
ax3.set_xlabel("Acceleration X(m/s2)", color="black")
ax3.set_xlim([-5, 5])
ax3.set_ylim([-5, 5])
ax3.grid()
fig3.savefig(foler+ filename + '-3' + '.png')


print("All images save to: ", foler)
