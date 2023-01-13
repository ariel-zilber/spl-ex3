import sys
import time

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

class MyListener(stomp.ConnectionListener):
    def __init__(self, conn):
        self.conn = conn

    def on_error(self, frame):
        print('received an error "%s"' % frame.body)

    def on_message(self, frame):
        print('received a message "%s"' % frame.body)
        for x in range(10):
            print(x)
            time.sleep(1)
        print('processed message')


conn = stomp.Connection10([('127.0.0.1', 7777)])
conn.set_listener('', stomp.PrintingListener())

conn.connect('admin', 'password', wait=True, headers={
    'host': "stomp.cs.bgu.ac.il"})
conn.subscribe(destination='x', id="78")

while True:
    print(conn.subscribe(destination='x', id="778"))
    time.sleep(1)

print(conn.disconnect())
