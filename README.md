## Laser Keyboard project 

### TO-DO list

- [ ] Take a look at how other repos are doing the finger press detection
- [x] Figuring out initial plan for implementing each part
- [X] Taking a look at how Wi-Fi interface working: We tried to use a Wi-Fi adapter, but it seems that the one we got is incompatible with the Beaglebone Black. Also, other compatible Wi-Fi adapters are not available online to buy, or it takes time to get them (the delivery time of those was after the project deadline). So, we decided to use one of our laptops as a router and connect the Beaglebone Black to the router with an ethernet cable.
- [X] Trying to setup the Wi-Fi interface on the BeagleBone: We have implemented sending and receiving a specific message with Wi-Fi, using our Beaglebone Black as the sender and one of our other devices as the receiver to get the data.
- [ ] Debugging the problem with implementing the Wi-Fi interface in C.
- [ ] Writing the code for sending a specific letter from BeagleBone to other devices
- [ ] Writing the code for the coordinates to letter conversion (class or a helper function, not decided yet). 
- [x] Find an image to handle the size of the sdcard and support network interface
- [x] Cross-compile opencv for arm (beaglebone) 
- [x] Fix problem with setting up a camera on beaglebone
- [x] Fix problem with green image otuput in opencv
- [x] Implement contour detection in c++
- [ ] Implement color thresholding for finger detection
- [ ] Setting up a kernel to handle finger press detection
- [ ] Pipeline finger detection and finger implementation
