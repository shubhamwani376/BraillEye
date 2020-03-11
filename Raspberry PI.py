import spidev
import time
import cv2
import pytesseract
from PIL import Image
import RPi.GPIO as GPIO
import time
import os
GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_UP)
global time_stamp
time_stamp = time.time()

try:
    image = cv2.imread('/home/pi/Downloads/tesseract-python/images/ocrtest.jpeg')
    # Convert image to grayscale

    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    '''temp = input("Do you want to pre-process the image ?\nThreshold : 1\nGrey : 2\nNone : 0\nEnter your choice : ")

    # If user enter 1, Process Threshold
    if temp == "1":
        gray = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)[1]
    elif temp == "2":
        gray = cv2.medianBlur(gray, 3)'''

     # store grayscale image as a temp file to apply OCR

    #filename = "{}.jpeg".format("temp")

    cv2.imwrite('/home/pi/Downloads/tesseract-python/images/Noisy image.jpeg', gray)

     # load the image as a PIL/Pillow image, apply OCR, and then delete the temporary file

    text = pytesseract.image_to_string(Image.open('/home/pi/Downloads/tesseract-python/images/ocrtest.jpeg'))
    print(text)
    w=open('ocr.txt','w')
    w.write(text)
    w.close()
    
    spi=spidev.SpiDev()
    spi.open(0,0)
    spi.max_speed_hz = 125000
    time.sleep(0.0005)
    r=open('ocr.txt','r')
    chars=[]
    for line in r:
        for c in line:
            chars.append(c)
    j=0
    for i in chars:
        chars[j]=i.rstrip("\n")
        j+=1
    print (chars)
    global Slave_No=0
    global i=0
    print ("Initialisation Done")
    
def my_callback(channel):         # put in to debounce  
    time_now = time.time()  
    if (time_now - time_stamp) >= 0.3:  
        print ("Rising edge detected on port 24 - even though, in the main thread,")  
        print ("we are still waiting for a falling edge - how cool? \n")
    
    if (i<len(chars)):
            Slave_No=0
            spi.open(0,Slave_No)
            time.sleep(0.0005)  #delay for stability
            st=chars[i]
            f=int(bin((ord(st))),2)
            spi.xfer([f])
            print([f])
   
            Slave_No=1
            spi.open(0,Slave_No)
            time.sleep(0.0005)  #delay for stability
            st=chars[i+1]
            f=int(bin((ord(st))),2)
            spi.xfer([f])
            print([f])
            i=i+2
    time_stamp = time_now
            
except Exception as e:
    print(e.args)

except KeyboardInterrupt:   #if Ctrl + C is pressed, cleanly exit our program
    to_send=0b00000000
    spi.xfer(to_send)
    spi.close()
    GPIO.cleanup()       # clean up GPIO on CTRL+C exit  
    GPIO.cleanup()           # clean up GPIO on normal exit  
    print ("Program Exited")
