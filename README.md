# Exam-Rank-05

Appunti sugli esercizi dell'Exam Rank 05 di 42: la prima parte in C++, la seconda in C.

- Parte 1: [BigInt](#bigint), [vect2](#vect2), [polyset](#polyset)
- Parte 2: [Game Of Life](#game-of-life), [BSQ](#bsq)

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

Prende un `unsigned int` (il bigint non ha segno) e lo trasforma in stringa. In C++98 non abbiamo `to_string()` quindi dobbiamo usare `std::ostringstream`.

`std::ostringstream` - output string stream
libreria: `<sstream>`
é come un `std::cout` che non scrive su terminale ma dentro ad uno stream in memoria che poi convertiamo in stringa.

quindi idealmente

```cpp
bigInt(42) {
    std::ostringstream out;
    out << 42;
    std::string str = out.str();
}
```

— ignorando il fatto che in realtá andremo a fare una assegnazione all'attributo privato della classe stessa ( non dichiarando quindi una stringa extra ), ma é giusto per capire.

Il costruttore di default invece mette direttamente `"0"`: una stringa vuota non é un numero, e tutte le altre funzioni darebbero per scontato che ci sia almeno una cifra.

#### Comparazioni

La prima cosa da guardare é la lunghezza del numero. A paritá di lunghezza il confronto parte dalle cifre di sinistra e quindi da inizio stringa: a quel punto posso confrontare direttamente le due stringhe.

Attenzione: il confronto per lunghezza funziona solo se non ci sono zeri a sinistra. `"0042"` é piú lunga di `"999"` ma é piú piccola. Nel mio codice gli zeri a sinistra non nascono mai (costruttore, somma e shift non li creano), quindi va bene, ma é il motivo per cui il subject insiste sull'output senza zeri iniziali.

Altra cosa: senza la lunghezza non potrei confrontare le stringhe e basta, perché il confronto tra stringhe é alfabetico: `"9" > "10"` per le stringhe é vero.

Una volta scritti `<` e `==`, gli altri si possono costruire da quelli: `a > b` é `b < a`, `a <= b` é `!(b < a)`, `a != b` é `!(a == b)`. Meno codice, meno posti dove sbagliare.

#### Operator+

Nel mio codice il lavoro vero lo fa `+=`, e `+` si appoggia a lui:

```cpp
bigint bigint::operator+(const bigint &other) const
{
    bigint tmp(*this);
    tmp += other;
    return (tmp);
}
```

`+` non deve modificare nessuno dei due operandi ( infatti é `const` ), quindi lavora su una copia.

Dentro `+=` inizio a scorrere le stringhe da destra, loopo finché non ho finito tutte e due le stringhe e addizionato l'ultimo riporto:

```cpp
while (i >= 0 || j >= 0 || carry)
```

Il `|| carry` é quello che gestisce `999 + 1`: le cifre finiscono, ma resta un riporto da scrivere.

Controllo se str1 non é finita, e aggiungo alla somma, poi controllo se str2 non é finita e aggiungo alla somma. Il nuovo riporto é `somma / 10`, mentre la cifra é `somma % 10` ritrasformata in char.

Per costruire il risultato ci sono due strade:

- aggiungo ogni cifra in fondo e alla fine faccio `std::reverse(result.begin(), result.end())`
- inserisco ogni cifra direttamente all'inizio con `res.insert(res.begin(), ...)`, e il risultato é giá nel verso giusto

Io ho usato la seconda. É piú semplice da leggere, ma ogni insert all'inizio sposta tutta la stringa, quindi costa di piú. Per l'esame non cambia niente.

#### Operator+=

Modifica l'oggetto e ritorna `*this` come reference, cosí si puó concatenare: `(c += a)` nel main viene stampato direttamente.

#### Operator++

```
this += bigInt(1)
```

#### Preincremento — `bigint &bigint::operator++()`

Incrementa e ritorna una **reference** a `*this`, cioé all'oggetto giá incrementato.

#### Postincremento — `bigint bigint::operator++(int)`

Ritorna una **copia** col vecchio valore ma aggiorna l'originale a quello successivo.
Creo oggetto tmp con this, poi addiziono `bigint(1)` a this e ritorno tmp.

Non puó ritornare una reference: `tmp` é una variabile locale, alla fine della funzione muore, e una reference a lei punterebbe a memoria che non esiste piú.

Il parametro `int` non si usa: serve solo al compilatore per distinguere `b++` da `++b`.

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
Altrimenti chiama `resize()`, a cui passo la nuova lunghezza, cioé `length - n`.

Nel main viene chiamato cosí:

```cpp
d >>= (const bigint)2
```

per questo serve la versione che prende un `bigint`.

Occhio al mini atoi: se il bigint che indica lo shift é enorme ( piú di 10 cifre ), non ci sta in un `unsigned int` e il numero "fa il giro", diventando piccolo. A quel punto toglierei poche cifre invece di azzerare. Per essere precisi, prima di convertire si puó confrontare con la lunghezza: se lo shift é >= del numero di cifre, il risultato é `"0"` e basta.

#### Operator >>

Differisce da `>>=` semplicemente perché non va a modificare l'oggetto che la chiama, quindi va a fare le modifiche su un oggetto tmp che poi restituirá.

#### Operator<< per la stampa

Da non confondere con lo shift: questo é quello di `std::cout << a`.

```cpp
std::ostream &operator<<(std::ostream &os, const bigint &a);
```

Sta fuori dalla classe, perché a sinistra c'é lo stream e non un bigint. Siccome `_num` é privato, uso il getter `getNum()`. Ritorna lo stream, cosí si possono concatenare piú `<<`.

### vect2

#### Cosa chiede il subject

Il subject ci chiede di creare una classe `vect2` che contiene 2 int ed agisca come vector. Dobbiamo fare l'overload di stampa, addizione, sottrazione e moltiplicazione per uno scalare. Inoltre dobbiamo implementare anche l'overload di `[]` per accedere agli elementi senza l'obbligo di controllare se l'indice sia corretto. La classe deve rappresentare in sostanza un vettore matematico {x,y}.
Da main poi abbiamo i classici overload di pre e post incremento/decremento, `+=`, `-=`, `*=`, `==` e `!=`.

Sorvolo sul ripasso degli overload perché l'ho giá fatto in bigint.

#### Struttura della classe

Abbiamo due opzioni: o creare due int come attributo, oppure un semplice array di int che semplifica ulteriormente l'operator[]. Opto per quest'ultima soluzione. Avendo la necessitá di cambiare valore agli elementi dell'attributo della classe anche dal main, ho messo l'array in public.

#### Costruttori

`vect2()` inizializza l'array a 0, 0, gli altri sono banali da spiegare.

#### operator[]

`[]` nel main viene usato per due operazioni. Entrambe accedono all'n-esimo elemento dell'array di int. Uno peró ha un `vect2 const` in ingresso e impone la non modifica di quanto restituisce, l'altro no. Quindi abbiamo bisogno di due firme in modo da gestire entrambi i casi.
Non dovendo gestire gli out of range, la funzione semplicemente restituisce l'ennesimo elemento e basta.

```cpp
int &operator[](int n);              // v1[1] = 12;  scrittura
const int &operator[](int n) const;  // v3[1] con v3 const, lettura
```

Il `const` finale é quello che conta: dice che il metodo si puó chiamare su un oggetto const. Senza la seconda versione, `v3[1]` con `v3` const non compilerebbe.

#### operator == e !=

`==` controlla che entrambi i valori siano uguali e restituisce un bool, facile facile.

`!=` invece non é "entrambi diversi": due vettori sono diversi se **almeno uno** dei valori é diverso. `{1, 2}` e `{1, 5}` sono diversi anche se la x é uguale.

Quindi se scrivo `!=` a mano devo usare `||`, non `&&`. Ancora piú semplice, lo costruisco dall'`==`:

```cpp
bool vect2::operator!=(const vect2 &other) const
{
    return (!(*this == other));
}
```

Nella prima versione avevo usato `&&`, e il main non se ne accorge perché confronta solo vettori con entrambi i valori diversi o entrambi uguali.

Avevo anche scritto due versioni di ognuno, una con `const bool` come tipo di ritorno. Non serve: basta una sola firma con il parametro const e il metodo const, che va bene sia per oggetti const che non const. E il `const` sul `bool` di ritorno non significa niente ( con gcc e `-Wextra -Werror` non compila proprio ).

#### operator += ( e -= e *= )

Come prima: devono modificare l'oggetto quindi ritorneranno una reference a this. Inoltre dobbiamo considerare in alcuni casi l'overload accetterá un int ( ad esempio `*=` ), in altri solo un oggetto. Il principio é sostanzialmente lo stesso peró.

Ritornare la reference permette anche questa riga del main:

```cpp
v2 += v2 += v3;
```

Si legge da destra: prima `v2 += v3` ( v2 diventa {10, 20} ), che ritorna v2 stesso, poi `v2 += v2` ( {20, 40} ).

#### Incremento e decremento

`++` e `--` aumentano o diminuiscono di 1 entrambe le componenti. Stessa logica del bigint: il pre ritorna una reference all'oggetto modificato, il post una copia del valore vecchio.

Da adesso spiego solo alcuni operandi che hanno una sorta di eccezione.

#### operator-

Ne abbiamo di due tipi. Uno unario e l'altro binario. Il binario segue la logica di tutti gli altri operator. Quello unario invece non prende parametri ed il suo scopo é quello di moltiplicare per -1 l'array di int.

#### operator*

Ne abbiamo anche qui due, tutti e due vettore per scalare.

Uno dentro la classe gestisce `v3 * 2`: a sinistra c'é il vettore, quindi puó essere un metodo, e prende come parametro l'int.

```cpp
vect2 operator*(int n) const;
```

L'altro invece serve a gestire il `3 * v2`. Va inserito fuori dalla classe perché in questo caso l'operando di sinistra é un int, e non un oggetto, quindi non posso inserirlo come metodo della classe.

```cpp
vect2 operator*(int n, const vect2 &other);
```

Il trucco é che dentro richiama semplicemente la logica dell'altro: `3 * v` é uguale a `v * 3`.

### polyset

Nell'esercizio ci vengono date alcune classi:

- `bag`: classe astratta
- `searchable_bag`: ancora astratta, eredita da bag e aggiunge il metodo che ci permette di cercare nella bag
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

#### Il diamante

Le classi che creiamo ereditano da due genitori:

```cpp
class searchable_array_bag : public array_bag, public searchable_bag
```

E tutti e due i genitori ereditano da `bag`:

```
            bag
          /     \
   array_bag   searchable_bag
          \     /
    searchable_array_bag
```

É lo stesso diamond problem di DiamondTrap in CPP03. Se nessuno facesse niente, dentro `searchable_array_bag` ci sarebbero due `bag`.

Per questo nei file dati `array_bag`, `tree_bag` e `searchable_bag` ereditano con `virtual public bag`: cosí di `bag` ce n'é una sola, condivisa. Non lo dobbiamo scrivere noi, ma all'esame é una delle domande classiche sul perché funziona.

Grazie a questo, `insert`, `print` e `clear` arrivano giá implementati da `array_bag` / `tree_bag`, e `has` lo scriviamo noi. Le nostre classi quindi non sono piú astratte e si possono istanziare.

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

Questa regola vale perché é la stessa che usa `tree_bag::insert`: valori piú piccoli a sinistra ( `l` ), maggiori o uguali a destra ( `r` ). La ricerca deve seguire esattamente la stessa strada dell'inserimento.

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

Il caso tipico é invertire `l` e `r`. Nella prima versione del mio codice era proprio cosí: la ricerca trovava solo la radice, e per tutti gli altri valori andava dalla parte sbagliata.

E il main non se ne accorge: scrive sempre `Found in T`, cambia solo il colore ( verde se trovato, rosso se no ). Senza guardare i colori sembra tutto giusto.

Per ogni passo scendo di un livello, quindi su un albero bilanciato la ricerca é O(log n), contro l'O(n) del for loop su array. Se peró inserisco numeri giá ordinati ( 1 2 3 4 5 ), l'albero diventa una specie di lista tutta a destra e si torna a O(n).

#### set

`set` é un bag senza duplicati, ci prendiamo la `searchable_bag` in modo da avere il metodo `has`, ed `insert` andrá implementato col controllo se il numero é giá presente o meno.

`set` non contiene i dati: tiene una reference a una `searchable_bag` che esiste giá, e lavora su quella.

```cpp
class set : public searchable_bag
{
private:
    searchable_bag &_bag;
public:
    set(searchable_bag &other);
    ...
};
```

Nel main:

```cpp
set sa(*a);
```

`sa` lavora direttamente sull'array bag `a`, non su una copia.

Siccome é una reference, va inizializzata nella initialization list del costruttore ( come HumanA in CPP01 ).

Il fatto che `_bag` sia una `searchable_bag &` e non un tipo preciso vuol dire che il set funziona sia con l'array che con il tree: é polimorfismo.

`print`, `clear` e `has` girano semplicemente la chiamata a `_bag`. L'unica che cambia é insert.

Quindi in `insert(int n)` mettiamo un controllo:

```cpp
if (_bag.has(n))
    return;
_bag.insert(n);
```

e su `insert(int *items, int size)` richiameremo `insert` per ogni elemento dell'array di int, cosí il controllo sui duplicati vale anche lí.

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

- La cella viva viene segnata con `O` ( la lettera O maiuscola, non lo zero )
- La cella morta viene segnata con ` ` ( spazio )

Negli schemi qui sotto, durante il disegno uso `.` per le celle morte cosí si vedono. Nell'output vero sono spazi.

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
.O.
...
...
```

- la penna scende ( e disegna )
- la penna scende di nuovo ( e disegna )

risultato finale:

```
.O.
.O.
.O.
```

Essendoci state 0 iterazioni, l'output sará:

```
 O 
 O 
 O 
```

( ogni riga é lunga `width`, quindi c'é anche lo spazio finale )

Se la penna arriva al bordo, il movimento che la porterebbe fuori viene semplicemente ignorato.

Dopo aver letto i comandi, bisogna eseguire le regole di GoL.

##### Regole

- **Cellula viva**: 2 o 3 vicini → vive, altrimenti muore.
- **Cellula morta**: esattamente 3 vicini → torna in vita, altrimenti rimane morta.

Tutte le celle devono essere aggiornate contemporaneamente, quindi ogni generazione deve essere calcolata a partire dalla config precedente completa, senza modificare la griglia mentre la si sta analizzando.

Ipotizziamo 1 iterazione ( `echo 'dxss' | ./life 3 3 1` ), partendo dalla colonna centrale viva:

- [0][0] morta, 2 vicine vive ( [0][1] e [1][1] ) → rimane morta
- [0][1] viva, 1 vicina viva ( [1][1] ) → muore
- [0][2] morta, 2 vicine vive → rimane morta
- [1][0] morta, 3 vicine vive ( tutta la colonna centrale ) → nasce
- [1][1] viva, 2 vicine vive ( sopra e sotto ) → sopravvive
- [1][2] morta, 3 vicine vive → nasce
- [2][0] morta, 2 vicine vive → rimane morta
- [2][1] viva, 1 vicina viva → muore
- [2][2] morta, 2 vicine vive → rimane morta

Risultato finale dopo 1 iterazione:

```
...
OOO
...
```

cioé nell'output:

```
   
OOO
   
```

La linea verticale é diventata orizzontale. Con 2 iterazioni torna verticale, e cosí via: si chiama blinker, é l'oscillatore piú semplice di Game of Life ed é un ottimo test.

Le celle sul bordo hanno meno di 8 vicini: quello che sta fuori dalla griglia conta come morto. Per questo in `countAlive` ogni controllo verifica prima di non uscire dalla matrice.

#### Struttura logica del programma

1. leggere i parametri passati ( width, height e iterazioni )
2. creiamo la griglia vuota
3. leggiamo i comandi ed iniziamo a disegnare le cellule vive
4. simuliamo le iterazioni e stampiamo il risultato

Abbiamo bisogno sostanzialmente di due matrici perché una serve da reference per disegnare il risultato, altrimenti la situazione cambia in continuazione e non abbiamo un risultato basato sullo stato delle celle precedente all'iterazione attuale.

La struct oltre a salvarsi gli argomenti passati e le due matrici, si salverà anche la posizione della penna (p_x, p_y).

Inizializziamo la struct
Controlliamo che ogni char sia un digit
Poi atoi ed inizializzo le due matrici.
Con clear_matrix imposto ogni char a spazio e facciamo partire la penna dalla posizione (0,0).
Spostiamo la penna in base ai char letti dallo stdin e modifichiamo la posizione o lo stato della penna in base ai char letti, controllando sempre di non stare alla fine o all'inizio della matrice.
se la penna è abbassata, impostiamo il char a '0'.
poi per n iterazioni chiamiamo play.
Su ogni cella di board contiamo le cellule vive in tutte le direzioni con countAlive ed in base a questo, modifichiamo la cella in next

Ci copiamo il risultato di next in Board e puliamo next. 
Alla fine delle iterazioni stampiamo e liberiamo tutto.

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
- vengono usati solo i char indicati nell'header ( vuoto e ostacolo: il char pieno non puó comparire nella mappa di partenza )
- il numero di righe deve essere esattamente quello dell'header: né meno, né di piú

Per l'ultimo controllo, dopo aver letto le righe dichiarate provo a leggere ancora un char: se ci riesco, il file ha righe in piú e la mappa non é valida.

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
2. Le celle vuote della prima riga e della prima colonna hanno valore 1 ( non hanno nessuno sopra o a sinistra, quindi al massimo sono un quadrato 1x1 )
3. Per tutte le altre celle vuote, il loro valore é uguale al numero piú piccolo tra le celle adiacenti da controllare + 1.

Per me é stregoneria, ma a quanto pare funziona.

Ok, provo a spiegarmela. Se la cella in alto, quella a sinistra e quella in diagonale sono tutte angoli di quadrati almeno 2x2, allora i tre quadrati insieme coprono tutto il quadrato 3x3 che finisce nella mia cella. Basta che uno dei tre sia piú piccolo ( o un ostacolo ) e il quadrato grande non ci sta. Quindi il mio quadrato puó essere grande quanto il piú piccolo dei tre, piú la mia cella: `min + 1`.

Il vantaggio é che ogni cella la guardo una volta sola: O(righe x colonne), invece di provare tutti i quadrati possibili da ogni posizione.

Esempio:

mappa iniziale:

```
....
....
..o.
....
```

primo step numerica ( prima riga e prima colonna ):

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

Il primo 2 lo troviamo in (1,1): `row` e `col` di `t_square` sono quindi l'angolo **in basso a destra** del quadrato.

Perché lo "strettamente maggiore" rispetta la regola del subject ( piú in alto, poi piú a sinistra )? Scorro la matrice dall'alto in basso e da sinistra a destra. Due quadrati della stessa grandezza hanno l'angolo in basso a destra nello stesso ordine del loro angolo in alto a sinistra, quindi il primo che trovo é proprio quello piú in alto, e a paritá il piú a sinistra. Con `>=` invece vincerebbe l'ultimo.

#### Stampa

Dall'angolo in basso a destra risalgo all'angolo in alto a sinistra:

```c
for (int i = square->row - square->size + 1; i <= square->row; i++)
    for (int j = square->col - square->size + 1; j <= square->col; j++)
        map->grid[i][j] = map->full;
```

Poi stampo le righe con `fputs`. Le righe lette con `getline` hanno giá la loro `\n` finale, quindi non devo aggiungerla.

Risultato dell'esempio:

```
xx..
xx..
..o.
....
```

( con `x` come char pieno )
