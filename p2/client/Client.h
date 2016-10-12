//
// Created by anton on 12.10.16.
//

#ifndef CHATCLIENT_CLIENT_H
#define CHATCLIENT_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>

class Client {
public:
    Client(std::string address, uint16_t port);
    void connect();
    void start();
private:
    char *getline();
private:
    struct sockaddr_in clientSocket;
    int clientFd;
    size_t packetSize = 1024;
    ssize_t n;
    char *message;
    size_t messageLength;
    static const size_t BUFSIZE = 8192;
    char buffer[BUFSIZE];
    struct timeval tv;
    fd_set readfd;
};


#endif //CHATCLIENT_CLIENT_H
