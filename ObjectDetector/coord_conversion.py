
def convert_coords(x, y):
    '''Takes the coordinates output by the object detection
    code and converts it into coordinates for the robot to move to.
    '''

    #extents of motion for the CV code and the robot
    lx = 20.5
    ly = 15.4
    rx = lx/2
    ry = 13.9

    posn_robot = ly - ry

    #convert x
    x_new = x - rx
    y_new = y - posn_robot

    return x_new, y_new


if __name__ == '__main__':
    xo = 16.4
    yo = 9.2

    xnew, ynew = convert_coords(xo,yo)
    print("xnew and ynew are:",xnew,ynew)



