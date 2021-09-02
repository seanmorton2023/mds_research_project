#use cv2
import cv2 as cv
import numpy as np

#serial port for arduino
#import serial

#for readability
import time

#parameters to dork with: confidence value for detection
#nms threshold is on a scale from 0 to 1. the lower the value
#of nms_threshold, the more that classifications get suppressed
#nms threshold close to 1 means "don't suppress anything"
thres = 0.45
nms_threshold = 0.5

#import our image
#img = cv.imread('1.png')
cap = cv.VideoCapture(1)

#second capture: we will need two cameras simultaneously
cap2 = cv.VideoCapture(2)


#define parameters on how big the picture is
cap.set(3, 640)
cap.set(4, 480)

#brightness, apparently
cap.set(10,150)

#coco gives the class names. better to import them all from a
#file than write all 90 class names individually
class_names = []
class_file = 'coco.names'

#import the text of the file and remove newline chars
f = open(class_file)
txt = f.read()
txt_modified = txt.rstrip('\n')
class_names = txt_modified.split('\n')

#weights of mobilenet_ssd model. opencv provides us with a 
#function that processes the weights and model by itself
config_path = 'ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
weights_path = 'frozen_inference_graph.pb'

#default parameters for the net; required to run. if you design
#your own model, that's when you need to dork with these values
net = cv.dnn_DetectionModel(weights_path, config_path)
net.setInputSize(320, 320)
net.setInputScale(1.0/127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)

#writing to arduino through python
#arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)

#def write_read(x):
 #   arduino.write(x.encode())
  #  time.sleep(0.05)
   # data = arduino.readline()
    #return data

 
#Taking input from user
obj = input("Enter an object: ")


#keep taking capture images indefinitely
while True:

    #gather data from both cameras
    success, img = cap.read()
    _, img2 = cap2.read()

    #feed our image into the neural net as testing data.
    #attempt to detect objects with a certain confidence threshold above
    #which we're sure there's an object
    classIds, confs, bbox = net.detect(img, confThreshold=thres)

    #print("\nClass IDs visible:")
    #print(classIds)
    
    #convert an array of arrays to a list of arrays... a distinction
    #that only really matters to opencv
    bbox = list(bbox)
    #print(bbox, confs)

    #remake the confidence values into a list, and remove outer brackets
    confs = list(np.array(confs).reshape(1,-1)[0])
    #print(confs)
    confs = list(map(float, confs))
    #print(confs[0])
    #print(type(confs[0]))

    
    #non-maximum suppression (NMS): if two boxes describe the same object,
    #remove all but the label with the max confidence level
    #works by scanning the bounding boxes and indices of the results,
    #and giving suggestions for what to keep
    indices = cv.dnn.NMSBoxes(bbox, confs, thres,  nms_threshold = nms_threshold)
    #print(indices)

    markers = []
    arm_object = []
    for i in indices:

        #loop through indices, and find the bounding boxes and 
        #classifications that correspond to them
        i = i[0]
        box = bbox[i]
        x, y, w, h = box
        confidence = confs[i]
        
        center_coords = [((x + w)/2),((y + h)/2)]

        text_coords = (x + 10, y + 30)
        conf_coords = (x + w - 70, y + 50)

        #this extracts a certain class ID from the list of class id's,
        #then subtracts 1 because it uses indexing from 0


        classification = class_names[classIds[i][0]-1]

        #print("\nClassification and center coords:")
        #print(classification)
        #print(center_coords)

        conf_string = str(round(confidence*100, 2))

        if(classification == "bird" or classification == "cat"):
            markers.append(center_coords)
        elif(classification.lower() == obj.lower()):
            arm_object = center_coords

        cv.rectangle(img, (x,y), (x+w, y+h), color=(0,255,0), thickness=2)
        cv.putText(img,classification, text_coords, 
                       cv.FONT_HERSHEY_PLAIN, 1.5, (0,255,0), 2)
        #cv.putText(img,conf_string, conf_coords, 
        #               cv.FONT_HERSHEY_PLAIN, 1.5, (0,255,0), 2)
       
    print("\nMarkers and arm object:")
    print(markers)
    print(arm_object)

    top_left = []
    top_right = []
    bottom_left = []
    bottom_right = []

    #we have 4 fiducial markers. check to see which ones are in each
    #of the 4 quadrants of our image and define those as the 4 corners
    #of our grid
    center_x = 160 
    center_y = 120

    for marker in markers:
        if(marker[0] < center_x and marker[1] < center_y):
            top_left = marker
        elif(marker[0] > center_x and marker[1] < center_y):
            top_right = marker
        elif(marker[0] < center_x and marker[1] > center_y):
            bottom_left = marker
        elif(marker[0] > center_x and marker[1] > center_y):
            bottom_right = marker


    #this code will only work if the top left + right markers are in the frame

    print("\nTop Left and Right:")
    print(top_left)
    print(top_right)
    

    try:
        x_squared_ref = (float(top_right[0]) - float(top_left[0]))**2
        y_squared_ref = (float(top_right[1]) - float(top_left[1]))**2
        conversion = 18/(x_squared_ref + y_squared_ref)**0.5 

        #conversion = (18/(((float(top_right[0])-float(top_left[0]))**2 
        #           + (float(top_right[1])-float(top_left[1]))**2)**0.5)) 
    except IndexError:
        print("Index error: conversion")

    try:
        x_dist = (float(arm_object[0]) - float(top_left[0])) 
        y_dist = (float(arm_object[1]) - float(top_left[1]))
        


        #multiply the distance by the conversion factor, pixels to inches
        x_dist *= conversion
        y_dist *= conversion
        print("x distance: ",x_dist)
        print("y distance: ",y_dist)

        #value = write_read()
        #print(value) # printing the value returned
    except NameError:
        print("Name error: conversion wasn't properly calculated")

    except IndexError:
        print("Index error: calculating dist of object")

    cv.imshow("Camera 1", img)
    cv.imshow("Camera 2", img2)

    #cv.waitKey(0) for indefinite time to keep img up 
    cv.waitKey(0)

    #for debug
    #time.sleep(1)








