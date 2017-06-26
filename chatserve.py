'''
Monica Pineda 
CS 372
May 7, 2017
Program 1: This is a client-server chat program where the server side is implemented in
Python and the client side is implemented in C++.
chatserve.py: is the server side of this program. once started it will wait for a client
to connect and then the conntected users can alternate sending messages.
'''
'''
    Title: Computer netowrking Top-Down Approach 
    Author: James Kurose & Keith Ross 
    Date: 2017
    Code version: pgs 168-170 and pg 195 and the resource website 
    Availability: class book and https://media.pearsoncmg.com/aw/ecs_kurose_compnetwork_7/cw/index.php
'''
from socket import *
import sys

def chat(connectionSocket, clientName, userName):
    '''Chat initiates a chart session with the client 
    	and lets them send the first messages
  		'''
    sent = ""
    while 1:
        # continue chat until peers choose to stop chatting
        received = connectionSocket.recv(501)[0:-1]
        # if the sever receives nothing, print connection closed and close connection
        if received == "":
            print "Connection closed"
            print "Waiting for new connection"
            break
        # print the clients name with their messages
        print "{}> {}".format(clientName, received)
        # grab input on the server side to send to the client
        sent = ""
        while len(sent) == 0 or len(sent) > 500:
            sent = raw_input("{}> ".format(userName))
            # send it to the client if the message is not \quit
        if sent == "\quit":
            print "Connection closed"
            print "Waiting for new connection"
            break
        connectionSocket.send(sent)

def handshake(connectionSocket, userName):
    ''' 
    handshake exchanges userNames with the incoming connection
    '''
    # get the client's name
    clientName = connectionSocket.recv(1024)
    # send server's userName to the client
    connectionSocket.send(userName)
    return clientName

if __name__ == "__main__":
    # If the number of arguments entered by the client is wrong, exit
    if len(sys.argv) != 2:
        print "You must specify the port number for the server to run"
        exit(1)
    # get the port number from the user and create a TCP socket
    serverport = sys.argv[1]
    serversocket = socket(AF_INET, SOCK_STREAM)
    # bind the socket to the port specified by the user
    serversocket.bind(('', int(serverport)))
    # listen on the port for incoming messages
    serversocket.listen(1)
    # ask the user for their name, must be less than 11 characters
    userName = ""
    while len(userName) == 0 or len(userName) > 10:
        userName = raw_input("Please enter a username (up to 10 characters):")
        print "The server is ready to receive incoming messages"
    while 1:
        # keep doing this until signal is interupted
        # create a new socket if there is an incoming connection
        connectionSocket, address = serversocket.accept()
        # print that we have received a connection
        print "received connection on address {}".format(address)
        # chat with the incoming connection, handshake with them first
        chat(connectionSocket, handshake(connectionSocket, userName), userName)
        # close the connection when we are done chatting
        connectionSocket.close()