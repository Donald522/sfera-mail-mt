//
// Created by anton on 12.10.16.
//

#ifndef CHATSERVER_SERVER_H
#define CHATSERVER_SERVER_H


#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <set>
#include <stdexcept>

#define BUFSIZE 1024
#define MAX_EVENTS 64

class Server {
public:
    Server(uint16_t port);
    void start();
private:
    int setNonblock(int fd);

private:
    int epoll_fd;
    ssize_t ret = 1;
    int accept_fd;
    struct epoll_event ev;
    struct epoll_event events[5];
    char buf[BUFSIZE];
    struct sockaddr_in serverSocket;
    struct sockaddr_in clientSocket;
    int len = sizeof clientSocket;
    int serverFd;
    int rc;
    std::set<int> clients;
    std::string welcomeMessage = "Welcome!\n";
};


#endif //CHATSERVER_SERVER_H
