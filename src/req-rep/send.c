#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *sock = zmq_socket (context, ZMQ_REQ);
    int rc = zmq_connect(sock, "tcp://localhost:5555");
    assert (rc == 0);

    char buf[24];
    int cnt = 0;
    while (cnt ++ < 10) {
        zmq_send (sock, "hello", 5, 0);
        printf ("send hello\n");
        
        int recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
        if (0 < recvBytes) {
          buf[recvBytes] = '\0';
          printf("recv word: %s\n", buf);
        }
    }

    zmq_send (sock, "end", 3, 0);

    zmq_close (sock);
    zmq_ctx_destroy (context);
    return 0;
}
