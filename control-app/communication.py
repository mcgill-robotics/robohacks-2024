import json
import socket


class CommunicationInterface():
    data = {
        "controlType": {"Controller": 0, "Keyboard": 1},
        "leftStick": {"x": 0.0, "y": 0.0},
        "rightStick": {"x": 0.0, "y":0.0},
        "buttons": {"Y": 0, "X": 0, "A": 0, "B": 0, "UP": 0, "DOWN": 0, "LEFT": 0, "RIGHT": 0},
        "keys": {"UP": 0, "DOWN": 0, "LEFT": 0, "RIGHT": 0, "W": 0, "A": 0, "S": 0, "D": 0},
        "mouse": {"x": 0, "y": 0}
    }
    ip_address = '192.168.1.10'
    port = 80
    sock = None
    jsonData = None

    def __init__(self, ip_address, port):
        self.ip_address = ip_address
        self.port = port

    def updateData(self, category, parameter, value):
        try:
            self.data[category][parameter] = value
            return 0
        except KeyError:
            return -1
        
    def createJSON(self):
        try:
            self.jsonData = json.dumps(self.data)
            return 0
        except:
            return -1
    def connectSocket(self, ip_address=ip_address, port=port):
        self.ip_address = ip_address
        self.port = port
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((self.ip_address, self.port))
            return 0
        except:
            return -1
        
    def closeSocket(self):
        try:
            self.sock.close()
            return 0
        except:
            return -1
        
    def sendUpdates(self):
        try:
            self.sock.sendall(self.jsonData.encode('utf8'))
            return 0
        except:
            return -1
        
    def getConnectionStatus(self):
        error = self.sock.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR)
        return error
    