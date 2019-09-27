# Analisi Riscaldamento - Algoritmo ad Albero
 Programma in C Algoritmo ad Albero

Progetto in C per linux - Progetto presentato per il corso di laurea L31 a Urbino
Sessione Estiva 2019

1. SPECIFICA DEL PROBLEMA 
Si supponga di dover progettare un programma per l’analisi un sistema di riscaldamento di un ediﬁcio. Il sistema rileva il consumo di gas e di energia elettrica (relativi ad ogni ora) e scrive i dati relativi su un ﬁle di log in formato testo, secondo il seguente formato (si assumano campi separati da tabulazione o spazio):
 * Tempo: un codice numerico che risulta dalla concatenazione di anno, mese, giorno e ora della rilevazione.
 * Gas: un numero reale che rappresenta il consumo in m3 nell’ora di rilevamento.
 * Elettricita’: un numero reale che rappresenta il consumo in KWh nell’ora di rilevamento.
```
 Ad esempio:
Tempo        Gas   Elettricita’ 
2019042908   1.3   2.70
2019042909   0.3   4.26
2019042910   8.3   5.14
```

Si scriva un programma ANSI C che esegue le seguenti elaborazioni:
1. Acquisisce il ﬁle e memorizza le relative informazioni in una struttura dati di tipo albero.
2. Ricerca e restituisce il dato relativo ai consumi di una determinata rilevazione. Ad esempio: sel’utente chiede quali consumi sono stati effettuati il giorno 29/04/2019 dalle ore 09 alle ore 10, il programma deve restituire le informazioni contenute nella riga 3 (2019042910, 8.3, 5.14).
3. Permette la cancellazione del dato corrispondente ad una determinata rilevazione (tempo, gas, elettricit`a) sulla base della richiesta dell’utente (sempre tramite indicazione del tempo).

Il programma deve inoltre prevedere una modalit`a che implementa le stesse funzionalit`a utilizzando un array al posto dell’albero. Per quanto riguarda l’analisi teorica si deve fornire la complessit`a corrispondente ad ognuna delle seguenti operazioni: ricerca; cancellazione. Oltre all’analisi teorica della complessit`a si deve effettuare uno studio sperimentale della stessa per le operazioni di ricerca e cancellazione. Come suggerimento si può operaregenerandounnumero N dirilevazionicasuali. L’analisisperimentaledevequindivalutarelacomplessit`a al variare di N e confrontare l’algoritmo di ricerca e quello di cancellazione che lavorano su albero con i corrispondenti che lavorano su array.

Spero il programma possa aiutare studenti per la compressione e gestione degli algoritmi ad albero.
Sono disponibile per domande o chiarimenti, contattatemi pure in email o su linkedin.

Luca