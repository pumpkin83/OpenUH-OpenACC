typedef void* acc_queue;

acc_queue acc_create_queue(void);

void acc_destroy_queue(acc_queue q);

void acc_queue_push(acc_queue q, void* value);

void acc_queue_



