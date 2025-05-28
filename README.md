## Game of Life

Salut! Am implementat Game of Life, un proiect care este format din 4 task-uri: folosind matrice (implementare normala); folosind stiva de liste pentru fiecare generatie (pentru stocare eficienta); folosind arbore binar (implementarea regulilor noului joc + parcurgere preordine) si gasirea lantului maxim Hamiltonian din fiecare valoare a nodului din arbore!

## Compilare

### Cu Makefile:
- **Compilare:** `make`
- **Rulare:** `make run`
- **Curățare:** `make clean`

### Fără Makefile (manual):

### Odata ce au fost create toate fisierele din input si de ref (luate de pe proiectul principal)!
gcc Pb_GOL.c
./a.out InputData/input1.txt (...)

## Checkerul checker-linux-amd64

Proiectul include un checker si se ruleaza folosind comenzile mentionate mai sus!

### Ce trebuie neaparat nevoie proiectul pentru a fi rulat?:
- `valgrind`   
- `cppcheck`  
- `git`


## Structuri folosite

`
// Structuri pentru Task 2 (stiva)
typedef struct
{
    void* ptr_val;  // Folosim void* pentru pointeri
    int i_val;
    char c_val;
} Data;
`

`
typedef struct Nod
{
    Data val;
    struct Nod* next;
} Nod;
`

`
// Structură pentru coordonate - lista simplu înlănțuită (Task 2)
typedef struct Coord
{
    int x, y;
    struct Coord* next;
} Coord;
`

`
typedef struct celula
{
    int x, y;
    struct celula *urm;
} Celula;
`

`
typedef struct arbore
{
    Celula *val;
    struct arbore *stanga, *dreapta;
} Node;
`

`
// Structuri pentru Task 4 (graf)
typedef struct {
    int V;        // nr. varfuri (noduri valide)
    int E;        // nr. muchii
    int **a;      // matricea de adiacenta
    int rows, cols;  // dimensiunile matricei originale
    int **hartaNoduri;   // mapare din coordonate (i,j) in numarul nodului
    int *nodLaLinie;  // mapare din numarul nodului in linia sa
    int *nodLaColoana;  // mapare din numarul nodului in coloana sa
} Graph;
`

## Task 1 -- Implementarea jocului

Pentru primul task, se citeste matricea si pentru usurinta atunci cand declaram matricea vom crea si conturul matricei (grid-ul) pentru a afla numarul de vecini cu usurinta fara a mai face verificari suplimentare! Se citeste matricea initiala obisnuita. Dupa aceea se parcurge fiecare generatie si la fiecare iteratie: se afiseaza generatia curenta cu spatiul aferent, se foloseste regula jocului pentru a construi urmatoarea generatie si se va afisa noua generatie, samd. Se va stoca fiecare generatie intr-un tensor (un vector de matrice) si problema a fost terminata!

## Task 2 -- Stocarea eficienta

Pentru cel de-al doilea task, am implementat niste functii care initializeaza matricea initiala, o citeste, declaram o stiva de generatii cu NULL pentru inceput. Dupa aceea, ca la task 1, vom calcula generatia urmatoare si vom avea astfel matricea generatiei urmatoare. Se vor pune intr-o lista toate celulele care si-au schimbat starea de la matricea de inceput la cealalta generatie si se va actualiza matricea generatiei urmatoare. In final, se va afisa stiva cu toate listele de schimbari inversand stiva la afisare pentru a afisa in ordinea ceruta si dam eliberare pentru stivasi problema a fost terminata!

## Task 3 -- Implementare arbore

Pentru cel de-al treilea task, trebuie sa cream un arbore, citim linie cu linie, construim matricea initiala, construim fiecare element al arborelui si tinem cont ca arborele construit ce porneste de la matricea initiala este un arbore binar perfect cu k generatii. In ce consta idea problemei, este sa se faca trecerea de la liste la matrice si o functie care face de la matrice la liste. Singura parte grea a problemei este ca elementele arborelui nu pot sa fie formate din matrice, ci decat dintr-o lista cu celulele vii. Prin urmare, folosim cele doua functii pentru implementarea regulilor vechi si noi pentru arbore (regula veche--dreapta si regula noua--stanga) si cream arborele ca in cerinta. Se parcurge arborele in preordine (radacina--se afiseaza, dupa stanga, dupa dreapta) si problemna a fost terminata!

## Task 4 -- Implementare graf

Pentru ultimul task, trebuie parcurs fix ca mai sus fiecare element din arbore si cand ajungem la un element trebuie sa aflam cel mai mare lant Hmailtonian cu proprietatea din enunt. Prima data, cream graful din matrice, mapam din coordonate in noduri, le asignam valori, alocam spatiu pentru toate elemntele din structura pentru graf si facem maparea inversa. Vom construi matricea de adiacenta a grafului neorientat si vom construi si muchiile. In acest graf, trebuie sa luam fiecare componenta conexa si in fiecare dintre componentele conexe sa vedem daca exista sau: nu lant Hmailtonian folosind o functie de backtracking (care incearca fiecare varianta pana o gaseste pe cea buna). Cu DFS luam toate componentele conexe, le separam si trebuie sa gasim un lant Hamiltonian in fiecare dintre componentele conexe gasite. Vom lua alta functie care incearca sa gaseasca lantul Hamiltonian cu nodurile sortate ca in enunt. Acum, pentru fiecare element din arbore, in penultima functie care va fi ultilizata si in main, se gaseste cel mai lung lant Hamiltonian maxim din toata matricea, daca se gaseste se afiseaza lungimea sa si coordonatele fiecarui elemnt cu spatiile de rigoare, daca nu se va afisa -1 si end line. In final, vom elibera graful, ca la o structura normala, si, astfel, problema a fost terminata!

Acesta a fost proiectul meu! Multumesc pentru atentia acordata!
