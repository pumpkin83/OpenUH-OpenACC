#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"
#include "synch.h"

#define INITIAL_SIZE 1024

typedef struct _hashmap_element{
	int key;
	int in_use;
	any_t data;
} hashmap_element;

typedef struct _hashmap_map{
	int table_size;
	int size;
	hashmap_element *data;
	semaphore_t lock;
} hashmap_map;

/*Return an empyty hashmap, or NULL on failure*/
map_t hashmap_new()
{
	hashmap_map* m = (hashmap_map*)malloc(sizeof(hashmap_map));
	if(!m) goto err;

	m->data = (hashmap_element*)calloc(INITIAL_SIZE, sizeof(hashmap_element));
	if(!m->data) goto err;

	m->lock = (semaphore_t)semaphore_create();
	if(!m->lock) goto err;

	semaphore_initialize(m->lock, 1);

	m->table_size = INITIAL_SIZE;
	m->size = 0;

	return m;

	err:
		if(m)
			hashmap_free(m);
		return NULL;
}

/* Hashing function for an integer */
unsigned int hashmap_hash_int(hashmap_map* m, unsigned int key)
{
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);
	key = (key >> 3) * 2654435761;

	return key%(m->table_size);
}

/*Return the integer of the location in data to store 
* the point to the item, or MAP_FULL. */
int hashmap_hash(map_t in, int key)
{
	int curr;
	int i;

	/*cast the hashmap*/
	hashmap_map *m = (hashmap_map*)in;

	/*If full, return immediately*/
	if(m->size == m->table_size) return MAP_FULL;

	/* Find the best index */
	curr = hashmap_hash_int(m, key);

	/* linear probling */
	for(i=0; i<m->table_size; i++)
	{
		if(m->data[curr].in_use == 0)
			return curr;

		if(m->data[curr].key == key && m->data[curr].in_use == 1)
			return curr;

		curr = (curr + 1) % m->table_size;
	}

	return MAP_FULL;
}

/* double the size of the hashmap, and rehashes all the elements */
int hashmap_rehash(map_t in)
{
	int i;
	int old_size;
	hashmap_element* curr;

	/* set up the new elements */
	hashmap_map *m = (hashmap_map*)in;
	hashmap_element *temp = (hashmap_element*)calloc(2*m->table_size, sizeof(hashmap_element));
	if(!temp)
		return MAP_OMEM;

	/* update the array */
	curr = m->data;
	m->data = temp;

	/* update the size */
	old_size = m->table_size;
	m->table_size = 2*m->table_size;
	m->size = 0;

	/* rehash the elements */
	for(i=0; i<old_size; i++)
	{
		int status = hashmap_put(m, curr[i].key, curr[i].data);
		if(status != MAP_OK)
			return status;
	}

	free(curr);
	return MAP_OK;
}

/* add a pointer to the hashmap with some key */
int hashmap_put(map_t in, int key, any_t value)
{
	int index;
	hashmap_map *m;

	/*cast the hashmap */
	m = (hashmap_map*)in;

	/*lock for concurrency*/
	semaphore_P(m->lock);

	/* find a place to put our value*/
	index = hashmap_hash(in, key);
	while(index == MAP_FULL)
	{
		if(hashmap_rehash(in) == MAP_OMEM)
		{
			semaphore_V(m->lock);
			return MAP_OMEM;
		}
		index = hashmap_hash(in, key);
	}

	m->data[index].data = value;
	m->data[index].key = key;
	m->data[index].in_use = 1;
	m->size++;

	/*unlock*/
	semaphore_V(m->lock);

	return MAP_OK;
}

/* get your pointer out of the hashmap with a key */
int hashmap_get(map_t in, int key, any_t* arg)
{
	int curr;
	int i;
	hashmap_map *m;

	m = (hashmap_map*)in;

	/* lock for concurrency */
	semaphore_P(m->lock);

	/* find data location */
	curr = hashmap_hash_int(m, key);

	/* linear probling, if necessary */
	for(i=0; i<m->table_size; i++)
	{
		if(m->data[curr].key == key && m->data[curr].in_use == 1)
		{
			*arg = (any_t*)(m->data[curr].data);
			semaphore_V(m->lock);
			return MAP_OK;
		}
		curr = (curr + 1) % m->table_size;
	}

	arg = NULL;
	semaphore_V(m->lock);

	return MAP_MISSING;
}

/*
 * Iterate the function parameter over each element in the hashmap. 
 * The additional any_t argument is passed to the function as its first
 * argument and the hashmap element is the second.
 */
int hashmap_iterate(map_t in, PFany f, any_t item)
{
	int i;
	hashmap_map *m = (hashmap_map*)in;

	/* on empty hashmap, return immediately*/
	if(hashmap_length(m) <= 0)
		return MAP_MISSING;

	/*lock for concurrency*/
	semaphore_P(m->lock);

	/* linear probing */
	for(i=0; i<m->table_size; i++)
	{
		if(m->data[i].in_use != 0)
		{
			any_t data = (any_t)(m->data[i].data);
			int status = f(item, data);
			if(status != MAP_OK)
			{
				semaphore_V(m->lock);
				return status;
			}
		}
	}

	semaphore_V(m->lock);
	return MAP_OK;
}

/*
 * remove an element with that key from the map 
 */
int hashmap_remove(map_t in, int key)
{
	int i;
	int curr;
	hashmap_map *m;

	m = (hashmap_map*)in;

	semaphore_P(m->lock);

	curr = hashmap_hash_int(m, key);

	for(i=0; i<m->table_size; i++)
	{
		if(m->data[curr].key == key && m->data[curr].in_use == 1)
		{
			/*blank out the fields*/
			m->data[curr].in_use = 0;
			m->data[curr].data = NULL;
			m->data[curr].key = 0;

			/*reduce the size*/
			m->size--;
			semaphore_V(m->lock);
			return MAP_OK;
		}
		
		curr = (curr + 1) % m->table_size;
	}
	
	semaphore_V(m->lock);

	return MAP_MISSING;
}

/* deallocate the hashmap */
void hashmap_free(map_t in)
{
	hashmap_map *m = (hashmap_map*)in;
	free(m->data);
	semaphore_destroy(m->lock);
}

/* return the length of the hashmap */
int hashmap_length(map_t in)
{
	hashmap_map *m = (hashmap_map*)in;
	if(m != NULL)
		return m->size;
	else
		return 0;
}

