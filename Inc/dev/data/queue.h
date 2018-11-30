/*
 * queue.h
 *
 *  Created on: 2018. 11. 6.
 *      Author: kdj6724
 */

#ifndef DEV_DATA_QUEUE_H_
#define DEV_DATA_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define QUEUE_MAX_DATA  32
typedef struct Node {
  char* data;
  struct Node *next;
}Node;

typedef struct Queue {
  Node* front;
  Node* rear;
  int count;
}Queue;

int init_queue(Queue* queue);
int is_empty(Queue* queue);
int is_full(Queue* queue);
void enqueue(Queue* queue, char* data, int len);
int dequeue(Queue* queue, char* data);

#ifdef __cplusplus
}
#endif

#endif /* DEV_DATA_QUEUE_H_ */
