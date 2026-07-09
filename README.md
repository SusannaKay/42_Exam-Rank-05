# Exam-Rank-05

## Parte 1

### BigInt

#### Cosa chiede il subject

Il subject ci chiede di creare una classe che possa salvare al suo interno un intero unsigned di lunghezza indefinita. Gestendo quindi senza limiti ogni numero ( oltre quindi int_max ).
La classe in questione deve supportare l'addizione, la comparazione e lo shift di digit. 

per digit shift si intende qualcosa che assomiglia allo shift di bit ma in base 10. 
Ad esempio 
42 << 3 = 42000 
1337 >> 2 = 13
Il bigInt deve essere stampabile con << (in base 10) e l'output non deve avere nessuno zero a sinistra.
Ci verrá fornito un main di testing.

L'unico modo per gestire un int senza limite é salvarlo in una stringa in cui ogni carattere ovviamente é una cifra.

#### Cose da ripassare:

- stringhe
- operator+
- confronti
- shift << e >>=

#### Cose da tenere a mente

Una parte critica di questo esercizio sono ovviamente le operazioni: non possiamo fare stoi(), perché l'int potrebbe essere troppo grande, per questo motivo dobbiamo simulare la somma "a mano".

123 +
456

3 + 6 = 9
2 + 5 = 7
1 + 4 = 5

risultato: la stringa "579". Da gestire quindi il riporto ( quando capita 9 + 1 ).
La lettura della stringa per effettuare la somma deve ovviamente partire da destra quindi da s.size() - 1

Altra cosa da tenere in considerazione sono le lunghezze diverse delle stringhe: se abbiamo 

999999 +
1

l'idea di base deve sempre essere quella di 
1. partire dall'ultima cifra
2. Sommare
3. gestire il carry
4. costruire il risultato al contrario
5. invertire il risultato

Shift vari: 

<<  dobbiamo aggiungere zeri a destra - unica eccezione  lo zero che deve rimanere zero

>>= rimuovi cifre da destra - se le cifre da rimuovere sono di piú di quelle contenute devo stampare zero

#### Costruttore

Prende un int e lo trasforma in stringa. In C++98 non abbiamo to_string() quindi dobbiamo usare std::ostringstream

std::ostringstream - output string stream
libreria: <sstream>
é come un std::cout che non scrive su terminale ma dentro ad uno stream in memoria che poi convertiamo in stringa.

quindi idealmente
bigInt(42){
    std::ostringstram out;
    out << 42
    std::string str = oss.str();
} - ignorando il fatto che in realtá andremo a fare una assegnazione all attributo privato della classe stessa ( non dichiarando quindi una stringa extra ), ma é giusto per capire

#### Comparazioni

La prima cosa da guardare é la lunghezza del numero. A paritá di lunghezza il confronto parte dalle cifre di sinistra e quindi da inizio stringa a quel punto posso confrontare direttamente le due stringhe. 

#### Operator+

inizio a scorrere le stringhe da destra, loopo finché non ho finito tutte e due le liste e addizionato l ultimo resto. controllo se str1 non é finita, e aggiungo alla somma, poi controllo se str2 non é finita e aggiungo alla somma. il nuovo resto é somma / 10, mentre il risultato é somma % 10 ritrasformato in char.
faccio std::reverse(result.begin(), result.end()), salvo in un oggett temporaneo e rtirono tmp

#### Operator+=

this = this + other

#### Operator++

this += bigInt(1)

#### Preincremento bigint &bigint::operator++()
ritorna puntatore, come operator++ 

#### Post incremento bigint bigint::operator++(int) <- ritorna una reference alla copia col vecchio valore ma aggiorna l originale a quello successivo >
creo oggetto tmp con this, poi addiziono bigInt(1) a this e ritorno tmp

#### operator<<==
Modifica l oggetto iniziale. Dobbiamo distinguere tra caso normale e caso 0. Se zero, ritorna *this direttamente
se caso normale aggiungi n volte il carattere "0" alla stringa 

#### Operator<<
questo deve aggiungere n zeri alla stringa ( a destra ), l operazione non modifica l'oggetto, ne deve creare uno nuovo facendo tmp<<= amount

#### Operator>>=
deve togliere n cifre a destra, se finiscono le cifre ritorna zero
bigint &operator>>=(const bigint &other);
quindi other ti dice quante cifre togliere a this, va fatto un mini atoi e poi chiamata la versione unsigned int dello stesso metodo. questo controlla che n non "esaurisca" tutta la stringa ed in caso ritorna zero
altrimenti chiama resize(). Resize diminuisce la stringa di n char

#### Operator >> 

differisce da >>= semplicemente perché non va a modificare l'oggetto che la chiama, quindi va a fare le modifiche su un oggetto tmp che poi restituirá 

### vect2

#### Cosa chiede il subject

Il subject ci chiede di creare una classe vect2 che contiene 2 int ed agisca come vector. Dobbiamo fare l'overload di stampa, addizione, sottrazione e moltiplicazione come se fosse uno scalare. Inoltre dobbiamo implementare anche l'overload di [] per accedere agli elementi senza l'obbligo di controllare se l'indice sia corretto. La classe deve rappresentare in sostanza un vettore matematico {x,y}.
Da main poi abbiamo i classici overload di pre e post incremento/decremento, +=, -=, *=, == e !=.

Sorvolo sul ripasso degli overload perché l'ho giá fatto in bigint

### struttura della classe

Abbiamo due opzioni: o creare due int come attributo, oppure un semplice array di int che semplifica ulteriormente l'operator[]. Opto per quest'ultima soluzione. Avendo la necessitá di cambiare valore agli elementi dell'attributo della classe anche dal main, ho messo l'array in public.

### costruttori

vect2() inizializza l'array a 0, 0, gli altri sono banali da spiegare

### operator[]

[] nel main viene usato per due operazioni. Entrambe accedono all'n elemento dell'array di int. Uno peró ha un vect2 const in ingresso e impone la non modifica di quanto restituisce, l'altro no. Quindi abbiamo bisogno di due firme in modo da gestire entrambi i casi.
Non dovendo gestire gli out of range, la funzione semplicemente restituisce l'ennesimo elemento e basta.

### operator == e !=

controllano se entrambi i valori sono == o != e restituiscono un bool, facile facile

### operator += ( e -= e *= )
Come prima: devono modificare l'oggetto quindi ritorneranno una reference a this. Inoltre dobbiamo considerare in alcuni casi l'overload accetterá un int ( ad esempio *= ), in altri solo un oggetto. il principio é sostanzialmente lo stesso peró

Da adesso spiego solo alcuni operandi che hanno una sorta di eccezione.

### operator-

ne abbiamo di due tipi. Uno unario e l'altro binario. Il binario segue la logica di tutti gli altri operator. Quello unario invece non prende parametri ed il suo scopo é quello di moltiplicare per -1 l'array di int. 

### operator*

Ne abbiamo anche qui due. Uno dentro la classe e corrisponde alla classica moltiplicazione fra due oggetti. Quindi prende come parametro other. L'altro invece serve a gestire il 3 * v2 (o quello che é). Va inserito fuori dalla classe perché in questo caso l'operando di sinistra é un int, e non un oggetto, quindi non posso inserirlo come metodo della classe. 

### polyset

nell'esercizio ci vengono date alcune classi:
bag : classe astratta
searchable_bag : una implementazione di bag con il metodo che ci permette di cercare nella bag
tree_bag: implementazione di bag con una struttura dati binary search tree 
array_bag: implementazione di bag con un array come struttura dati

il subject ci chiede implementare due extra classi:
searchable_array_bag 
searchable_tree_bag 
che ereditano dalle rispettive classi ed implementano la funzione di searchable_bag 

poi ci chiede di implementare una classe set, che wrappa searchable_bag in un set ( non ammette duplicati ). 

#### bag
é la classe piú astratta, lei promette tre metodi
insert
print
clear
ma non dice come

#### array_bag
ha giá implementato i tre metodi ed aggiunte 2 attributi: l- array ed il size

#### tree_bag
stessa cosa di array, ma al posto dell'array definisce la struct tree

#### searchable_bag

promette solo il metodo has, che andrá implementato nelle due nuove classi che dobbiamo creare a partire da array e tree bag

#### searchable_array_bag
has sará un banale for loop all interno dell array per cercare l elemento

#### searchable_tree_bag

has diventa piú complesso da implementare. il tree bag di bse crea una struttura simile a questa 

        8
      /   \
     3     10
    / \      \
   1   6      14

la regola di base é 
se nodo in cui sto é maggiore di quello che sto cercando, vado a sinistra
se nodo in cui sto é minore di quello che sto cercando, vado a destra 
quindi
se cerco 6:
parto da 8
6 < 8 → vado a sinistra

sono su 3
6 > 3 → vado a destra

sono su 6
trovato

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

#### set

set é un bag senza duplicati, ci prendiamo la searchable_bag in modo da avere il metodo has, ed insert andrá implementato col controllo se il numero é giá presente o meno. 

quindi in insert(int n) mettiamo un controllo, e su insert (int * items, int size) richiameremo insert per ogni elemento dell-array di int. 