#include <pthread.h>
#include <zmq.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct RepContext {
  int id;
  int start_port;
};

void *reply(void * ctx)
{
  struct RepContext * rctx = (struct RepContext *)ctx;
  void *zctx = zmq_ctx_new();
  void *reply = zmq_socket(zctx, ZMQ_REP);
  char * prefix = "tcp://127.0.0.1:";
  char * addr = malloc(strlen(prefix) + 10);
  if (NULL == addr) {
    return NULL;
  }

  int addr_size = snprintf(addr, (strlen(prefix) + 10), "%s%d", prefix, rctx->start_port);
  assert(addr_size > strlen(prefix));

  assert(0 == zmq_bind(reply, addr));
  char buf[32];
  int recvBytes = 0;

  recvBytes = zmq_recv(reply, buf, sizeof(buf), 0);
  if (0 < recvBytes) {
    buf[recvBytes] = '\0';
    printf("[recv%d] recv %s\n", rctx->id, buf);

    sleep(10 - rctx->id);
    char * reply_buf = malloc(strlen(buf) + 30);
    if (NULL != reply_buf) {
      int rsize = snprintf(reply_buf, strlen(buf) + 30, "%s%d", buf, rctx->id);
      zmq_send(reply, reply_buf, rsize, 0);
    }
  }

  zmq_close(reply);
  zmq_ctx_destroy(zctx);

  return NULL;
}

int main(void)
{

  int threadCnt = 2;
  int startPort = 3000;
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
    threads[i].start_port = startPort + i;
    
    assert(0 == pthread_create(workers + i, NULL, reply, threads + i));
  }

  // sock
  void *sock = zmq_socket(ctx, ZMQ_DEALER);
  char * addr_prefix = "tcp://127.0.0.1:";
  char * addr = malloc(strlen(addr_prefix) + 10);
  if (NULL == addr) {
    printf("mom!\n");
    return 1;
  }

  // connect to multiple relpy
  printf("dealer connecting...\n");
  for (int i = 0; i < threadCnt; ++i) {
    int addr_s = snprintf(addr, (strlen(addr_prefix) + 10),  "%s%d", addr_prefix, threads[i].start_port);
    assert(addr_s > strlen(addr_prefix));
    assert(0 == zmq_connect(sock, addr));
  }
  free (addr);

  // send msg
  printf("dealer sending msg...\n");
  for (int i = 0; i < threadCnt; ++i) {
    zmq_send(sock, "", 0, ZMQ_SNDMORE);
    zmq_send(sock, "hello", 5, 0);
  }
    
  char buf[32];
  int recvBytes = 0;
  int recvCnt = 0;
  printf("dealer receiving msg...\n");
  while (recvCnt < threadCnt) {
    recvBytes = zmq_recv(sock, buf, sizeof(buf), 0);
    if (0 < recvBytes) {
      buf[recvBytes] = '\0';
      printf("[dealer] recv %s\n", buf);
      recvCnt ++;
    } else {
      printf("[dealer] recv %d\n", recvBytes);
    }
  }

  zmq_close(sock);
  zmq_ctx_destroy(ctx);
  return 0;
}
