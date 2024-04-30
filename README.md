## Laser Keyboard project 
![Members](https://github.com/beste-oztop/ec535-project/blob/main/private/cover.png)
### Introduction
In this project, we will implement a sample version of laser keyboards. For this project, we use a Wi-Fi interface to send data between BeagleBone and our device, two cameras to detect the key, and whether the key is pressed or the user hovers over the keyboard. We clarify everything in detail in our [report](https://docs.google.com/document/d/1uT_VG_Ezw5Ygg8JyqgYG7obMW2hD8z3a-nv8eL2APus/edit#heading=h.emdgjmjokc88).
In addition, you could find the video demonstration in the VideoDemo directory in the repo or use [Youtube Link](https://www.youtube.com/watch?v=6opqM818R8w)

### TO-DO list

- [x] Take a look at how other repos are doing the finger press detection
- [x] Figuring out initial plan for implementing each part
- [X] Taking a look at how Wi-Fi interface working: We tried to use a Wi-Fi adapter, but it seems that the one we got is incompatible with the Beaglebone Black. Also, other compatible Wi-Fi adapters are not available online to buy, or it takes time to get them (the delivery time of those was after the project deadline). So, we decided to use one of our laptops as a router and connect the Beaglebone Black to the router with an ethernet cable.
- [X] Trying to setup the Wi-Fi interface on the BeagleBone: We have implemented sending and receiving a specific message with Wi-Fi, using our Beaglebone Black as the sender and one of our other devices as the receiver to get the data.
- [x] Debugging the problem with implementing the Wi-Fi interface in C.
- [x] Writing the code for sending a specific letter from BeagleBone to other devices
- [x] Writing the code for the coordinates to letter conversion (class or a helper function, not decided yet). 
- [x] Find an image to handle the size of the sdcard and support network interface
- [x] Cross-compile opencv for arm (beaglebone) 
- [x] Fix problem with setting up a camera on beaglebone
- [x] Fix problem with green image otuput in opencv
- [x] Implement contour detection in c++
- [x] Implement color thresholding for finger detection
- [x] Implement touch detection with the second camera
- [x] Setting up a kernel to handle finger press detection
- [x] Pipeline finger detection and finger implementation
- [x] Combine different parts together
- [x] Setting up the cameras and other devices to test our design
- [x] Preparing a final video demonstration
- [x] Preparing a final report
