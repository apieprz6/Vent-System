def sendCommand(motorvalue):
    from subprocess import call
    call(["sudo", "/home/pi/Git/433Utils/RPi_utils/codesend" , motorvalue])

sendCommand(180);
