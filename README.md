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
quindi other ti dice quante cifre togliere a this
