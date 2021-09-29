
import cv2
import numpy as np
import requests #for IP webcam
import imutils
import time
#import serial

#some of the IP addresses of my phone cameras at diff places
#ipv4_url = 'http://10.0.0.164:8080/shot.jpg' #sherman apt
ipv4_url = 'http://10.105.76.135:8080/shot.jpg' #northwestern

#create a new class for object detector: makes member
#data and functions
class ObjectDetector:

    def __init__(self):
        self.thres = 0.45
        self.nms_threshold = 0.5
        #weights of mobilenet_ssd model. opencv provides us with a 
        #function that processes the weights and model by itself
        self.config_path = 'ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
        self.weights_path = 'frozen_inference_graph.pb'

    def config_video(self):
        '''sets up video self.captures for object detection
        grab an image, define parameters on the size and brightness of 
        the image. also sets default parameters for the net; required 
        to run. if you design your own model, that's when you need 
        to dork with the dnn values

        "coco" gives the class names. better to import them all from a
        file than write all 90 class names individually
        '''

        self.cap = cv2.VideoCapture(0)
        self.cap2 = cv2.VideoCapture(2)

        #size
        self.cap.set(3, 640)
        self.cap.set(4, 480)

        #brightness
        self.cap.set(10,150)
        
        #import the text of the class_names file and remove newline chars
        class_names = []
        class_file = 'coco.names'
        
        f = open(class_file)
        txt = f.read()
        f.close()

        txt_modified = txt.rstrip('\n')
        self.class_names = txt_modified.split('\n')

        #parameters for dnn
        self.net = cv2.dnn_DetectionModel(self.weights_path, self.config_path)
        self.net.setInputSize(320, 320)
        self.net.setInputScale(1.0/127.5)
        self.net.setInputMean((127.5, 127.5, 127.5))
        self.net.setInputSwapRB(True)

    def gather_camdata(self, ip_url = None):
        '''get data from either laptop webcams or from phone camera
        gather data from both cameras

        if user gives an IP address, use phone cam as primary cam. else,
        use laptop webcam as primary cam
        '''

        if ip_url:
            img_response = requests.get(ip_url)
            img_array = np.array(bytearray(img_response.content), dtype = np.uint8)

            #decode, resize and show the image data
            self.img = cv2.imdecode(img_array, -1)
            self.img = imutils.resize(self.img, width=1000, height=1800)

        else:
            _, self.img = self.cap.read()

        #second capture, from webcam #2. may or not be active; handled later
        _, self.img2 = self.cap2.read()

    def classify_objects(self):
        '''carry out detection and classification of objects

        first, feed our image into the neural net as testing data.
        attempt to detect objects with a certain confidence threshold above
        which we're sure there's an object

        also carries out non-maximum suppression (NMS): if two boxes describe
        the same object, remove all but the label with the max confidence 
        level. works by scanning the bounding boxes and indices of the results,
        and giving suggestions for what to keep
        '''
        classIds, confs, bbox = self.net.detect(self.img, confThreshold=self.thres)
    
        #convert an array of arrays to a list of arrays
        bbox = list(bbox)

        #remake into a list and remove outer brackets
        confs = list(np.array(confs).reshape(1,-1)[0])
        confs = list(map(float, confs))
    
        #non-max suppression
        indices = cv2.dnn.NMSBoxes(bbox, confs, self.thres,  nms_threshold = self.nms_threshold)

        self.markers = []
        self.arm_object = []
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
            classification = self.class_names[classIds[i][0]-1]
            conf_string = str(round(confidence*100, 2))

            if(classification == "bird" or classification == "cat"):
                self.markers.append(center_coords)
            else:
                #check to see if "obj" exists:
                try:
                    if classification.lower() == obj.lower():
                        self.arm_object = center_coords
                except NameError:
                    print("Name error: object not defined")

            cv2.rectangle(self.img, (x,y), (x+w, y+h), color=(0,255,0), thickness=2)
            cv2.putText(self.img,classification, text_coords, 
                           cv2.FONT_HERSHEY_PLAIN, 1.5, (0,255,0), 2)
            #cv.putText(self.img,conf_string, conf_coords, 
            #               cv2.FONT_HERSHEY_PLAIN, 1.5, (0,255,0), 2)
       
        print("\nMarkers and arm object:")
        print(self.markers)
        print(self.arm_object)

    def find_markers(self):
        '''find locations of our 4 fiducial markers. check to see which ones
        are in each of the 4 quadrants of our image and define those as the 
        4 corners of our grid
        '''

        self.top_left = []
        self.top_right = []
        self.bottom_left = []
        self.bottom_right = []

        #640 by 480 are the approx. dims of our camera width
        center_x = 160 #should be 320 and 240, not sure why it isn't
        center_y = 120

        for marker in self.markers:
            if(marker[0] < center_x and marker[1] < center_y):
                self.top_left = marker
            elif(marker[0] > center_x and marker[1] < center_y):
                self.top_right = marker
            elif(marker[0] < center_x and marker[1] > center_y):
                self.bottom_left = marker
            elif(marker[0] > center_x and marker[1] > center_y):
                self.bottom_right = marker

        #this code will only work if the top left + right markers are in the frame
        print("\nTop Left and Right:")
        print(self.top_left)
        print(self.top_right)

    def locate_object(self):
        '''find distance of objects from reference markers
        multiply the distance by the conversion factor, pixels to inches
        this is where location data can be sent to the arduino as well
        '''

        try:
            x_squared_ref = (float(self.top_right[0]) - float(self.top_left[0]))**2
            y_squared_ref = (float(self.top_right[1]) - float(self.top_left[1]))**2
            conversion = 18/(x_squared_ref + y_squared_ref)**0.5 
 
        except IndexError:
            print("Index error: conversion")

        try:
            x_dist = (float(self.arm_object[0]) - float(self.top_left[0])) 
            y_dist = (float(self.arm_object[1]) - float(self.top_left[1]))
       
            x_dist *= conversion
            y_dist *= conversion
            print("x distance: ",x_dist)
            print("y distance: ",y_dist)

            #value = self.write_read()
            #print(value) # printing the value returned

        except NameError:
            print("Name error: conversion wasn't properly calculated")

        except IndexError:
            print("Index error: calculating dist of object")

    def display(self):
        '''display images to screen.
        camera 2 may not always be connected; error is handled here
        '''

        cv2.imshow("Camera 1", self.img)
        try:
            cv2.imshow("Camera 2", self.img2)
        except cv2.error:
            print("CV error: data not read from camera 2")

    def write_read(x):
        #writing to arduino through python
        arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)
        arduino.write(x.encode())
        time.sleep(0.05)
        data = arduino.readline()
        return data


#make a main function so that we can determine whether or not to run the code
#in this file. especially useful for the streamlit app, which imports
#the object detector class but doesn't run the exact same code

if __name__ == '__main__':

    od = ObjectDetector()
    od.config_video()
    
    #obj = input("Enter an object: ")

    #collect data infinitely
    while True:
        od.gather_camdata()
        od.classify_objects()
        od.find_markers()
        od.locate_object()

        od.display()
        cv2.waitKey(1)




