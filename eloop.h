
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

/**
 * Worker function with data passed to it
 */

typedef el_work_status_t (*el_work_handle_t) (void *);

/**
 * Work structure with function handle and data
 */

typedef struct el_work {
  el_work_handle_t handle;
  void *data;
} el_work_t;

/**
 * Event loop arbiter
 */

typedef struct eloop {
  el_work_t *work[EL_MAX_WORK];
  size_t length;
} el_t;

/**
 * Allocates a new `el_t' type
 */

el_t *
el_alloc (void);

/**
 * Frees an event loop
 */

void
el_free (el_t *);

/**
 * Initializes work with argument and function handle
 */

el_work_t *
el_work_init (void *, el_work_handle_t);

/**
 * Destroys work. Does not destroy argument pointer
 */

void
el_work_destroy (el_work_t *);

/**
 * Enqueues work with argument and handle
 */

size_t
el_enqueue (el_t *, void *, el_work_handle_t);

/**
 * Dequeues all queued work
 */

size_t
el_dequeue (el_t *);

/**
 * Runs the event loop
 */

int
el_run (el_t *);

#endif
