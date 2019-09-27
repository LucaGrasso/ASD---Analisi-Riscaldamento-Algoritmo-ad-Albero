/*********************************************/
/* Progetto di Algoritmi e Strutture Dati   */
/* Sessione Estiva 2018/2019                */
/* Gestione di un sistema di riscaldamento  */
/* Studente: Luca Grasso                    */
/* Matricola 294612                         */
/********************************************/

/* Inclusione delle librerie */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Definizione delle costanti */
#define MAX_LENGTH 20     /* massima lunghezza della stringa che rappresenta il tempo */
#define INITIAL_SIZE 20   /* dimensione iniziale dell'array che contiene le letture */
#define MULTIPLY_FACTOR 2 /* fattore che definisce di quanto si incrementa l'array delle
                             letture quando si raggiunge il numero massimo di elementi */

/* Definizione del tipo di dato che contiene il nodo dell'albero che rappresenta una singola lettura */
typedef struct NodoLettura{
	char tempo[MAX_LENGTH];
	float gas;
	float elettricita;
	struct NodoLettura *destro;
	struct NodoLettura *sinistro;
} NodoLettura_t;

/* Definizione del tipo di dato struttura che rappresenta una lettura da inserire nell'array */
typedef struct Lettura {
	char tempo[MAX_LENGTH];
	float gas;
	float elettricita;
} Lettura_t;

/*************************************************************/
/* Dichiarazione delle funzioni implementate in questo file  */
/*************************************************************/
/* Sezione albero */
int inserisci_nodo_albero(NodoLettura_t **radice, char *tempo, float gas, float elettricita);
int ricercaAlbero(NodoLettura_t **radice);
int cancellaDaAlbero(NodoLettura_t **radice);
void cerca_simm(NodoLettura_t *nodo, char *inizio, char *fine);
void stampaAlbero(NodoLettura_t *nodo);

/*sezione array */
int inserisci_lettura_array(Lettura_t *arrayLetture, int size, int *dimensione_array, char *tempo, float gas, float elettricita);
int ricercaArray(Lettura_t *arrayLetture, int contaLetture);
int cancellaDaArray(Lettura_t *arrayLetture, int contaLetture);
int ordina_array(Lettura_t *arrayLetture, int inizio, int fine);
void fondi(Lettura_t a[], int sx, int mx, int dx);
void cancella_nodi(NodoLettura_t **radice, NodoLettura_t *nodo, NodoLettura_t *padre, char *inizio, char *fine);
int controllaData(char data[]);

/* sezione Test sperimentale */
int test(NodoLettura_t **radice);
int generaTempo(char *tempo);

/***********************************/
/* Definizione della funzione main */
/***********************************/
int main(void){
	/* Dichiarazione delle variabili della funzione */

	/* creazione della radice dell'albero */
	NodoLettura_t **radice = (NodoLettura_t **) malloc(sizeof(NodoLettura_t *));

	*radice = NULL;     /* lavoro: creazione della radice dell'albero */

	/* creazione di un array di letture che può contenere INITIAL_SIZE elementi*/
	Lettura_t *arrayLetture = malloc(INITIAL_SIZE*sizeof(Lettura_t));

	int dimensione_array = INITIAL_SIZE;  /* Lavoro: impostazione dimensione dell'array a valore inizale */

	/* Menu Utente di scelta dell'operazione da effettuare */
	printf("\n\nSelezionare una opzione (T/A/Q):\n");
    printf("***********************\n");
	printf("T. Alberi\n");
	printf("A. Array\n");
	printf("X. Analisi Sperimentale\n");
	printf("Q. Esci dal Programma\n");
    printf("***********************\n");

	/* codice numerico che indica quale struttura dati ha scelto l'utente
    (assumerà il valore 1 se l'utente ha scelto albero, 2 se ha scelto array) */
	int strutturaDati = 0;      /* Lavoro: Valore della scelta Utente per accedere ai vari sottoprogrammi main */

	/* si chiede all'utente quale struttura dati vuole utilizzare,
     la richiesta viene ripetuta finché l'utente inserisca un codice
     valido (T o A o X), oppure finché l'utente non decide di terminare
     il programma selezionando Q */
	while(strutturaDati == 0){
		char stringaLetta[MAX_LENGTH];  /* Input: Stringa della scelta utente */

		/* letto la stringa di input */
		scanf("%s", stringaLetta);

		/* Verifico se l'input cade in una opzione valita con controllo di sensitive case */
		if(stringaLetta[0] == 'T' || stringaLetta[0] == 't')
			strutturaDati = 1;      /* Accedo ai sottoprogrammi inerenti a una struttura Albero */
		else if(stringaLetta[0] == 'A' || stringaLetta[0] == 'a')
			strutturaDati = 2;		/* Accedo ai sottoprogrammi inerenti a una struttura Array */
		else if(stringaLetta[0] == 'X' || stringaLetta[0] == 'x'){
			test(radice);			/* Accedo al sottoprogrammi di Test Sperimentale */
			return 0;
		}
		else if(stringaLetta[0] == 'Q' || stringaLetta[0] == 'q')
			return 0;				/* Concludo il programma come da scelta Utente */
		else {
			/* WARNING Indico all'utente che ha scelto un'opzione non valida */
			printf("Non hai selezionato una opzione valida, riprova\n\n");
		}
	}

	/* si chiede all'utente di inserire il nome del file */
	printf("\nInserisci il nome del file di tipo nome.estensione (es. Lettura.txt):\n");

	char *nomeFile = malloc(MAX_LENGTH*sizeof(char));   /* Input: Puntatore alla varibile di lettura nome file */
	/* Leggo l'input del nome del file */
	scanf("%s", nomeFile);

	FILE *file;     /* Lavoro: variabile di tipo file */
    
	/* apro il file */
	file = fopen(nomeFile, "r");

	/* se il file indicato dall'utente non è valido, il programma stampa un messaggio di errore e termina */
	if(file == NULL){
		printf("Non hai inserito un nome valido\n FINE DEL PROGRAMMA\n\n");
		return 1;
	}

	/* variabili utilizzate per contenere le stringhe lette dal file */
	char *nChar;						/* Lavoro: puntatore a char */
	char buffer[100];				    /* Lavore: buffer della riga */

	int contaLetture = 0; 	/* Lavoro variabile che rappresenta il numero di letture inserite nell'array */


	/* si legge il file una riga alla volta, si individuano all'interno della stringa tempo, gas ed elettricità
	se nella struttura scelta non è già presente una lettura con il valore di tempo letto, la si inserisce
	se invece la struttura dati scelta (albero o array) contiene già una lettura con lo stesso tempo,
	la nuova stringa viene ignorata */
	do{
		nChar = fgets(buffer, 100, file);   /* leggo la riga */
		if(nChar != NULL && nChar[0]>= '0' && nChar[0] <= '9'){   /* verifico che abbia solo numeri o se il riga sia vuota */

			char tempo[20],		        /* Input: variabile stringa contenente la data e ora */
					 s_gas[20],         /* Input: variabile stringa contenente la lettura del gas*/
					 s_elettricita[20]; /* Input: variabile stringa contenente la lettura dell'elettricità */

			/* si copiano i primi 10 caratteri nella variabile tempo */
			strncpy(tempo, buffer, 10); /* copio i primi 10 caratteri */
			tempo[10] = 0;

			/* si ricerca l'indice che corrisponde all'inizio del valore del gas tenendo conto dello spazio */
			int i = 10;
			while(buffer[i]==' ')
				i++;

			/* si legge il valore del gas e lo si memorizza nella stringa s_gas tenendo conto dello spazio*/
			int j=0;
			while(buffer[i]!=' '){
				s_gas[j++] = buffer[i++];
			}
			s_gas[j] = 0;

			/* si ricerca l'indice che corrisponde all'inizio del valore dell'elettricita tenendo conto dello spazio */
			j = 0;
			while(buffer[i]==' ')
				i++;

			/* si legge il valore dell'elettricita e lo si memorizza nella stringa s_elettricita */
			while(buffer[i]!=' ' && buffer[i]!=0){
				s_elettricita[j++] = buffer[i++];
			}
			s_elettricita[j] = 0;

			/* conversione di s_gas e s_elettricita in float */
			float gas = atof(s_gas);                    /* variabile contenente il valore di gas in float */
			float elettricita = atof(s_elettricita);    /* variabile contenente il valore di elettricità in float */

			/* inserimento della nuova lettura nell'albero oppure nell'array */
			if(strutturaDati==1){
				inserisci_nodo_albero(radice, tempo, gas, elettricita); /* sottoprogramma di inserimento dati per l'albero */
			} else {
                /* sottoprogramma di inserimento dati per l'array */
				inserisci_lettura_array(arrayLetture, contaLetture++, &dimensione_array, tempo, gas, elettricita);
			}
		}
	} while(nChar != NULL); /* ripeto finchè non trovo una riga nulla */

	/* chiusura del file */
	fclose(file);

	/* Nel caso in cui è stato scelto la struttura dati array lo riordino */
	if(strutturaDati == 2){
		ordina_array(arrayLetture, 0, contaLetture-1); /* ordino l'array con l'algoritmo mergesort */

		/* controllo duplicati */
		int i;  /* Lavoro: contatore del ciclo for */
		for(i = 0; i < contaLetture-1; i++){
			if(strcmp(arrayLetture[i].tempo, arrayLetture[i+1].tempo) == 0){
				int j;
                for(j = i + 2; j < contaLetture; j++)
                    arrayLetture[j-1] = arrayLetture[j];
                contaLetture--;
                i--;
            }
		}

		/* stampo i valori dell'array ordinati*/
		for(i = 0; i < contaLetture; i++){
			printf("%s ", arrayLetture[i].tempo);
			printf("%f ", arrayLetture[i].gas);
			printf("%f\n", arrayLetture[i].elettricita);
		}
	} else {
		stampaAlbero(*radice);		
	}

	/* si chiede all'utente quale operazione vuole fare
	si ripete la richiesta finché l'utente non seleziona Q (uscita dal programma) */
	while(1){

		printf("\nSelezionare una opzione (R/D/Q):\n");
        printf("**********************************\n");
		printf("R. Ricerca\n");
		printf("D. Cancellazione\n");
		printf("Q. Esci dal Programma\n\n");

		char stringaLetta[MAX_LENGTH];
		scanf("%s", stringaLetta);

		switch(stringaLetta[0]){
			case 'r':
			case 'R': 		/* l'utente vuole fare una ricerca nell'albero o nell'array */
				if(strutturaDati == 1)
					ricercaAlbero(radice);
				else {
					ricercaArray(arrayLetture, contaLetture);
				}
				break;
			case 'd':
			case 'D':		 /* l'utente vuole fare una cancellazione nell'albero o nell'array */
				if(strutturaDati == 1)
					cancellaDaAlbero(radice);
				else {
					contaLetture = cancellaDaArray(arrayLetture, contaLetture);
				}
				break;
			case 'q':
			case 'Q': 		/* l'utente vuole uscire dal programma */
				return 0;
			default:		/* l'utente non ha selezione un'opzione valida e viene comunicato a video */
				printf("Hai selezionato una opzione non valida, riprova\n");
		}
	}

	return 0;
}

/********************************************************************/
/* Sottoprogramma per inserisce un nuovo NodoLettura_t nell'albero, */
/* si è usato l'algoritmo di inserimento di un nodo in un albero    */
/* ordinato visto a lezione                                         */
/********************************************************************/
int inserisci_nodo_albero(NodoLettura_t **radice, char *tempo, float gas, float elettricita){
	int inserito;
	NodoLettura_t *nodo, *padre, *nuovo;

	/* per confrontare due letture, confrontiamo i rispettivi valori della variabile tempo con la funzione strcmp
	strcmp prende come argomento due stringhe,
	- restituisce 0 se le due stringhe sono uguali
	- restituisce un numero positivo se la prima stringa segue la seconda in ordine lessicografico
	- restituisce un numero negativo se la prima stringa precede la seconda in ordine lessicografico */
	for (nodo = padre = *radice;
			((nodo != NULL) && (strcmp(tempo, nodo->tempo) != 0));
			padre = nodo, nodo = strcmp(tempo, nodo->tempo) < 0 ? nodo->sinistro: nodo->destro);
	if (nodo != NULL)
		inserito = 0;
	else {
		inserito = 1;
		nuovo = (NodoLettura_t *)malloc(sizeof(NodoLettura_t));
		strncpy(nuovo->tempo, tempo, 15);
		nuovo->gas = gas;
		nuovo->elettricita = elettricita;
		nuovo->sinistro = nuovo->destro = NULL;
		if (nodo == *radice)
			*radice = nuovo;
		else
			if (strcmp(tempo, padre->tempo) < 0)
				padre->sinistro = nuovo;
			else
				padre->destro = nuovo;
		}
	return(inserito);
}

/******************************************************************/
/* Sottoprogramma che chiede all'utente di inserire un intervallo */
/* di tempo e stampa tutte le letture nell'intervallo             */
/******************************************************************/
int ricercaAlbero(NodoLettura_t **radice){

	char inizio[MAX_LENGTH];	/* Lavoro: variabile di inizio lettura dati */
	char fine[MAX_LENGTH];		/* Lavoro: variabile di fine lettura dati */
	char temp[MAX_LENGTH];		/* Input: variabile di appoggio dati in Input*/

	int isValid = 0;			/* Lavoro: verifico se il dato è valido */

	/* l'utente deve inserire il tempo nel formato aaaammddhh, esempio 2019042908 */
	printf("Inserisci giorno, mese, anno, orario di inizio rilevazioni da ricercare nel formato aaaammddhh, per esempio 2019042908\n");

	/* verifico se l'input è esclusivamente numerico a prescindere della validità posizionale */
	do{
		scanf("%s", temp);
		isValid = controllaData(temp);
		if(!isValid)
			printf("Non hai inserito una sequenza di 10 cifre\n");
	}while(!isValid);
	strncpy(inizio, temp, 11);

	/* l'utente deve inserire il tempo nel formato aaaammddhh, esempio 2019042908 */
	printf("Inserisci giorno, mese, anno, orario di fine rilevazioni da ricercare nel formato aaaammddhh, per esempio 2019042910\n");

	/* verifico se l'input è esclusivamente numerico a prescindere della validità posizionale */
	do{
		scanf("%s", temp);
		isValid = controllaData(temp);
		if(!isValid)
			printf("Non hai inserito una sequenza di 10 cifre\n");
	}while(!isValid);
	strncpy(fine, temp, 11);

	/* chiamata al sottoprogramma ricorsivo cerca_simm */
	cerca_simm(*radice, inizio, fine);

	return 0;
}

/*********************************************************/
/* Sottoprogramma per l'inserimento dell'input per la    */
/* cancellazione di un valore/i nell'albero              */
/*********************************************************/
int cancellaDaAlbero(NodoLettura_t **radice){

	char inizio[MAX_LENGTH];		/* Lavoro: varibile con la data e ora di inizio */
	char fine[MAX_LENGTH];			/* Lavoro: variabile con la data e ora di fine */
	char temp[MAX_LENGTH];			/* Input: variabile che acquisisce l'input da utente */

	int isValid = 0;				/* Lavoro: verifico se il dato è valido */

	/* l'utente deve inserire il tempo nel formato aaaammddhh, esempio 2019042908 */
	printf("Inserisci giorno, mese, anno, orario di inizio rilevazioni da cancellare nel formato aaaammgghh\n");

	/* verifico se l'input è esclusivamente numerico a prescindere della validità posizionale */
	do{
		scanf("%s", temp);
		isValid = controllaData(temp);
		if(!isValid)
			printf("Non hai inserito una sequenza di 10 cifre\n");
	}while(!isValid);
	strncpy(inizio, temp, 11);

	/* l'utente deve inserire il tempo nel formato aaaammddhh, esempio 2019042908 */
	printf("Inserisci giorno, mese, anno, orario di fine rilevazioni da cancellare\n");

	/* verifico se l'input è esclusivamente numerico a prescindere della validità posizionale */
	do{
		scanf("%s", temp);
		isValid = controllaData(temp);
		if(!isValid)
			printf("Non hai inserito una sequenza di 10 cifre\n");
	}while(!isValid);
	strncpy(fine, temp, 11);

	/* Sottoprogramma per la cancellazione di dati nell'albero */
	cancella_nodi(radice, *radice, *radice, inizio, fine);

	return 0;
}

/********************************************************/
/* Sottoprogramma che inserisce una lettura nell'array  */
/* mediante un intervallo di tempo aaaammddhh           */
/********************************************************/
int inserisci_lettura_array(Lettura_t *arrayLetture, int contaLetture, int *dimensione_array, char *tempo, float gas, float elettricita){

	if(contaLetture >= *dimensione_array){

		/* ridimensione l'array se contaletture è maggiore della dimensione_array */
		arrayLetture = realloc(arrayLetture, MULTIPLY_FACTOR*(*dimensione_array)*sizeof(Lettura_t));

		/* aggiorno il valore della dimensione */
		*dimensione_array = MULTIPLY_FACTOR*(*dimensione_array);
	}

	/* creazione di un Lettura_t */
	Lettura_t nuovo;    /* Lavoro: creo un nodo di tipo lettura_t */

	strncpy(nuovo.tempo, tempo, MAX_LENGTH); /* assegno la data e ora */
	nuovo.gas = gas;					     /* assegno il valore digas */
	nuovo.elettricita = elettricita;         /* assegno il valore dell'elettricità */

	/* la nuova lettura viene copiata alla fine dell'array */
	arrayLetture[contaLetture] = nuovo;
    
	return 0;
}

/********************************************************/
/* Funzione per la ricerca in un array con inserimento  */
/* di input da utente                                   */
/********************************************************/
int ricercaArray(Lettura_t *arrayLetture, int contaLetture){

	char inizio[MAX_LENGTH];		/* Lavoro: varibile con la data e ora di inizio */
	char fine[MAX_LENGTH];			/* Lavoro: variabile con la data e ora di fine */
	char temp[MAX_LENGTH];			/* Input: variabile che acquisisce l'input da utente */

	int isValid = 0;				/* Lavoro: verifico se il dato è valido */

	/* si chiede all'utente di inserire giorno e ora di inizio letture da ricercare nel formato aaaammddhh */
	printf("Inserisci giorno, mese, anno, orario di inizio rilevazioni da ricercare\n");

	/* controllo della validità dell'input se è interamente numerico */
	do{
		scanf("%s", temp);
		isValid = controllaData(temp);
		if(!isValid)
			printf("Non hai inserito una sequenza di 10 cifre\n");
	}while(!isValid);
	strncpy(inizio, temp, 11);

	/* si chiede all'utente di inserire giorno e ora di fine letture da ricercare nel formato aaaammddhh */
	printf("Inserisci giorno, mese, anno, orario di fine rilevazioni da ricercare\n");

	/* controllo della validità dell'input se è interamente numerico */
	do{
		scanf("%s", temp);
		isValid = controllaData(temp);
		if(!isValid)
			printf("Non hai inserito una sequenza di 10 cifre\n");
	}while(!isValid);
	strncpy(fine, temp, 11);

	int contatore = 0;					/* Lavoro: indice per l'array dei dati inseriti */

	/* cerco la prima lettura successiva alla data di inizio */
	while(strcmp(inizio, (arrayLetture[contatore]).tempo) > 0 && contatore < contaLetture)
		contatore++;

	/* si stampa la lettura corrente finché la lettura corrente è precedente alla data di fine */
	while(strcmp(fine, (arrayLetture[contatore]).tempo) > 0 && contatore < contaLetture) {
		printf("%s ", arrayLetture[contatore].tempo);
		printf("%f ", arrayLetture[contatore].gas);
		printf("%f\n", arrayLetture[contatore].elettricita);
		contatore++;
	}
	return 0;
}

/********************************************************/
/* Sottoprogramma per la cancellazione da un array      */
/* con inserimento degli input                          */
/********************************************************/
int cancellaDaArray(Lettura_t *arrayLetture, int contaLetture){

	char inizio[MAX_LENGTH];		/* Lavoro: varibile con la data e ora di inizio */
	char fine[MAX_LENGTH];			/* Lavoro: variabile con la data e ora di fine */
	char temp[MAX_LENGTH];			/* Input: variabile che acquisisce l'input da utente */

	int isValid = 0;				/* Lavoro: verifico se il dato è valido */

	/* si chiede all'utente di inserire giorno e ora di inizio rilevazioni da cancellare nel formato aaaammddhh */
	printf("Inserisci giorno, mese, anno, orario di inizio rilevazioni da cancellare\n");

	/* controllo della validità dell'input se è interamente numerico */
	do{
		scanf("%s", temp);
		isValid = controllaData(temp);
		if(!isValid)
			printf("Non hai inserito una sequenza di 10 cifre\n");
	}while(!isValid);
	strncpy(inizio, temp, 11);

	/* si chiede all'utente di inserire giorno e ora di fine rilevazioni da cancellare nel formato aaaammddhh */
	printf("Inserisci giorno, mese, anno, orario di fine rilevazioni da cancellare\n");

	/* controllo della validità dell'input se è interamente numerico */
	do{
		scanf("%s", temp);
		isValid = controllaData(temp);
		if(!isValid)
			printf("Non hai inserito una sequenza di 10 cifre\n");
	}while(!isValid);
	strncpy(fine, temp, 11);

	/* contatore per le letture cancellate */
	int lettureCancellate = 0;  /* Lavoro: contatore per indice di array */

	int contatore = 0;          /* Lavoro: contatore per indice di arra */

	/* si cerca l'indice della prima lettura successiva al valore del tempo di inizio scelto dall'utente */
	while(strcmp(inizio, (arrayLetture[contatore]).tempo) > 0 && contatore < contaLetture)
		contatore++;

	/* incremento lettureCancellate finché la lettura corrente ha il valore del tempo che precede il tempo di fine scelto dall'utente */
	while(strcmp(fine, (arrayLetture[contatore]).tempo) > 0 && contatore < contaLetture) {
		lettureCancellate++;
		contatore++;
	}

	/* a questo punto contatore è l'indice del primo elemento che non va cancellato
	copio tutte letture successive al contatore corrente nelle posizioni precedenti
	in modo da cancellare esattamente lettureCancellate elementi prima del contatore */
	int i;      /* Lavoro: indice per lo scorrimento dell'array */
	for(i=contatore; i<contaLetture; i++){
		arrayLetture[i - lettureCancellate] = arrayLetture[i];
	}

	/* restituisce il numero di letture presenti nell'array dopo la cancellazione */
	return contaLetture - lettureCancellate;
}

/**********************************************/
/* Sottoprogramma per la ricerca in un albero */
/**********************************************/
void cerca_simm(NodoLettura_t *nodo, char *inizio, char *fine){
	/* se il nodo è nullo cerco a destra */
	if (nodo != NULL) {
		/* chiamata ricorsiva per sinistra */
		cerca_simm(nodo->sinistro, inizio, fine);
		/* se trovo il nodo lo stampo */
		if( strcmp(nodo->tempo, inizio) >= 0 && strcmp(nodo->tempo, fine) < 0){
				printf("%s ", nodo->tempo);
				printf("%f ", nodo->gas);
				printf("%f\n", nodo->elettricita);
		}
		/* ricerca nel nodo destro */
		cerca_simm(nodo->destro, inizio, fine);
	}
}

/*************************************************************/
/* Sottoprogramma per l'ordinamento dell'array con Mergesort */
/* split dell'array                                          */
/*************************************************************/
int ordina_array(Lettura_t *arrayLetture, int inizio, int fine){

	int mx; /* Lavoro: variabile di appoggio per mergesort */

    if (inizio < fine) {
        mx = (inizio + fine) / 2;    /* calcolo indice per la metà */

				/* ordino da inizio a mx */
				ordina_array(arrayLetture,inizio,mx);
        
				/* ordino da mx + 1 a fine */
				ordina_array(arrayLetture,mx + 1,fine);

				/* unisco i due array */
				fondi(arrayLetture,inizio,mx,fine);
	}

	return 0;
}

/*************************************************************/
/* Sottoprogramma per l'ordinamento dell'array con Mergesort */
/* unione dei due array													  						*/
/*************************************************************/
void fondi(Lettura_t a[], int sx, int mx, int dx){

	Lettura_t *b; /* Lavoro: array di appoggio */

	int i, 	/* Lavoro: indice per la parte sinistra di a (da sx ad m) */
	j, 		/* Lavoro: indice per la parte destra di a (da m + 1 a dx) */
	k;	    /* Lavoro: indice per la porzione di b da sx a dx */

	/* fondi ordinatamente le due parti finche’ sono entrambe non vuote */
	b = (Lettura_t *)calloc(dx + 1,sizeof(Lettura_t));
    for (i = sx, j = mx + 1, k = 0;((i <= mx) && (j <= dx));k++) {
		if (strcmp(a[i].tempo, a[j].tempo) <= 0){
			b[k] = a[i];
			i++;
		} else {
			b[k] = a[j];
			j++;
		}
	}
	while (i<=mx) {
		b[k] = a[i];
		i++;
		k++;
	}
	while (j<=dx) {
		b[k] = a[j];
		j++;
		k++;
	}
	for (k=sx; k<=dx; k++)
		a[k] = b[k-sx];

	/* Libero la memoria */
	free(b);
}

/*************************************************************/
/* Sottoprogramma per la cancellazione dei nodi in un albero */
/*************************************************************/
void cancella_nodi(NodoLettura_t **radice, NodoLettura_t *nodo, NodoLettura_t *padre, char *inizio, char *fine){

	if(nodo != NULL) {
		cancella_nodi(radice, nodo->sinistro, nodo, inizio, fine);
		cancella_nodi(radice, nodo->destro, nodo, inizio, fine);
	}
	if(nodo != NULL && strcmp(nodo->tempo, inizio) >= 0 && strcmp(nodo->tempo, fine) < 0) {
		if (nodo->sinistro == NULL){
			if (nodo == *radice)
				*radice = nodo->destro;
			else
		if (strcmp(nodo->tempo, padre->tempo) < 0)
			padre->sinistro = nodo->destro;
		else
			padre->destro = nodo->destro;
		} else
			if (nodo->destro == NULL){
				if (nodo == *radice)
					*radice = nodo->sinistro;
				else
					if (strcmp(nodo->tempo, padre->tempo) < 0)
						padre->sinistro = nodo->sinistro;
					else
						padre->destro = nodo->sinistro;
			}
		else {
			NodoLettura_t *sost = nodo;
			for (padre = sost, nodo = sost->sinistro;(nodo->destro != NULL);padre = nodo, nodo = nodo->destro);
			strncpy(sost->tempo, nodo->tempo, MAX_LENGTH);
			sost->gas = nodo->gas;
			sost->elettricita = nodo->elettricita;
			if (padre == sost)
				padre->sinistro = nodo->sinistro;
			else
				padre->destro = nodo->sinistro;
		}
	}
}

/***************************************************************/
/* Sottoprogramma per il controllo della data che sia numerico */
/* e della lunghezza giusta                                    */
/***************************************************************/
int controllaData(char data[]) {

	if(strlen(data) != 10)
		return 0;
	int i;   					/* Lavoro: variabile per scorrere il ciclo for */
	for(i=0; i<10; i++){
		if(data[i] < '0' || data[i] > '9')
			return 0;
	}

	return 1;
}

/***************************************************************/
/* Sottoprogramma per il test sperimentale					   */
/*                                                             */
/***************************************************************/
int test(NodoLettura_t **radice) {

	clock_t start_time, end_time; /* variabili utilizzate per calcolare il tempo di esecuzione di ciascuna funzione */
	FILE *fd_test;/* i risultati dell'analisi vengono riportati in un file di nome Risultati.txt */
	fd_test=fopen("Risultati.txt", "w");
	int N[] = {100, 1000, 10000 }; /* l'analisi viene eseguita 3 volte: con 100, 1000, 10000 elementi nell'array e nell'albero*/
	int i;
	for(i=0; i<3; i++){
		int contaLetture = 0; /* numero di elementi nell'array*/
		Lettura_t *arrayLetture = malloc(N[i]*sizeof(Lettura_t)); /* creazione array di 100, 1000, 10000 */
		int j;
		for(j=0; j<N[i]; j++){
			char *tempo = malloc(MAX_LENGTH*sizeof(char));
			generaTempo(tempo); /* creazione di un valore casuale di tempo
			Notiamo che i valori di gas ed elettricità non influiscono sul tempo di ricerca e cancellazione, quindi
			prendiamo valori nulli per gas ed elettricità*/
			inserisci_lettura_array(arrayLetture, contaLetture++, &N[i], tempo, 0, 0); /* inserimento di una lettura nell'array */
			inserisci_nodo_albero(radice, tempo, 0, 0);		/* inserimento di una lettura nell'albero 	*/
		}
		ordina_array(arrayLetture, 0, contaLetture-1); /* ordinamento dell'array */

		char inizio[MAX_LENGTH] = "2018010109";  /* definizione dei tempi di inizio e fine della ricerca e cancellazione da effettuare*/
		char fine[MAX_LENGTH] = "2018030109";

		/* CERCA ALBERO */
		start_time = clock();   /* si registra l'istante di tempo in cui inizia l'operazione */
		cerca_simm(*radice, inizio, fine);
		end_time = clock();  /* si registra l'istante di tempo in cui è terminata l'operazione */
		/* il numero di secondi impiegati è la differenza end_time - start_time divisa per la costante CLOCKS_PER_SEC (numero di clocks per secondo
		eseguiti dal computer, è necessario fare il cast du (end_time - start_time) a float, altrimenti viene eseguita la divisione fra interi */
		fprintf(fd_test, "Ricerca albero di %d elementi: %f secondi\n", N[i], (float)(end_time - start_time) / CLOCKS_PER_SEC);

		/* CERCA ARRAY */
		start_time = clock(); /* si registra l'istante di tempo in cui inizia l'operazione */
		int contatore = 0;
		/* cerco la prima lettura successiva alla data di inizio */
		while(strcmp(inizio, (arrayLetture[contatore]).tempo) > 0 && contatore < contaLetture)
			contatore++;

		/* si stampa la lettura corrente finché la lettura corrente è precedente alla data di fine */
		while(strcmp(fine, (arrayLetture[contatore]).tempo) > 0 && contatore < contaLetture) {
			printf("%s ", arrayLetture[contatore].tempo);
			printf("%f ", arrayLetture[contatore].gas);
			printf("%f\n", arrayLetture[contatore].elettricita);
			contatore++;
		}
		end_time = clock(); /* si registra l'istante di tempo in cui è terminata l'operazione */
		fprintf(fd_test, "Ricerca array di %d elementi: %f secondi\n", N[i], (float)(end_time - start_time) / CLOCKS_PER_SEC);

		/* CANCELLA ALBERO */
		start_time = clock(); /* si registra l'istante di tempo in cui inizia l'operazione */
		cancella_nodi(radice, *radice, *radice, inizio, fine);
		end_time = clock(); /* si registra l'istante di tempo in cui è terminata l'operazione */
		fprintf(fd_test, "Cancellazione albero di %d elementi: %f secondi\n", N[i], (float)(end_time - start_time) / CLOCKS_PER_SEC);

		/* CANCELLA ARRAY */
		start_time = clock(); /* si registra l'istante di tempo in cui inizia l'operazione */
		contatore = 0;
		int lettureCancellate = 0;

		/* si cerca l'indice della prima lettura successiva al valore del tempo di inizio scelto dall'utente */
		while(strcmp(inizio, (arrayLetture[contatore]).tempo) > 0 && contatore < contaLetture)
			contatore++;

		/* incremento lettureCancellate finché la lettura corrente ha il valore del tempo che precede il tempo di fine scelto dall'utente */
		while(strcmp(fine, (arrayLetture[contatore]).tempo) > 0 && contatore < contaLetture) {
			lettureCancellate++;
			contatore++;
		}
		/* a questo punto contatore è l'indice del primo elemento che non va cancellato
		copio tutte letture successive al contatore corrente nelle posizioni precedenti
		in modo da cancellare esattamente lettureCancellate elementi prima del contatore */
		int k;
		for(k=contatore; k<contaLetture; k++){
			arrayLetture[k - lettureCancellate] = arrayLetture[k];
		}
		end_time = clock(); /* si registra l'istante di tempo in cui è terminata l'operazione */
		fprintf(fd_test, "Cancellazione array di %d elementi: %f secondi\n", N[i], (float)(end_time - start_time) / CLOCKS_PER_SEC);


	}
	fclose(fd_test);
    
    printf("\nCreato il file Risultato.txt per la ricerca sperimentale\n\n");
    printf("Il programma è terminato\n\n");
    
	return 0;

}

/********************************************/
/* Genera il tempo per il test sperimentale */
/********************************************/
int generaTempo(char *tempo){

	/* rand() restituisce un numero intero casuale compreso fra 0 e il massimo intero rappresentabile */
	/* rand()%K  genera un numero casuale fra 0 e K-1 */
	int anno = rand()%3 + 2017; /* genera un anno tra 2017, 2018, 2019 */
	int mese = rand()%12 + 1; /* genera un numero fra 1 e 12 (mese) */
	int giorno = 0;
	switch(mese){
		case 11:
		case 4:
		case 6:
		case 9:
		giorno = rand()%30 + 1;
		break;
		case 2:
		giorno = rand()%28 + 1;
		break;
		default:
		giorno = rand()%31 + 1;
	}
	int ora = rand()%10 + 9; /* le rilevazioni avvengono fra le 9 e le 18 */
	sprintf(tempo, "%4d%02d%02d%02d", anno, mese, giorno, ora);
	return 0;

}

/****************************************/
/* Sottoprogramma di stampa dell'albero */
/****************************************/
void stampaAlbero(NodoLettura_t *nodo) {
	
	if(nodo != NULL) {
		stampaAlbero(nodo->sinistro);
		printf("%s %f %f\n", nodo->tempo, nodo->gas, nodo->elettricita);
		stampaAlbero(nodo->destro);
	}
	
}
