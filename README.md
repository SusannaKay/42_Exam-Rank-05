# Exam-Rank-05

## Parte 1

### BigInt

#### Cosa chiede il subject

Il subject ci chiede di creare una classe che possa salvare al suo interno un intero unsigned di lunghezza indefinita. Gestendo quindi senza limiti ogni numero ( oltre quindi int_max ).
La classe in questione deve supportare l'addizione, la comparazione e lo shift di digit.

per digit shift si intende qualcosa che assomiglia allo shift di bit ma in base 10.
Ad esempio

```
42 << 3 = 42000
1337 >> 2 = 13
```

Il bigInt deve essere stampabile con `<<` (in base 10) e l'output non deve avere nessuno zero a sinistra.
Ci verrá fornito un main di testing.

L'unico modo per gestire un int senza limite é salvarlo in una stringa in cui ogni carattere ovviamente é una cifra.

#### Cose da ripassare:

- stringhe
- operator+
- confronti
- shift `<<` e `>>=`

#### Cose da tenere a mente

Una parte critica di questo esercizio sono ovviamente le operazioni: non possiamo fare `stoi()`, perché l'int potrebbe essere troppo grande, per questo motivo dobbiamo simulare la somma "a mano".

```
123 +
456

3 + 6 = 9
2 + 5 = 7
1 + 4 = 5

risultato: la stringa "579".
```

Da gestire quindi il riporto ( quando capita 9 + 1 ).
La lettura della stringa per effettuare la somma deve ovviamente partire da destra quindi da `s.size() - 1`.

Altra cosa da tenere in considerazione sono le lunghezze diverse delle stringhe: se abbiamo

```
999999 +
1
```

l'idea di base deve sempre essere quella di:

1. partire dall'ultima cifra
2. sommare
3. gestire il carry
4. costruire il risultato al contrario
5. invertire il risultato

Shift vari:

- `<<` dobbiamo aggiungere zeri a destra - unica eccezione lo zero che deve rimanere zero
- `>>=` rimuovi cifre da destra - se le cifre da rimuovere sono di piú di quelle contenute devo stampare zero

#### Costruttore

Prende un int e lo trasforma in stringa. In C++98 non abbiamo `to_string()` quindi dobbiamo usare `std::ostringstream`.

`std::ostringstream` - output string stream
libreria: `<sstream>`
é come un `std::cout` che non scrive su terminale ma dentro ad uno stream in memoria che poi convertiamo in stringa.

quindi idealmente

```cpp
bigInt(42) {
    std::ostringstream out;
    out << 42;
    std::string str = oss.str();
}
```

— ignorando il fatto che in realtá andremo a fare una assegnazione all'attributo privato della classe stessa ( non dichiarando quindi una stringa extra ), ma é giusto per capire.

#### Comparazioni

La prima cosa da guardare é la lunghezza del numero. A paritá di lunghezza il confronto parte dalle cifre di sinistra e quindi da inizio stringa: a quel punto posso confrontare direttamente le due stringhe.

#### Operator+

Inizio a scorrere le stringhe da destra, loopo finché non ho finito tutte e due le liste e addizionato l'ultimo resto. Controllo se str1 non é finita, e aggiungo alla somma, poi controllo se str2 non é finita e aggiungo alla somma. Il nuovo resto é `somma / 10`, mentre il risultato é `somma % 10` ritrasformato in char.
Faccio `std::reverse(result.begin(), result.end())`, salvo in un oggetto temporaneo e ritorno tmp.

#### Operator+=

```
this = this + other
```

#### Operator++

```
this += bigInt(1)
```

#### Preincremento — `bigint &bigint::operator++()`

Ritorna puntatore, come operator++.

#### Postincremento — `bigint bigint::operator++(int)`

Ritorna una reference alla copia col vecchio valore ma aggiorna l'originale a quello successivo.
Creo oggetto tmp con this, poi addiziono `bigInt(1)` a this e ritorno tmp.

#### Operator<<=

Modifica l'oggetto iniziale. Dobbiamo distinguere tra caso normale e caso 0. Se zero, ritorna `*this` direttamente.
Se caso normale aggiungi n volte il carattere "0" alla stringa.

#### Operator<<

Questo deve aggiungere n zeri alla stringa ( a destra ). L'operazione non modifica l'oggetto, ne deve creare uno nuovo facendo `tmp <<= amount`.

#### Operator>>=

Deve togliere n cifre a destra, se finiscono le cifre ritorna zero.

```cpp
bigint &operator>>=(const bigint &other);
```

Quindi other ti dice quante cifre togliere a this, va fatto un mini atoi e poi chiamata la versione `unsigned int` dello stesso metodo. Questo controlla che n non "esaurisca" tutta la stringa ed in caso ritorna zero.
Altrimenti chiama `resize()`. Resize diminuisce la stringa di n char.

#### Operator >>

Differisce da `>>=` semplicemente perché non va a modificare l'oggetto che la chiama, quindi va a fare le modifiche su un oggetto tmp che poi restituirá.

### vect2

#### Cosa chiede il subject

Il subject ci chiede di creare una classe `vect2` che contiene 2 int ed agisca come vector. Dobbiamo fare l'overload di stampa, addizione, sottrazione e moltiplicazione come se fosse uno scalare. Inoltre dobbiamo implementare anche l'overload di `[]` per accedere agli elementi senza l'obbligo di controllare se l'indice sia corretto. La classe deve rappresentare in sostanza un vettore matematico {x,y}.
Da main poi abbiamo i classici overload di pre e post incremento/decremento, `+=`, `-=`, `*=`, `==` e `!=`.

Sorvolo sul ripasso degli overload perché l'ho giá fatto in bigint.

### Struttura della classe

Abbiamo due opzioni: o creare due int come attributo, oppure un semplice array di int che semplifica ulteriormente l'operator[]. Opto per quest'ultima soluzione. Avendo la necessitá di cambiare valore agli elementi dell'attributo della classe anche dal main, ho messo l'array in public.

### Costruttori

`vect2()` inizializza l'array a 0, 0, gli altri sono banali da spiegare.

### operator[]

`[]` nel main viene usato per due operazioni. Entrambe accedono all'n-esimo elemento dell'array di int. Uno peró ha un `vect2 const` in ingresso e impone la non modifica di quanto restituisce, l'altro no. Quindi abbiamo bisogno di due firme in modo da gestire entrambi i casi.
Non dovendo gestire gli out of range, la funzione semplicemente restituisce l'ennesimo elemento e basta.

### operator == e !=

Controllano se entrambi i valori sono `==` o `!=` e restituiscono un bool, facile facile.

### operator += ( e -= e *= )

Come prima: devono modificare l'oggetto quindi ritorneranno una reference a this. Inoltre dobbiamo considerare in alcuni casi l'overload accetterá un int ( ad esempio `*=` ), in altri solo un oggetto. Il principio é sostanzialmente lo stesso peró.

Da adesso spiego solo alcuni operandi che hanno una sorta di eccezione.

### operator-

Ne abbiamo di due tipi. Uno unario e l'altro binario. Il binario segue la logica di tutti gli altri operator. Quello unario invece non prende parametri ed il suo scopo é quello di moltiplicare per -1 l'array di int.

### operator*

Ne abbiamo anche qui due. Uno dentro la classe e corrisponde alla classica moltiplicazione fra due oggetti. Quindi prende come parametro other. L'altro invece serve a gestire il `3 * v2` (o quello che é). Va inserito fuori dalla classe perché in questo caso l'operando di sinistra é un int, e non un oggetto, quindi non posso inserirlo come metodo della classe.

### polyset

Nell'esercizio ci vengono date alcune classi:

- `bag`: classe astratta
- `searchable_bag`: una implementazione di bag con il metodo che ci permette di cercare nella bag
- `tree_bag`: implementazione di bag con una struttura dati binary search tree
- `array_bag`: implementazione di bag con un array come struttura dati

Il subject ci chiede implementare due extra classi:

- `searchable_array_bag`
- `searchable_tree_bag`

che ereditano dalle rispettive classi ed implementano la funzione di `searchable_bag`.

Poi ci chiede di implementare una classe `set`, che wrappa `searchable_bag` in un set ( non ammette duplicati ).

#### bag

É la classe piú astratta, lei promette tre metodi:

- `insert`
- `print`
- `clear`

ma non dice come.

#### array_bag

Ha giá implementato i tre metodi ed aggiunte 2 attributi: l'array ed il size.

#### tree_bag

Stessa cosa di array, ma al posto dell'array definisce la struct tree.

#### searchable_bag

Promette solo il metodo `has`, che andrá implementato nelle due nuove classi che dobbiamo creare a partire da array e tree bag.

#### searchable_array_bag

`has` sará un banale for loop all'interno dell'array per cercare l'elemento.

#### searchable_tree_bag

`has` diventa piú complesso da implementare. Il tree bag di base crea una struttura simile a questa:

```
        8
      /   \
     3     10
    / \      \
   1   6      14
```

La regola di base é:

- se nodo in cui sto é maggiore di quello che sto cercando, vado a sinistra
- se nodo in cui sto é minore di quello che sto cercando, vado a destra

quindi se cerco 6:

```
parto da 8
6 < 8 → vado a sinistra

sono su 3
6 > 3 → vado a destra

sono su 6
trovato
```

```
current = tree

while current non è NULL
{
    se value == current->value
        return true

    se value < current->value
        current = current->l
    altrimenti
        current = current->r
}

return false
```

#### set

`set` é un bag senza duplicati, ci prendiamo la `searchable_bag` in modo da avere il metodo `has`, ed `insert` andrá implementato col controllo se il numero é giá presente o meno.

Quindi in `insert(int n)` mettiamo un controllo, e su `insert(int *items, int size)` richiameremo `insert` per ogni elemento dell'array di int.

## Parte 2

### Game Of Life

Funzioni ammesse: `atoi, read, putchar, malloc, calloc, realloc, free`.

#### Scopo dell'esercizio

Devo creare una versione di Game Of Life di Conway.

Comando da terminale:

```
./life width height iterations
```

Crea una griglia width x height e deve riprodurre le regole di GoL per n iterazioni. La configurazione iniziale viene estrapolata dai comandi `wasd` iniziali.
Idealmente dobbiamo immaginarci una penna che parte dall'angolo in alto a sinistra della griglia. `wasd` spostano la penna mentre `x` alterna lo stato della penna da alzata ad abbassata e viceversa.

Se la penna é abbassata, la cella viene marcata come viva. Una volta terminata la lettura dei comandi, la griglia rappresenta la generazione iniziale da cui poi partiranno le n iterazioni.

Ad ogni iterazione ogni cella osserva le 8 celle adiacenti ( verticalmente, orizzontalmente, diagonalmente ) ed il suo stato nella generazione successiva viene determinato dal numero di celle vicine "vive".

- La cella viva viene segnata con `O` (zero)
- La cella morta viene segnata con ` ` ( spazio )

Esempio pratico:

```
echo 'dxss' | ./life 3 3 0
```

Crea una griglia 3 x 3:

```
P..
...
...
```

`P` é la posizione iniziale della penna, parte da alzata. Con le istruzioni passate sullo stdin la penna fa questo:

- va a destra ( non disegna perché é alzata )
- abbassa la penna
- la cella corrente diventa viva

```
.0.
...
...
```

- la penna scende ( e disegna )
- la penna scende di nuovo ( e disegna )

risultato finale:

```
.0.
.0.
.0.
```

Essendoci state 0 iterazioni, l'output sará:

```
 0
 0
 0
```

Dopo aver letto i comandi, bisogna eseguire le regole di GoL.

##### Regole

- **Cellula viva**: 2 o 3 vicini → vive, altrimenti muore.
- **Cellula morta**: esattamente 3 vicini → torna in vita, altrimenti rimane morta.

Tutte le celle devono essere aggiornate contemporaneamente, quindi ogni generazione deve essere calcolata a partire dalla config precedente completa, senza modificare la griglia mentre la si sta analizzando.

Ipotizziamo 1 iterazione:

- la cella in [0][0] ha una sola cella vicina, quindi muore
- la cella in [0][1] ha una sola cella vicina, quindi muore
- la cella in [1][0] ha tre celle vicine, quindi resuscita
- la cella in [1][1] ha due celle vicine vive, quindi vive
- la cella in [1][2] ha 3 celle vicine, quindi resuscita
- la cella in [2][0] ha 2 celle vicine, quindi rimane morta
- la cella in [2][1] ha una sola cella vicina, quindi muore
- la cella in [2][2] ha due celle vicine, quindi rimane morta

Risultato finale dopo 1 iterazione:

```
000
```

#### Struttura logica del programma

1. leggere i parametri passati ( width, height e iterazioni )
2. creiamo la griglia vuota
3. leggiamo i comandi ed iniziamo a disegnare le cellule vive
4. simuliamo le iterazioni e stampiamo il risultato

Abbiamo bisogno sostanzialmente di due matrici perché una serve da reference per disegnare il risultato, altrimenti la situazione cambia in continuazione e non abbiamo un risultato basato sullo stato delle celle precedente all'iterazione attuale.

### BSQ

Funzioni ammesse: `malloc, calloc, realloc, free, fopen, fclose, getline, fscanf, fputs, fprintf, stderr, stdout, stdin, errno`.

#### Scopo dell'esercizio

Il programma deve leggere una mappa. Il file verrá passato o come argomento argv, oppure dallo stdin.
Il file della mappa contiene un header dove vengono dichiarati in ordine:

- Il numero delle righe della mappa
- Il char che segna la posizione vuota
- Il char che segna l'ostacolo
- Il char di riempimento

La mappa inizialmente sará composta da char vuoti e char ostacolo.

Data una mappa valida, dobbiamo trovare il quadrato piú grande fillabile con il char di riempimento. Se esistono piú quadrati della stessa grandezza, dobbiamo stampare quello posizionato piú in alto. A paritá di altezza, quello piú a sinistra.

#### Panoramica funzioni ammesse

- Per la gestione della memoria abbiamo a disposizione `malloc, calloc, realloc, free`.

- Gestione dei file e input:

Per leggere da un file passato come argomento:

```c
FILE *fopen(const char *filename, const char *mode);
```

Si prende `argv[1]`, il mode sará impostato su read ( quindi `"r"` ) e ritornerá un puntatore di tipo `FILE`, che sará sostanzialmente il nostro segnalibro all'interno del file.

Se `fopen` fallisce (perché il file non esiste o mancano i permessi), ritorna `NULL` - quindi stamperemo `"map error\n"`, ed imposta `errno` su `ENOENT` se il file non esiste. Non ci frega granché per l'esercizio, ma é sempre utile saperlo.

```c
fclose(FILE *stream);
```

Quando finiamo con la lettura del file dobbiamo chiuderlo, altrimenti file descriptor leak. Va chiamato sempre, anche in caso di errori.

**stdin**

Ok non é propriamente una funzione ma un flusso giá aperto dal sistema, lo useremo in caso non fossero stati passati argomenti. Il flusso é dello stesso tipo di `fopen` (`FILE *`).

- Lettura file:

**getline** lo conosciamo. Arriva fino a newline o fino alla fine del file. Se impostiamo il buffer a `NULL` e la dimensione a `0`, `getline` utilizza `malloc` per allocare l'esatta quantitá di memoria necessaria per contenere la riga, se quella successiva é piú lunga, userá `realloc`. Insomma gestisce tutto lui.

```c
int fscanf(FILE *stream, const char *format, ...);
```

`fscanf` é interessante perché legge i dati da un flusso e li analizza secondo un formato specifico. Nel chiamarla le diciamo esattamente cosa ci aspettiamo dalla riga letta e le salva in variabili di cui passiamo l'indirizzo.

Esempio:

```c
fscanf(stream, "%d %c %c %c", &linee, &vuoto, &ostacolo, &pieno)
```

In sostanza gli stiamo dicendo: da stream devi aspettarti un int e 3 char e me li devi salvare in `linee, vuoto, ostacolo, pieno`. Ovviamente queste sono variabili che abbiamo creato prima di chiamarla, e gli passiamo l'indirizzo di memoria.

**RITORNO**: `fscanf` ritorna il numero di elementi assegnati con successo. Quindi a fine chiamata dobbiamo verificare che ritorni esattamente 4.
Oppure `EOF` che solitamente é -1.

Quando `fscanf` raggiunge l'ultimo carattere, lascia "fuori" la newline che quindi dovremo consumare prima di procedere a leggere il resto della mappa.

- Funzioni di stampa:

```c
fputs(const char *s, FILE *stream);
```

Stampa una stringa nel flusso, senza formattazione. Ha senso usarla per stampare l'errore su `stderr`, o le righe della mappa risolta su `stdout`.

```c
fprintf(FILE *stream, const char *format, ...);
```

Rispetto alla classica `printf` ti permette semplicemente di scegliere su quale flusso stampare.

#### Logica del programma

1. Parsing e validazione
2. Memorizzazione
3. Algoritmo

##### 1. Parsing e validazione

**Header:**

- non deve contenere caratteri uguali
- sono tutti char printabili
- char_num == 4

**Mappa:**

- tutte le righe devono avere la stessa lunghezza
- deve esserci almeno una riga e almeno un quadrato
- alla fine di ogni riga abbiamo una newline
- vengono usati solo i char indicati nell'header

Se il parsing fallisce per mappa invalida dovremo ritornare su `stderr` `"map error\n"`.

##### 2. Memorizzazione

La cosa piú sensata é usare:

- una struct ( `t_map` ) in cui ci salviamo tutto ció che ci serve ( elementi header ), matrice bidimensionale di char dove salvare la mappa.
- una seconda struct ( `t_square` ) per tenere in memoria il quadrato piú grande trovato finora. Ci salveremo al suo interno la grandezza ( `int size` ) e la sua posizione ( `int row, int col` ). A livello di algoritmo ci sará un controllo se la `size` del nuovo quadrato é > di `size` salvata, se cosí fosse, sostituisce. Se é uguale o inferiore, non fará nulla. In questo modo, il quadrato salvato sará sempre il primo effettivamente trovato.

##### 3. Algoritmo

Ogni cella della mappa deve rispondere ad una domanda: se io fossi l'angolo in basso a destra di un quadrato, quanto sarebbe grande questo quadrato?

Per rispondere a questa domanda basta guardare i valori di 3 celle adiacenti:

- quella sopra ( `i - 1, j` )
- quella a sinistra ( `i, j - 1` )
- quella in alto a sinistra ( `i - 1, j - 1` )

Creiamo una matrice numerica di supporto, delle stesse dimensioni della mappa, e la compiliamo dall'alto verso il basso, da sinistra verso destra applicando 3 regole:

1. Gli ostacoli assumono valore 0
2. Tutte le celle sui bordi a sinistra hanno valore 1
3. Per tutte le altre celle vuote, il loro valore é uguale al numero piú piccolo tra le celle adiacenti da controllare + 1.

Per me é stregoneria, ma a quanto pare funziona.

Esempio:

mappa iniziale:

```
...
...
..o
...
```

primo step numerica ( celle bordi a sinistra ):

```
1111
1???
1???
1???
```

step 2: applicazione regole

```
1111
1222
1???
1???
```

- (1,1) i suoi vicini sono tutti 1, quindi il minimo é 1. Valore cella 1 + 1.
- (1,2) i suoi vicini sono 1, 1 e 2. Il minimo é 1. Valore della cella 1 + 1.
- (1,3) i suoi vicini sono 1, 1 e 2. Il minimo é 1. Valore della cella 1 + 1.

Andiamo avanti:

```
1111
1222
1201
1???
```

- (2,1) i suoi vicini sono 1, 1 e 2. Il minimo é 1. Valore della cella 1 + 1.
- (2,2) questa cella é un ostacolo nella mappa iniziale, quindi il suo valore é 0.
- (2,3) il valore minimo dei suoi vicini é 0, il suo valore é 0 + 1.

Ultimo passaggio:

```
1111
1222
1201
1211
```

Mentre compiliamo la matrice ci salviamo la struttura `t_square` solo se il numero inserito é strettamente maggiore di quello salvato in precedenza, in questo caso il numero piú alto in questione sará 2.
