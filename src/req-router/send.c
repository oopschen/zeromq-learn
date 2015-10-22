#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    if (1 > argc) {
      printf("input id\n");
      return 1;
    }
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *sock = zmq_socket (context, ZMQ_REQ);
    int rc = zmq_connect(sock, "tcp://localhost:5555");
    assert (rc == 0);

    char buf[24];
    char * sendPrefix = "hello";
    int sendBufSize = strlen(sendPrefix) + strlen(argv[0]);
    char * sendBuf = malloc(sendBufSize);
    assert(NULL != sendBuf);

    int sendBufActualSize = snprintf(sendBuf, sendBufSize, "%s%s", sendPrefix, argv[0]);
    assert(0 < sendBufActualSize);

    int cnt = 0;
    while (cnt ++ < 10) {
        zmq_send (sock, sendBuf, sendBufActualSize, 0);
        printf ("send hello\n");
        
        int recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
        if (0 < recvBytes) {
          buf[recvBytes] = '\0';
          printf("recv word: %s\n", buf);
        }
    }

    free(sendBuf);

    zmq_close (sock);
    zmq_ctx_destroy (context);
    return 0;
}
