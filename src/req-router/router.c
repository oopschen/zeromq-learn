#include <zmq.h>
#include <assert.h>

int main(void)
{
  void *ctx = zmq_ctx_new();
  void *sock = zmq_socket(ctx, ZMQ_ROUTER);
  int rc = zmq_bind(sock, "tcp://127.0.0.1:5555");
  assert(0 == rc);

  char buf[128];
  int recvBytes = 0;
  while (1) {
    // recv  id
    recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
    if (0 < recvBytes) {
      // send it back
      printf("recv id bytes=%d\n", recvBytes);

      zmq_send(sock, buf, recvBytes, ZMQ_SNDMORE);
    }

    // delimiter emtpy frame
    recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
    assert(0 == recvBytes);

    recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
    // actual message
    if (0 < recvBytes) {
      // send it back
      buf[recvBytes] = '\0';
      printf("recv msg %s, bytes=%d\n", buf, recvBytes);

      zmq_send(sock, "", 0, ZMQ_SNDMORE);
      zmq_send(sock, buf, recvBytes, 0);
    }
  }

  zmq_close(sock);
  zmq_ctx_destroy(ctx);
  return 0;
}
