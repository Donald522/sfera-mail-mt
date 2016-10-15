//
// Created by anton on 12.10.16.
// Linux mint
//

#include <stdio_ext.h>
#include <system_error>
#include "Server.h"

Server::Server(uint16_t port) {

    std::fill(buf, buf + BUFSIZE, 0);

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFd < 0) {
        throw std::system_error(errno, std::system_category());
    }

    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons(port);
    serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);

    rc = bind(serverFd, reinterpret_cast<struct sockaddr *>(&serverSocket), sizeof(serverSocket));
    if(rc < 0) {
        throw std::system_error(errno, std::system_category());
    }
    rc = setNonblock(serverFd);
    if(rc < 0) {
        throw std::system_error(errno, std::system_category());
    }

    int optVal = 1;
    rc = setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof optVal);
    if(rc < 0) {
        throw std::system_error(errno, std::system_category());
    }

    listen(serverFd, SOMAXCONN);
    std::cout << "Server socket start listen...." << std::endl;

    epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if(epoll_fd == 0) {
        throw std::system_error(errno, std::system_category());
    }
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = serverFd;
    rc = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverFd, &ev);
    if(rc < 0) {
        throw std::system_error(errno, std::system_category());
    }
}

int Server::setNonblock(int fd) {
    int flags;
#if defined(O_NONBLOCK)
    if(-1 == (flags = fcntl(fd, F_GETFL, 0))) {
        flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}

void Server::start() {
    while(true) {
        int eventsNumber = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if(eventsNumber == -1) {
            throw std::system_error(errno, std::system_category());
        }

        for(int i = 0; i < eventsNumber ; i++) {
            if (events[i].data.fd == serverFd) {
                accept_fd = accept(serverFd, (sockaddr *) &clientSocket, (socklen_t *) &len);
                if (accept_fd < 0) {
                    std::cout << "Can't accept the clientSocket" << std::endl;
                    continue;
                }
                std::cout << "accepted connection" << std::endl;
                send(accept_fd, welcomeMessage.data(), welcomeMessage.length(), 0);
                // std::string msg = "client " + std::to_string(accept_fd) + " joined\n";
                // for (auto client : clients) {
                //     send(client, msg.data(), msg.length(), 0);
                // }
                setNonblock(accept_fd);
                clients.insert(accept_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = accept_fd;
                rc = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &ev);
                if (rc < 0) {
                    throw std::system_error(errno, std::system_category());
                }
            } else if (events[i].events & EPOLLIN) {
                bool firstPacket = true;
                std::string pre = "clientSocket " + std::to_string(events[i].data.fd) + ": ";
                // while(1) {
                    ret = recv(events[i].data.fd, buf, sizeof(buf), 0);
                    if (ret <= 0) {
                        std::cout << "connection terminated" << std::endl;
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
                        // break;
                    } else {
                        std::string message;
                        if(buf[ret - 1] != '\n') {
                            std::string text(buf, static_cast<unsigned long>(ret));
                            auto it = messages.find(events[i].data.fd);
                            if(it != messages.end()) {
                                message += text;
                                it->second += message;
                            } else {
                                message += pre;
                                message += text;
                                messages.insert(std::pair<int, std::string>(static_cast<int>(events[i].data.fd), message));
                            }
                        } else {
                            std::string text(buf, static_cast<unsigned long>(ret));
                            auto it = messages.find(events[i].data.fd);
                            std::string toClients;
                            if(it != messages.end()) {
                                message += text;
                                it->second += message;
                                toClients = it->second;
                                std::cout << it->second << std::endl;
                                messages.erase(it);
                            } else {
                                message += pre;
                                message += text;
                                toClients = message;
                                std::cout << message << std::endl;
                            }
                            for (auto client : clients) {
                                send(client, toClients.data(), toClients.length(), MSG_NOSIGNAL);
                            }
                            // break;
                        }
                    }
                // }
            }
        }
    }
}
