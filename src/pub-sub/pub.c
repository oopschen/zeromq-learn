#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *sock = zmq_socket (context, ZMQ_PUB);
    int rc = zmq_bind(sock, "tcp://127.0.0.1:5555");
    assert (rc == 0);

    // must use while loop, as pub-sub socket do not keep message when the other end have not start up.
    while(1) {
      zmq_send (sock, "categoryA", 9, ZMQ_SNDMORE);
      zmq_send (sock, "hello A", 7, 0);
      zmq_send (sock, "categoryB", 9, ZMQ_SNDMORE);
      zmq_send (sock, "hello", 5, 0);
    }

    zmq_close (sock);
    zmq_ctx_destroy (context);
    return 0;
}
