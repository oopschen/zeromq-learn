//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    void *context = zmq_ctx_new ();
    void *sock = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind(sock, "tcp://127.0.0.1:5555");
    if (0 != rc) {
      printf("bind error %s\n", strerror(rc));
      return 1;
    }

    char buf[24];
    while(1) {
      int recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
      if (0 < recvBytes) {
        buf[recvBytes] = '\0';
        printf("recv %s\n", buf);

        if (0 == strcmp("end", buf)) {
          break;
        }

        zmq_send(sock, "world!", 6, 0);
      }
    }

    zmq_close(sock);
    zmq_ctx_destroy (context);
    return 0;
}

