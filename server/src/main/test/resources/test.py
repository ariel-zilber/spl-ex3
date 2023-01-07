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
    time.sleep(1)

    return conn
def test1():
    logger=PrintingListener()
    conn =init_connection(username='123',password='1',logger=logger)
    conn.subscribe(destination='x',id="78")
    time.sleep(1)
    for i in range(0,len(logger.get_data())):
        print(i,logger.data[i])
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': '123', 'passcode': '********'}" in logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]
    assert "on_send SUBSCRIBE {'destination': 'x', 'id': '78', 'ack': 'auto'}" in logger.data[3]

############## login related tests
def test_login_sanity():
    logger=PrintingListener()
    conn =init_connection(username='123',password='1',logger=logger)
    time.sleep(1)
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': '123', 'passcode': '********'}" in logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]

def test_login_bad_no_host():
    logger = PrintingListener()
    conn = stomp.Connection10([('127.0.0.1', 7777)])
    conn.set_listener('', logger)
    time.sleep(1)
    conn.connect(username='meni',passcode= 'films', wait=True,headers={})
    time.sleep(1)
    for i,j in enumerate(logger.data):
        print(i,j)
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': 'meni', 'passcode': '********'}" in \
           logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]

def test_login_bad_valueof_host():
    logger = PrintingListener()
    conn = stomp.Connection10([('127.0.0.1', 7777)])
    conn.set_listener('', logger)
    time.sleep(1)
    conn.connect(username='meni',passcode= 'films', wait=True,headers={'host': "stomp1.cs.bgu.ac.il"})
    time.sleep(1)
    for i,j in enumerate(logger.data):
        print(i,j)
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': 'meni', 'passcode': '********'}" in \
           logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]
def test_login_bad_valueof_passcode():
    has_failed=False
    try:
        logger = PrintingListener()
        conn = stomp.Connection10([('127.0.0.1', 7777)])
        conn.set_listener('', logger)
        time.sleep(1)
        conn.connect(username='meni',passcode= '', wait=True,headers={'host': "stomp.cs.bgu.ac.il"})
        time.sleep(1)
        for i,j in enumerate(logger.data):
            print(i,j)
    except  Exception:
        has_failed=True
    assert has_failed

def test_login_bad_valueof_username():
    has_failed=False
    try:
        logger = PrintingListener()
        conn = stomp.Connection10([('127.0.0.1', 7777)])
        conn.set_listener('', logger)
        time.sleep(1)
        conn.connect(username='',passcode= '123', wait=True,headers={'host': "stomp.cs.bgu.ac.il"})
        time.sleep(1)
        for i,j in enumerate(logger.data):
            print(i,j)
    except  Exception:
        has_failed=True
    assert has_failed

def connect_user_helper():
    logger=PrintingListener()
    conn =init_connection(username='123',password='1',logger=logger)
    conn.disconnect()
    time.sleep(0.4)
    for i, j in enumerate(logger.data):
        print(i, j)
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': '123', 'passcode': '********'}" in logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]
    assert "on_send DISCONNECT" in logger.data[3]

def test_login_multiple_users():
    print()
    for i in range(0,10):
        print("------[",i,"]-----------")
        connect_user_helper()

def test_login_two_users_same_username():
    logger=PrintingListener()
    conn =init_connection(username='meni',password='films',logger=logger)
    time.sleep(0.4)
    for i, j in enumerate(logger.data):
        print(i, j)
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': 'meni', 'passcode': '********'}" in logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]
    should_fail=False
    try:
        logger2 = PrintingListener()
        conn2 = init_connection(username='meni', password='films', logger=logger2)
        conn2.disconnect()
        time.sleep(1)
        for i, j in enumerate(logger2.data):
            print(i, j)
    except Exception:
        should_fail=True
    conn.disconnect()
    assert should_fail
    assert "on_send DISCONNECT" in logger.data[3]



def test_login_multipletimes_same_user():
    logger=PrintingListener()
    conn =init_connection(username='meni',password='films',logger=logger)
    time.sleep(0.4)
    for i, j in enumerate(logger.data):
        print(i, j)
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': 'meni', 'passcode': '********'}" in logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]
    should_fail=False
    try:
        conn = init_connection(username='meni', password='films', logger=logger)
        for i, j in enumerate(logger.data):
            print(i, j)
    except Exception:
        should_fail=True
    conn.disconnect()
    assert should_fail

###########
def test_subscribe_sanity():
    logger=PrintingListener()
    conn =init_connection(username='123',password='1',logger=logger)
    conn.subscribe(destination='x',id="78")
    time.sleep(1)
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': '123', 'passcode': '********'}" in logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]
    assert "on_send SUBSCRIBE {'destination': 'x', 'id': '78', 'ack': 'auto'}" in logger.data[3]


def test_unsubscribe_sanity():
    logger=PrintingListener()
    conn =init_connection(username='123',password='1',logger=logger)
    conn.subscribe(destination='x',id="78")
    time.sleep(1)
    conn.unsubscribe(destination='x',id="78")
    time.sleep(1)
    assert "on_connecting 127.0.0.1 7777" in logger.data[0]
    assert "on_send CONNECT {'host': 'stomp.cs.bgu.ac.il', 'accept-version': '1.0', 'login': '123', 'passcode': '********'}" in logger.data[1]
    assert "on_connected {'version': '1.2'} " in logger.data[2]
    assert "on_send SUBSCRIBE {'destination': 'x', 'id': '78', 'ack': 'auto'}" in logger.data[3]
    assert "on_send SUBSCRIBE {'destination': 'x', 'id': '78', 'ack': 'auto'}" in logger.data[4]
