#Without USB cable:
import cv2
import mediapipe as mp 
import socket
import time
#HOST depends on WiFi’s IP, go to Arduino -> File -> Example -> Wifi scan to check if the Wifi exists. Then run the Arduino code provided above to check IP
HOST = "xxx.xxx.xx.xxx"
PORT = 1210


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


s.connect((HOST, PORT))


mp_draw = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

#this is usually 0
video = cv2.VideoCapture(1)


if not video.isOpened():
    print("Cannot open camera")
    exit()
last_cmd = ""


with mp_hands.Hands(max_num_hands = 2, min_detection_confidence=0.5, min_tracking_confidence=0.5) as hands:
    while True:
        ret, image = video.read()
        
        if not ret:
            print("Failed to grab frame")
            break
        #open cv use the original color, mediapipe use rgb
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        #improve the productivity
        image.flags.writeable = False
        #process
        results = hands.process(image)
        #process is done
        image.flags.writeable = True
        #chang the color again
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        cmd = "S"
        if results.multi_hand_landmarks:


            hand = results.multi_hand_landmarks[0]


            wrist = hand.landmark[0]


            index_tip = hand.landmark[8]
            middle_tip = hand.landmark[12]
            pinky_tip = hand.landmark[20]


            pinky_mcp = hand.landmark[17]


            dx = index_tip.x - wrist.x
            dy = index_tip.y - wrist.y
            dy2 = middle_tip.y - wrist.y
            #pinky up 
            #reverse = (pinky_tip.y > pinky_mcp.y and index_tip.y > hand.landmark[6].y and middle_tip.y > hand.landmark[10].y)


            if dy > 0.2:
                cmd = "D"


            elif dx > 0.15: 
                cmd = "L"


            elif dx < -0.15:
                cmd = "R"


            elif dy < -0.2: 
                cmd = "F"


            else:
                cmd = "S"


            mp_draw.draw_landmarks(image, hand, mp_hands.HAND_CONNECTIONS)
        
        if cmd != last_cmd:
            s.sendall(cmd.encode())
            last_cmd = cmd
#This code is from Judy Cutie

        cv2.imshow("Judy Cutie", image)
        k = cv2.waitKey(1)
        if k == ord('q'):
            break


video.release()
cv2.destroyAllWindows()







