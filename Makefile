progetto: AnalisiRiscaldamento.c Makefile
	gcc -ansi -Wall -O AnalisiRiscaldamento.c -o progetto
pulisci:
	rm -f progetto.o
pulisci_tutto:
	rm -f progetto progetto.o Risultati.txt
esegui:
	./progetto

