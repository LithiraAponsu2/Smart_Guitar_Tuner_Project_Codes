def is_off_tune(f):
    # El
    if 81 <= f <= 83:
        print("Tuned-El") 
    elif f < 81:
        print("Under-El") 
    elif 83 < f <= 96:
        print("Over-El") 
    # A   
    if 109 <= f <= 111:
        print("Tuned-A") 
    elif 96 < f < 109:
        print("Under-A") 
    elif  111 < f <= 129:
        print("Over-A")
    # D   
    if 145 <= f <= 147:
        print("Tuned-D") 
    elif 129 < f < 145:
        print("Under-D") 
    elif  147 < f <= 172:
        print("Over-D")
    # G 
    if 195 <= f <= 197:
        print("Tuned-G") 
    elif 172 < f < 195:
        print("Under-G") 
    elif  197 < f <= 222:
        print("Over-G")
    # B  
    if 245 <= f <= 248:
        print("Tuned-B") 
    elif 222 < f < 245:
        print("Under-B") 
    elif  248 < f <= 289:
        print("Over-B")
    # Eh   
    if 328 <= f <= 331:
        print("Tuned-Eh") 
    elif 289 < f < 328:
        print("Under-Eh") 
    elif  331 < f:
        print("Over-Eh") 
        
is_off_tune(153.23)