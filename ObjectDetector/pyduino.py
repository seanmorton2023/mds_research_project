import serial
import math
from math import acos, atan, cos, sin

def sq(x): return x**2

def convert_coords(x, y):
    '''Takes the coordinates output by the object detection
    code and converts it into coordinates for the robot to move to.
    '''

    #extents of motion for the CV code
    lx = 20.5
    ly = 15.4
    #extents of motion of the robot
    rx = lx/2
    ry = 13.9

    posn_robot = ly - ry

    #convert x
    x_new = x - rx
    y_new = y - posn_robot

    return x_new, y_new

def write_read(text):
    #writing to arduino through python

    arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
    arduino.write(text.encode())

    time.sleep(0.05)
    data = arduino.readline()
    return data

def forward_kinematics(theta1, theta2):
    '''Given a set of angles to rotate the robot to, finds the current position
    of the toolhead in x and y.
    '''

    theta1F = theta1 * math.pi / 180   # degrees to radians
    theta2F = theta2 * math.pi / 180
    xP = L1 * cos(theta1F) + L2 * cos(theta1F + theta2F)
    yP = L1 * sin(theta1F) + L2 * sin(theta1F + theta2F)


    pass

def inverse_kinematics(x,y):
    '''Given a desired position in (x,y), finds the joint angles
    theta1, theta2 needed to achieve that position
    '''

    #lengths of arms
    L1 = 8.98 #inches
    L2 = 5.61 #inches

    theta2 = acos((sq(x) + sq(y) - sq(L1) - sq(L2)) / (2 * L1 * L2))
    if (x < 0 and y < 0) :
        theta2 = (-1) * theta2
      
    theta1 = atan(x / y) - atan((L2 * sin(theta2)) / (L1 + L2 * cos(theta2)))
  
    theta2 = (-1) * theta2 * 180 / math.pi
    theta1 = theta1 * 180 / math.pi

    # Angles adjustment depending in which quadrant the final tool coordinate x,y is
    if (x >= 0 and y >= 0) :       # 1st quadrant
        theta1 = 90 - theta1
      
    if (x < 0 and y > 0) :       # 2nd quadrant
        theta1 = 90 - theta1
      
    if (x < 0 and y < 0) :       # 3rd quadrant
        theta1 = 270 - theta1
        phi = 270 - theta1 - theta2
        phi = (-1) * phi
      
    if (x > 0 and y < 0) :       # 4th quadrant
        theta1 = -90 - theta1
      
    if (x < 0 and y == 0) :
        theta1 = 270 + theta1
      
  
    # Calculate "phi" angle so gripper is parallel to the X axis
    phi = 90 + theta1 + theta2
    phi = (-1) * phi

    # Angle adjustment depending in which quadrant the final tool coordinate x,y is
    if (x < 0 and y < 0) :       # 3rd quadrant
        phi = 270 - theta1 - theta2
      
    if (abs(phi) > 165) :
        phi = 180 + phi
      
    theta1=round(theta1)
    theta2=round(theta2)
    phi=round(phi)

    return theta1, theta2


if __name__ == '__main__':
    
    x = 6.2
    y = 11.3
    theta1, theta2 = inverse_kinematics(x,y)
    print('Theta1 and theta2 are: ', theta1, theta2)