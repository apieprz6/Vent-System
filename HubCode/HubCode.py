def sendCommand(motorvalue):
    if motorvalue = 180:
        from subprocess import call
        call(["sudo", "/home/pi/Git/433Utils/RPi_utils/codesend" , "180"])
    else
        from subprocess import call
        call(["sudo", "/home/pi/Git/433Utils/RPi_utils/codesend" , "0"])

sendCommand(180);

