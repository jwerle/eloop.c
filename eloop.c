
#include <stdlib.h>
#include "eloop.h"

el_t *
el_alloc (void) {
  el_t *self = (el_t *) malloc(sizeof(el_t));
  if (NULL == self) { return NULL; }
  self->length = 0;
  return self;
}

void
el_free (el_t *self) {
  if (NULL == self) { return; }
  el_work_t *work = NULL;
  while (self->length) {
    work = self->work[self->length--];
    el_work_destroy(work);
  }
  free(self);
}

size_t
el_enqueue (el_t *self, void *data, el_work_handle_t handle) {
  el_work_t *work = el_work_init(data, handle);
  if (NULL == self) { return -1; }
  if (self->length < EL_MAX_WORK) {
    self->work[self->length++] = work;
  }
  return self->length;
}

size_t
el_dequeue (el_t *self) {
  int length = 0;
  int i = 0;
  el_work_status_t status = 0;
  el_work_t *work = NULL;
  if (NULL == self) { return -1; }
  length = self->length;
  for (; i < length; ++i) {
    work = self->work[i];
    status = work->handle(work->data);
    switch (status) {
      case EL_WORK_ERROR: goto error;

      case EL_WORK_PENDING:
        break;

      case EL_WORK_COMPLETE:
        self->work[i] = NULL;
        self->length--;
        el_work_destroy(work);
        break;
    }
  }

okay: { return self->length; }
error:
  // @TODO - handle with error
  return -1;
}

el_work_t *
el_work_init (void *data, el_work_handle_t handle) {
  el_work_t *work = (el_work_t *) malloc(sizeof(el_work_t));
  if (NULL == work) { return NULL; }
  work->data = data;
  work->handle = handle;
  return work;
}

void
el_work_destroy (el_work_t *self) {
  if (NULL != self) { free(self); }
}

int
el_run (el_t *self) {
  int status = 0;
  if (NULL == self) { return 1; }
  while (1) {
    status = el_dequeue(self);
    if (-1 == status) {
      // @TODO - handle with error
      return 1;
    } else if (0 == status) {
      return 0;
    }
  }
  return status;
}
