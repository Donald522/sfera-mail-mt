
#include "Client.h"

int main(int argc, char *argv[]) {

    Client client("127.0.0.1", 9002);
    client.connect();
    client.start();

    return 0;
}
