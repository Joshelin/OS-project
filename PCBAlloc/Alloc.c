#include "Alloc.h"
// variabili globali qui
int i = 0;
pcb_t *temp = (pcb_t *)&pcbfree_h; // Così piace a gcc, non sono sicuro al 100% del motivo, ma senza il cast a puntatore pcb_t non gli piaceva.

void initPcbs() {
	// Solo per lista ancora vuota.
	if (i == 0){
		temp = &pcbFree_table[i++];
		temp->p_next = &pcbFree_table[i];
		temp = temp->p_next;
		i += 2;
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