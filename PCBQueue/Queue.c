#include "Queue.h"

pcb_t *temp ;

void insertProcQ(pcb_t **head, pcb_t *p){ 
	if (*head == NULL) { //lista vuota
		*head = p;
		temp = *head ;
	}
	else if ((*head)->p_next == NULL){ //lista con un solo elemento 
		if ((*head)->priority <= p->priority){
			p->p_next = *head;
			*head = p;
		}
		else {
			(*head)->p_next = p ;			
		}
	}
	else {
		if ((*head)->priority <= p->priority){
			p->p_next = *head;
			*head = p;
		}
		else if ((*head)->p_next->priority <= p->priority){
			p->p_next = (*head)->p_next ;
			(*head)->p_next = p ;
		}
		else { 
			temp = temp->p_next ;
			insertProcQ(&temp,p);
		}

	}
		
}