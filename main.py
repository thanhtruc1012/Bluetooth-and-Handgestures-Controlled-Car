#With USB cable:
import cv2
import mediapipe as mp 
import socket
import time
import serial


# This depends on arduino board, go to Arduino -> Tools -> Port to check
ser = serial.Serial('/dev/cu.usbmodem206EF131E6582', 115200)
mp_draw = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands


video = cv2.VideoCapture(1)


if not video.isOpened():
    print("Cannot open camera")
    exit()




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
            reverse = (pinky_tip.y > pinky_mcp.y and index_tip.y > hand.landmark[6].y and middle_tip.y > hand.landmark[10].y
            )
            #if command changes, it changes
            current_command = "S"
            if reverse or dy > 0.2:
                if current_command != 'D':
                    ser.write(b'D')
                    current_command = 'D'


            elif dx > 0.15 and pinky_tip.y < pinky_mcp.y:
                if current_command != 'R':
                    ser.write(b'R')
                    current_command = 'R'


            elif dx < -0.15 and pinky_tip.y < pinky_mcp.y:
                if current_command != 'L':
                    ser.write(b'L')
                    current_command = 'L'


            elif dy < -0.2 and pinky_tip.y < pinky_mcp.y:
                if current_command != 'F':
                    ser.write(b'F')
                    current_command = 'F'


            else:
                if current_command != 'S':
                    ser.write(b'S')
                    current_command = 'S'


            # if cannot dectect hand       
        
            mp_draw.draw_landmarks(image, hand, mp_hands.HAND_CONNECTIONS)
        elif not results.multi_hand_landmarks:
            ser.write(b'S')


        
#This code belongs to Judy Cutie

        cv2.imshow("Judy Cutie", image)
        k = cv2.waitKey(1)
        if k == ord('q'):
            break


video.release()
cv2.destroyAllWindows()


