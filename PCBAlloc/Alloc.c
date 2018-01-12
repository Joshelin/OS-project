#include "Alloc.h"
// variabili globali qui
int i = 0;
pcb_t *temp ; // Così piace a gcc, non sono sicuro al 100% del motivo, ma senza il cast a puntatore pcb_t non gli piaceva.

void initPcbs() {
	// Solo per lista ancora vuota.
	if (i == 0){
		pcbfree_h = &pcbFree_table[i++];
		temp = pcbfree_h ;
		temp->p_next = &pcbFree_table[i++];
		temp = temp->p_next;
	}
	if (i < MAXPROC) {  
		temp->p_next = &pcbFree_table[i++];
		temp = temp->p_next;
		initPcbs();
	}
}

int main(int argc, char* argv[]){

	initPcbs();
}