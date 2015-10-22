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
    // recv message 
    
    
    // send it back

  }

  zmq_close(sock);
  zmq_ctx_destroy(ctx);
  return 0;
}
