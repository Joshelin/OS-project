#include "Alloc.h"

// Non posso farlo ricorsivo senza parametri.
void initPcbs() {
	int i=0;
	pcb_t *temp = *pcbfree_h;
	for(i; i < MAXPROC-1; i++){
		*temp = pcbFree_table[i];
		temp->p_next = pcbFree_table[i+1];
	}
}