import json
import socket


class CommunicationInterface():
    data = {}
    ip_address = ''
    port = 80
    sock = None
    jsonData = None

    def __init__(self, ip_address, port, config_path="user_config.json"):
        self.ip_address = ip_address
        self.port = port
        with open(config_path, 'r') as config_file:
            self.data = json.load(config_file)

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
            self.createJSON()
            self.sock.sendall(self.jsonData.encode('utf8'))
            return 0
        except:
            return -1
        
    def getConnectionStatus(self):
        error = self.sock.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR)
        return error
    