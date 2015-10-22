#include <pthread.h>
#include <zmq.h>
#include <assert.h>

void *reply(void * ctx)
{
  void *reply = zmq_socket(ctx, ZMQ_REP);
  assert(0 == zmq_bind(reply, "tcp://127.0.0.1:5555"));
  char buf[32];
  int recvBytes = 0;

  recvBytes = zmq_recv(reply, buf, sizeof(buf), 0);
  if (0 < recvBytes) {
    buf[recvBytes] = '\0';
    printf("[reply] recv %s\n", buf);
  }

  return NULL;
}

int main(void)
{

  void * ctx = zmq_ctx_new();
  
  pthread_t worker;
  assert(0 == pthread_create(&worker, NULL, reply, ctx));

  // router
  void *router = zmq_socket(ctx, ZMQ_ROUTER);
  assert(0 == zmq_connect(router, "tcp://127.0.0.1:5555"));
  zmq_send(router, "", 0, ZMQ_SNDMORE);
  zmq_send(router, "hello", 5, 0);

  char buf[32];
  int recvBytes = 0;
  recvBytes = zmq_recv(router, buf, sizeof(buf), 0);
  if (0 < recvBytes) {
    buf[recvBytes] = '\0';
    printf("recv %s\n", buf);
  }
  return 0;
}
