//
// Created by anton on 12.10.16.
// Linux mint
//

#include <system_error>
#include "Client.h"

Client::Client(std::string address, uint16_t port) {
    clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientFd < 0) {
        throw std::system_error(errno, std::system_category());
    }
    clientSocket.sin_family = AF_INET;
    clientSocket.sin_addr.s_addr = inet_addr(address.data());
    clientSocket.sin_port = htons(port);

    tv.tv_sec = 600;
    tv.tv_usec = 0;
}

void Client::connect() {
    int rc = ::connect(clientFd, reinterpret_cast<struct sockaddr *>(&clientSocket), sizeof(clientSocket));
    if(rc < 0) {
        throw std::system_error(errno, std::system_category());
    }
}

void Client::start() {
    bool isConnected = true;
    while(isConnected) {
        FD_ZERO(&readfd);
        FD_SET(0, &readfd);
        FD_SET(clientFd, &readfd);
        if((select(clientFd+1, &readfd, NULL, NULL, &tv)) < 0) {
            throw std::system_error(errno, std::system_category());
        }
        if(FD_ISSET(0, &readfd)) {
            message = getline();
            messageLength = strlen(message);
            int sent = 0;
            while (sent < messageLength) {
                if ((n = send(clientFd, message+sent, std::min(strlen(message + sent), packetSize), 0)) <= 0) {
                    isConnected = false;
                    break;
                }
                sent += n;
            }
            free(message);
        }
        if(FD_ISSET(clientFd, &readfd)) {
            if((n = recv(clientFd, buffer, BUFSIZE-1, MSG_NOSIGNAL)) <=0 ) {
                throw std::system_error(errno, std::system_category());
            }
            buffer[n] = '\0';
            write(STDOUT_FILENO, buffer, static_cast<size_t> (n));
            fflush(stdout);
        }
    }
    close(clientFd);
}

char *Client::getline() {
    size_t N = 1;
    char *line = new char[N];
    char *begin = line, *new_line;
    while(true) {
        if(line <= begin + (N - 1)) {
            if(std::cin.get(*line) && *(line) != std::cin.eof()) {
                if(*line == '\n') {
                    break;
                }
                line++;
            }
            else {
                break;
            }
        }
        else {
            new_line = new char[N*2];
            for(line = begin; line < begin + N; *(new_line++) = *(line++))
                ;
            delete [] begin;
            begin = new_line - N;
            line = new_line;
            N *= 2;
        }
    }
    *(++line) = '\0';
    return begin;
}
