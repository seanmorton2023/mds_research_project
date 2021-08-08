#use cv2
import cv2 as cv
import numpy as np

#parameters to dork with: confidence value for detection
#nms threshold is on a scale from 0 to 1. the lower the value
#of nms_threshold, the more that classifications get suppressed
#nms threshold close to 1 means "don't suppress anything"
thres = 0.35 #prev. 0.45
nms_threshold = 0.5

#import our image
#img = cv.imread('lena.png')
cap = cv.VideoCapture(1)

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

#keep taking capture images indefinitely
while True:

    success, img = cap.read()

    #feed our image into the neural net as testing data.
    #attempt to detect objects with a certain confidence threshold above
    #which we're sure there's an object
    classIds, confs, bbox = net.detect(img, confThreshold=thres)
    #print(classIds, bbox)
    
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


    for i in indices:

        #loop through indices, and find the bounding boxes and 
        #classifications that correspond to them
        i = i[0]
        box = bbox[i]
        x, y, w, h = box
        confidence = confs[i]

        text_coords = (x + 10, y + 30)
        conf_coords = (x + w - 70, y + 50)

        #this extracts a certain class ID from the list of class id's,
        #then subtracts 1 because it uses indexing from 0
        classification = class_names[classIds[i][0]-1]
        conf_string = str(round(confidence*100, 2))

        cv.rectangle(img, (x,y), (x+w, y+h), color=(0,255,0), thickness=2)
        cv.putText(img,classification, text_coords, 
                       cv.FONT_HERSHEY_PLAIN, 1.5, (0,255,0), 2)
        #cv.putText(img,conf_string, conf_coords, 
        #               cv.FONT_HERSHEY_PLAIN, 1.5, (0,255,0), 2)
       


    #class IDs will print as numerical representations from our list,
    #but these can be converted back into alphabetical representations

    #loop through all the data we found from our image input simultaneously
    #if there is no data found, skip the labeling step
    #if (len(classIds) != 0):
    #    zipped_data = zip(classIds.flatten(), confs.flatten(), bbox)
    #    for classId, confidence, box in zipped_data:
    
    #        #index starting with 0, not 1, and retrieve a classification from our list of names
    #        classification = class_names[classId - 1]
    #        text_coords = (box[0] + 10, box[1] + 30)
    #        conf_coords = (box[2] - 10, box[1] + 30)
    #        conf_string = str(round(confidence*100,2))


    #        #print out the class, the confidence level, and draw a box around objects
    #        cv.rectangle(img, box, color=(0,255,0), thickness=2)
    #        cv.putText(img,classification, text_coords, 
    #                   cv.FONT_HERSHEY_PLAIN, 1.5, (0,255,0), 2)
    #        cv.putText(img,conf_string, conf_coords, 
    #                   cv.FONT_HERSHEY_PLAIN, 1.5, (0,255,0), 2)

    cv.imshow("Output", img)

    #cv.waitKey(0) for indefinite time to keep img up 
    cv.waitKey(1)








