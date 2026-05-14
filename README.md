Project Description:

1. Control by using Bluetooth:
The phone should download an app named BLE Controller to send signals to the built-in Bluetooth and Wifi Arduino Nano Esp32. The app can be used to control the directions and the speed of the car. 
	Pros: 
- More stable than the hand gestures.
- Does not require a controller.
- Convenient to use an app on the phone.
        Cons:
- The app only sends signals once when the user presses the button, not continuously like the joint stick, which leads to the movement not so smooth.
- Sometimes it is hard to connect to Bluetooth.

2. Control by using hand gestures:
The camera of the laptop will detect hand gestures once the Python program is run. If the USB cable is connected between the laptop and the Arduino, it will use serial to communicate. If the cable is not connected, it will use WiFi.

	Pros: 
- Use hands to control a car
- The movement of the car is smoother compared to Bluetooth communication
	Cons:
- It requires a stable local WiFi connection.
- It does not work with mobile hotspots or public WiFi.
- Sometimes hand-gestures are not detectable and inaccurate.
