/*
 * queue.c
 *
 *  Created on: 2018. 11. 6.
 *      Author: kdj6724
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dev/data/queue.h"

int init_queue(Queue* queue) {
  queue->front = NULL;
  queue->rear = NULL;
  queue->count = 0;
  return 0;
}
int is_empty(Queue* queue) {
  return (queue->front == NULL);
}
int is_full(Queue* queue) {
  return (queue->count >= QUEUE_MAX_DATA);
}
void enqueue(Queue* queue, char* data, int len) {
  Node* newNode;

  if (is_full(queue))
    return;
  newNode = (Node*) malloc(sizeof(Node));
  newNode->next = NULL;
  newNode->data = (char*) malloc(len+1);
  memset(newNode->data, 0, len+1);
  memcpy(newNode->data, data, len);

  if (queue->rear == NULL)
    queue->front = newNode;
  else
    queue->rear->next = newNode;

  queue->rear = newNode;
  queue->count++;
}
int dequeue(Queue* queue, char* data) {
  Node* tempPtr;
  int len = 0;
  if (is_empty(queue)) {
    return 0;
  }
  if (data == NULL)
    return -1;

  tempPtr = queue->front;
  memcpy(data, tempPtr->data, strlen(tempPtr->data));
  len = strlen(data);
  queue->front = tempPtr->next;
  if (tempPtr->next == NULL)
    queue->rear = NULL;
  free(tempPtr->data);
  free(tempPtr);
  queue->count--;

  return len;
}
