
#ifndef ELOOP_H
#define ELOOP_H

#ifndef EL_MAX_WORK
#define EL_MAX_WORK 1024
#endif

typedef enum {
#define X(t) EL_WORK_ ## t
  X(ERROR) = -1,
  X(PENDING) = 0,
  X(COMPLETE) = 1
#undef X
} el_work_status_t;

typedef el_work_status_t (*el_work_handle_t) (void *);

typedef struct el_work {
  el_work_handle_t handle;
  void *data;
} el_work_t;

typedef struct eloop {
  el_work_t *work[EL_MAX_WORK];
  size_t length;
} el_t;

static el_t *EL_DEFAULT_LOOP = NULL;

int
el_init (void);

el_t *
el_alloc (void);

void
el_free (el_t *);

el_work_t *
el_work_init (void *, el_work_handle_t);

void
el_work_destroy (el_work_t *);

size_t
el_enqueue (el_t *, void *, el_work_handle_t);

size_t
el_dequeue (el_t *);

int
el_run (el_t *);

#endif
