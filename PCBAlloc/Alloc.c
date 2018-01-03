#include "Alloc.h"
// variabili globali qui
int i = 0;
pcb_t *temp = pcbfree_h;
/* Ricorsivo v1, da controllare.
Scorro lista pcbfree_h tramite temp.*/
void initPcbs() {
	if (i < MAXPROC) {  
		temp = pcbFree_table[i];
		temp = temp->p_next;
		i++;
		initPcbs();
	}
}