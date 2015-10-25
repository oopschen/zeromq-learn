#include <pthread.h>
#include <zmq.h>
#include <assert.h>
#include <stdlib.h>

struct RepContext {
  int id;
  int start_port;
  void * ctx;
};

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

  int threadCnt = 10;
  int startPort = 10240;
  struct RepContext * threads = malloc(sizeof(struct RepContext) * threadCnt);
  if (NULL == threads) {
    printf("mom!\n");
    return 1;
  }

  pthread_t * workers = malloc(sizeof(pthread_t) * threadCnt);
  if (NULL == workers) {
    printf("mom!\n");
    return 1;
  }

  void * ctx = zmq_ctx_new();
  
  for (int i = 0; i < threadCnt; i++) {
    threads[i].id = i;
    threads[i].ctx = ctx;
    threads[i].start_port = startPort;
    
    assert(0 == pthread_create(workers + i, NULL, reply, threads + i));
  }

  // router
  void *router = zmq_socket(ctx, ZMQ_DEALER);
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
