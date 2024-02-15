from communication import CommunicationInterface

comms_interface = CommunicationInterface('192.168.4.1', 80)
comms_interface.connectSocket('192.168.4.1', 80)

while (1):
    inpt = input("Turn on or off")
    comms_interface.updateData("controlType", "Keyboard", inpt)
    comms_interface.sendUpdates()


