
#a streamlit version of the object detector code used with the SCARA arm. 
#goal is to create a web app version of object detector so people can try it

from cvzone_real_time_detector import ObjectDetector
import cv2
import streamlit as st

#NOTE: the streamlit app must be run by command line.
#1. open up Anaconda shell
#2. activate streamlit environment: "conda activate <shell name>",
#   I called my shell streamlit_shell
#2.5 to check that streamlit is installed in your shell, open python with
##  command "py", in Python >>>import streamlit and >>>streamlit.__version__
#   to get out of python: >>>exit() 
#3. navigate to location where your streamlit script is
#4. run streamlit app: streamlit run <script name>, my script is called
#   streamlit_app.py

if __name__ == '__main__':

    #start up app
    st.title("My first Streamlit app")
    st.text("Enter a number:")

    num = st.number_input("Number", step=1)
    st.write(f'{num} + 1 = {num+1}')

    name = st.text_input("Enter your name")
    st.write(f'Hello {name}.')

    #st.title("Webcam Live Feed")
    #run = st.checkbox('Run')
    #FRAME_WINDOW = st.image([])
    #camera = cv2.VideoCapture(0)

    #while run:
    #    _, frame = camera.read()
    #    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    #    FRAME_WINDOW.image(frame)
    #else:
    #    st.write('Stopped')


    od = ObjectDetector()
    od.config_video()

    #for showing image in a Streamlit environment
    FRAME_WINDOW = st.image([])

    while True:
        od.gather_camdata()
 
        od.classify_objects()
        od.find_markers()
        od.locate_object()

        #this takes the place of the display() function
        frame_img = cv2.cvtColor(od.img, cv2.COLOR_BGR2RGB)
        FRAME_WINDOW.image(frame_img)

        ####IMPORTANT###
        #currently my color scheme is in BGR. need to convert it to RGB
            #    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)


    #st.write("Main function worked!")