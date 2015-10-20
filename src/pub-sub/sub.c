#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *sock = zmq_socket (context, ZMQ_SUB);
    int rc = zmq_connect(sock, "tcp://127.0.0.1:5555");
    assert(rc == 0);
    zmq_setsockopt(sock, ZMQ_SUBSCRIBE, "categoryA", 9);

    // recv
    char buf[32];
    int recvBytes = 0;
    recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
    if (0 < recvBytes) {
      buf[recvBytes] = '\0';
      printf("recv address %s\n", buf);
    }

    recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
    if (0 < recvBytes) {
      buf[recvBytes] = '\0';
      printf("recv message %s\n", buf);
    }

    zmq_close (sock);
    zmq_ctx_destroy (context);
    return 0;
}
