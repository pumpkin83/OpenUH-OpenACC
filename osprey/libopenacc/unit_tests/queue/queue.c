
typedef struct _qnode_t
{
	void *value;
	struct _qnode_t *next;
}_qnode_t;

typedef struct
{
	_qnode_t *head;
	_qnode_t *tail;
	int size;
}_queue_t;
