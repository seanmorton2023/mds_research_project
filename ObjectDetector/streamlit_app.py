
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
    st.title("Real-Time Object Detection")
    st.text("Hello! Here's a brief tutorial on how to turn your phone camera\n" 
        + "into a smart computer vision camera.")

    webcam_choice = st.radio('Webcam choice',
                             ('My phone', 'Demo Computer'))

    if webcam_choice == 'My phone':
        st.text('You will need to download an app called "IP Webcam" (Android)\n'
                + 'or "ipCam" (Apple) onto your phone in order to run this demo.')
        st.text("Find the option that says 'start server' and press the button.")
        st.text("Enter the IP address of your webcam (ex. 12.345.67.890:8080):")
        ip_url = st.text_input("IP address")
        ip_url = "http://" + ip_url + "/shot.jpg"

        st.text(f"Using variable ip_url = {ip_url}")

    else:
        st.text('You chose "Demo computer"')
        ip_url = None

    ##num = st.number_input("Number", step=1)
    ##st.write(f'{num} + 1 = {num+1}')

    ##name = st.text_input("Enter your name")
    ##st.write(f'Hello {name}.')

    od = ObjectDetector()
    od.config_video()

    #for showing image in a Streamlit environment
    FRAME_WINDOW = st.image([])

    while True:
        od.gather_camdata(ip_url)
 
        od.classify_objects()
        od.find_markers()
        od.locate_object()

        #this takes the place of the display() function
        frame_img = cv2.cvtColor(od.img, cv2.COLOR_BGR2RGB)
        FRAME_WINDOW.image(frame_img)

