# OS-project

<h1 align="center">YAEOS</h1>

<h2>INTRODUCTION</h2>
<p>Sistema Operativo accademico basato su processore <b>ARM7TDMI</b> emulato.<p>
<p>Occorre installare <a href="https://github.com/mellotanica/uARM/releases"><b>µARM</b></a> per testare il progetto.</p>
<h3>Github:</h3>
<p>https://github.com/Plutone11011/OS-project<p>

<hr>

<h2>AUTHORS</h2>
<table>
  <!--primo-->
  <tr>
    <td>Lorenzo Borelli</td>
    <td>0000789622</td>
  </tr>
  <!--secondo-->
  <tr>
    <td>Federico Biagi</td>
    <td>0000758635</td>
  </tr>
  <!--terzo-->
  <tr>
    <td>Yihong Chen</td>
    <td>0000801605</td>
  </tr>
  <!--end table-->
</table>

<hr>

<h2>COMPILAZIONE/LINKING</h2>
<p>Dopo aver installato µARM ed i relativi pacchetti (Vedi <a href="https://github.com/mellotanica/uARM#readme">Readme</a>). <br>
  Compilare usando <em><b>Makefile</b></em>, digitando <em><b>make</b></em>.<br>
  L'eseguibile si chiamerà <b>main</b>.</p>

<hr>

<h2>SCELTE PROGETTUALI<h2>
<h3>Abbiamo suddiviso il progetto in 2 cartelle principali</h3>
<h4>"src" per tutti i file sorgente:</h4>
<pre>
-Alloc.c 
-Queue.c
-Tree.c
-Semaphore.c
</pre>
<h4>"header" per tutti i file header:</h4>
<pre>
-Alloc.h 
-Queue.h
-Tree.h
-Semaphore.h
-libuarm.h
-uARMtypes.h
-uARMconst.h
-PCB.h
</pre>

<h4>Alloc</h4>
<p>Gestisce i pcb (Process control block), consentendo di allocarli e deallocarli.</p>

<h4>Queue</h4>
<p>Gestisce code di pcb allocati con priorità decrescente.</p>

<h4>Tree</h4>
<p>Gestisce alberi di pcb allocati.</p>

<h4>Semaphore</h4>
<p>Gestisce i semafori tramite una Hash table.<br>
Ogni semaforo consente di bloccare e/o liberare pcb (thread) concorrenti tramite mutua esclusione con priorità (Usando Queue.c).<br>
Consente inoltre di liberare l'esecuzione di un pcb e tutti i suoi figli (thread), 'bypassando' la coda (Terminate process tree).</p>
<p align="right"><b><em>To be continued!!!</em></b></p>
