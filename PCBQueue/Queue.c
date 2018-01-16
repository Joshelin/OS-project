#include "Queue.h"

void insertProcQ(pcb_t **head, pcb_t *p){
	if (*head == NULL) {
		*head = p;
	} else if (*head->priority <= p->priority){
		p->p_next = *head;
		*head = p;
	}
}