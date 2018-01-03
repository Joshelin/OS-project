/* Il gestore delle code implementa quattro funzionalità  relative ai PCB:  
– Allocazione e deallocazione dei PCB. "/PCBAlloc/PCBAlloc.h"  
– Gestione delle code dei  PCB. "/PCBQueue/Queue.h"
– Gestione dell’abero dei  PCB. "/PCBTree/Tree.h"
– Gestone  di  una  Active  Semaphore Hash Table (ASHT), che  gestisce i processi bloccati sui semafori. "/Semaphore/Semaphore.h"  
ASSUNZIONE:  non  ci sono più  di  MAXPROC processi concorrenti  in YAEOS.*/

typedef struct pcb_t {
	struct pcb_t *p_next;
	struct pcb_t *p_parent;
	struct pcb_t *p_first_child;
	struct pcb_t *p_sib;
	state_t p_s;
	int priority;
	int *p_semKey;
} pcb_t;