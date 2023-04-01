from machine import Pin
import utime

pins = [
    Pin(15,Pin.OUT), #IN1
    Pin(14,Pin.OUT), #IN2
    Pin(16,Pin.OUT), #IN3
    Pin(17,Pin.OUT), #IN4
]

full_step_sequence = [
    [1,0,0,0],
    [0,1,0,0],
    [0,0,1,0],
    [0,0,0,1],
]

# stepper control unction
def stepper(quarter_turns: int, direction: str): 
    if direction == "CW":
        for j in range(130*quarter_turns):  # CW
            for step in full_step_sequence:
                for i in range(len(pins)):
                    pins[i].value(step[i])
                    utime.sleep(0.001)
    elif direction == "CCW":
        for j in range(130*quarter_turns):  # CCW
            for step in reversed(full_step_sequence):
                for i in range(len(pins)-1, -1, -1):
                    pins[i].value(step[i])
                    utime.sleep(0.001)
    
def is_off_tune(f):
    
    # T - tuned
    # O - over tuned
    # U - under tuned
    
    # El
    if 81 <= f <= 83:
        return "T", "El" 
    elif f < 81:
        return "U", "El" 
    elif 83 < f <= 96:
        return "O", "El" 
    # A   
    if 109 <= f <= 111:
        return "T", "A" 
    elif 96 < f < 109:
        return "U", "A"
    elif  111 < f <= 129:
        return "O", "A"    
    # D   
    if 145 <= f <= 147:
        return "T", "D" 
    elif 129 < f < 145:
        return "U", "D"
    elif  147 < f <= 172:
        return "O", "D"    
    # G 
    if 195 <= f <= 197:
        return "T", "G" 
    elif 172 < f < 195:
        return "U", "G"
    elif  197 < f <= 222:
        return "O", "G"    
    # B  
    if 245 <= f <= 248:
        return "T", "B" 
    elif 222 < f < 245:
        return "U", "B"
    elif  248 < f <= 289:
        return "O", "B"    
    # Eh   
    if 328 <= f <= 331:
        return "T", "Eh" 
    elif 289 < f < 328:
        return "U", "Eh" 
    elif  331 < f:
        return "O", "Eh" 
    else:
        return "H", "H"  


while True:
    frequency = 350  # give frquency input
    status, note = is_off_tune(frequency)
    if status == "T":
        print(f"Tuned {note}")
    elif status == "U":
        print(f"Under Tuned {note}")
        stepper(quarter_turns = 1, direction = "CW")
        utime.sleep(1)
    elif status == "O":
        print(f"Over Tuned {note}")
        stepper(quarter_turns = 1, direction = "CCW")
        utime.sleep(1)
    else:
        print("Error")
    