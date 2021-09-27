
#a streamlit version of the object detector code used with the SCARA arm. 
#goal is to create a web app version of object detector so people can try it

#import cvzone_real_time_detector
import streamlit as st

#start up app
st.title("My first Streamlit app")
st.text("Enter your credit card number below >:)")

num = st.number_input("Number", step=1)
st.write(f'{num} + 1 = {num+1}')

name = st.text_input("Enter your name")
st.write(f'Hello {name}.')