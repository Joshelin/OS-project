#include "Alloc.h"
// variabili globali qui
int i = 0;
pcb_t *temp = pcbfree_h;
/* Ricorsivo v1, da controllare.
Scorro lista pcbfree_h tramite temp.*/
void initPcbs() {
	// Solo per lista ancora vuota.
	if (i == 0){
		temp = &pcbFree_table[i];
		temp->p_next = &pcbFree_table[i+1];
		temp = temp->p_next;
		i += 2;
	}
	if (i < MAXPROC) {  
		temp->p_next = &pcbFree_table[i];
		temp = temp->p_next;
		i++;
		initPcbs();
	}
}

int main(int argc, char* argv[]){

	initPcbs();
}