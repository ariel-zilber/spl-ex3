import sys
import time
import stomp.utils as utils

import stomp
"""
server frame
• CONNECTED (as defined above)
• MESSAGE
• RECEIPT
• ERROR

Client frames:
• CONNECT (as defined above)
• SEND
• SUBSCRIBE
• UNSUBSCRIBE
• DISCONNECT
"""
def connect_and_subscribe(conn):
    conn.connect('guest', 'guest', wait=True)
    conn.subscribe(destination='/queue/test', id=1, ack='auto')

class PrintingListener(stomp.ConnectionListener):
    def __init__(self, ):
        self.data=[]
    def __print(self, msg, *args):
        self.data.append(msg%args)
    def on_connecting(self, host_and_port):
        """
        :param (str,int) host_and_port:
        """
        self.__print("on_connecting %s %s", *host_and_port)


    def on_connected(self, frame):
        """
        :param Frame frame: the stomp frame
        """
        self.__print("on_connected %s %s", frame.headers, frame.body)

    def on_disconnected(self):
        self.__print("on_disconnected")

    def on_heartbeat_timeout(self):
        self.__print("on_heartbeat_timeout")

    def on_before_message(self, frame):
        """
        :param Frame frame: the stomp frame
        """
        self.__print("on_before_message %s %s", frame.headers, frame.body)

    def on_message(self, frame):
        """
        :param Frame frame: the stomp frame
        """
        self.__print("on_message %s %s", frame.headers, frame.body)

    def on_receipt(self, frame):
        """
        :param Frame frame: the stomp frame
        """
        self.__print("on_receipt %s %s", frame.headers, frame.body)

    def on_error(self, frame):
        """
        :param Frame frame: the stomp frame
        """
        self.__print("on_error %s %s", frame.headers, frame.body)

    def on_send(self, frame):
        """
        :param Frame frame: the stomp frame
        """
        self.__print("on_send %s %s %s", frame.cmd, utils.clean_headers(frame.headers), frame.body)

    def on_heartbeat(self):
        self.__print("on_heartbeat")
    def get_data(self):
        return  self.data
def init_connection(logger=None,username='admin',password='password',headers={'host': "stomp.cs.bgu.ac.il"}):
    conn = stomp.Connection10([('127.0.0.1', 7777)])
    conn.set_listener('', logger)
    conn.connect(username, password, wait=True, headers={'host': "stomp.cs.bgu.ac.il"})

    return conn
def test1():
    logger=PrintingListener()
    conn =init_connection(username='123',password='1',logger=logger)
    conn.subscribe(destination="/dest")
    conn.send(destination="/dest2",id="2",body='xxx')
    conn.disconnect()
    for i in range(0,len(logger.get_data())):
        print(i,logger.data[i])
