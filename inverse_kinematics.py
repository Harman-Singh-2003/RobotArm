import matplotlib
import matplotlib.pyplot as plt
matplotlib.use('TkAgg')  # Use TkAgg backend which is known to be interactive
import ikpy.chain
import ikpy.utils.plot as plot_utils
import numpy as np
import math

# Import serial library if you want to communicate with the robot
# import serial

# Define your robot arm model from URDF file (replace with your file path)
my_chain = ikpy.chain.Chain.from_urdf_file("arm_urdf.urdf", active_links_mask=[False, True, True, True, True, True, True])

# Set initial target position and orientation
target_position = [0, 0, 0.58]
target_orientation = [-1, 0, 0]
ik = []
plt.ion()

def do_ik():
  """
  Calculates inverse kinematics for the target position and orientation.
  Updates the global variable `ik` with the joint angles.
  """
  global ik
  old_position = ik.copy() if hasattr(ik, 'tolist') else None  # Handle potential NoneType error
  ik = my_chain.inverse_kinematics(target_position, target_orientation, orientation_mode="Y", initial_position=old_position)

def update_plot():
  """
  Clears the plot and redraws the robot arm with the current joint angles.
  """
  ax.clear()
  my_chain.plot(ik, ax, target=target_position)
  plt.xlim(-0.5, 0.5)
  plt.ylim(-0.5, 0.5)
  ax.set_zlim(0, 0.6)
  plt.draw()
  plt.pause(0.001)  # Allow for interactive updates

def move(x, y, z):
  """
  Updates the target position, calculates new joint angles, and updates the plot.
  Optionally, send commands to the robot (if serial library is imported).
  """
  global target_position
  target_position = [x, y, z]
  do_ik()
  update_plot()


# Initialize the plot
fig, ax = plot_utils.init_3d_figure()
fig.set_figheight(9)
fig.set_figwidth(13)

# Example usage: move the arm to a new position
move(0, 0.3, 0.3)

plt.show()

move(0, -0.3, 0.3)

from time import sleep
print('1')
import ipywidgets as widgets
from IPython.display import display
con = widgets.Controller()


display(con)
print(con.connected)

y=0.25
z=0.1
while con.buttons[9].value<1:
    xp=con.axes[0].value
    yp=con.axes[1].value
    zp=con.axes[2].value
    if(abs(xp)>0.1 or abs(yp)>0.1 or abs(zp)>0.1):
        x=x+xp/100
        y=y-yp/100
        z=z-zp/100
        move(x,y,z)
        sleep(0.05)