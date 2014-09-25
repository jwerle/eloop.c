
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>
#include "eloop.h"

static el_t *loop = NULL;
static int i = 0;

static int
work1 (void *data) {
  assert(NULL == data);
  printf("work1!\n");
  if (++i > 2) {
    return EL_WORK_COMPLETE;
  } else {
    return EL_WORK_PENDING;
  }
}

static int
work2 (void *data) {
  printf("hello %s\n", (char *) data);
  return EL_WORK_COMPLETE;
}

static void *
work_in_thread (void *data) {
  int status = EL_WORK_COMPLETE;
  printf("message from thred '%s'\n", (char *) data);
  return (void *) (intptr_t) status;
}

static int
work_with_thread (void *data) {
  void *status = NULL;
  pthread_t thread;
  assert(0 == pthread_create(&thread, NULL, data, "work in thread"));
  assert(0 == pthread_join(thread, &status));
  return (int) status;
}

int
main (int argc, char **argv) {
  loop = el_alloc();
  assert(loop);
  assert(1 == el_enqueue(loop, NULL, work1));
  assert(2 == el_enqueue(loop, "world", work2));
  assert(3 == el_enqueue(loop, work_in_thread, work_with_thread));
  return el_run(loop);
}
