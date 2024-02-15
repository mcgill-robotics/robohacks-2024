import json
import socket


class CommunicationInterface():
    data = {}
    ip_address = '192.168.4.1'
    port = 80
    sock = None
    hasConnection = False
    jsonData = None

    def __init__(self, config_path="user_config.json"):
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
    def connectSocket(self, ip_address, port):
        self.ip_address = ip_address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(5)
        try:
            self.sock.connect((self.ip_address, self.port))
            self.hasConnection = True
            return True
        except socket.timeout:
            return False
        
    def closeSocket(self):
        try:
            self.sock.close()
            self.hasConnection = False
            return True
        except:
            return False
        
    def sendUpdates(self):
        try:
            self.createJSON()
            self.sock.sendall(self.jsonData.encode('utf8'))
            return 0
        except socket.error:
            return 1
        
    def getConnectionStatus(self):
        error = self.sock.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR)
        return error
    
    def getSockConnection(self):
        return self.hasConnection
