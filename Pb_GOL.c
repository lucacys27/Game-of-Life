#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

typedef struct celula
{
    int x, y;
    struct celula *urm;
} Celula;

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

typedef struct arbore
{
    Celula *val;
    struct arbore *stanga, *dreapta;
} Node;

// Structuri pentru Task 2 (stiva)
typedef struct
{
    void* ptr_val;  // Folosim void* pentru pointeri
    int i_val;
    char c_val;
} Data;

typedef struct Nod
{
    Data val;
    struct Nod* next;
} Nod;

// Structură pentru coordonate - lista simplu înlănțuită (Task 2)
typedef struct Coord
{
    int x, y;
    struct Coord* next;
} Coord;

// Directiile pentru toti cei 8 vecini: sus, jos, stanga, dreapta + diagonale (Task 4)
int directiiX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int directiiY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

void elibereazaGraf(Graph *g);
Graph* creeazaGrafDinMatrice(const char matrice[][101], int rows, int cols);

Node* creazaNod()
{
    Node* nodNou = (Node*)malloc(sizeof(Node));
    nodNou->val = NULL;
    nodNou->stanga = NULL;
    nodNou->dreapta = NULL;
    return nodNou;
}

void listaLaMatrice(Celula *lista, int n, int m, char matrice[][101])
{
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            matrice[i][j] = '+';

    Celula *curent = lista;
    while (curent != NULL)
    {
        if (curent->x >= 1 && curent->x <= n && curent->y >= 1 && curent->y <= m)
            matrice[curent->x][curent->y] = 'X';
        curent = curent->urm;
    }
}

void matriceLaLista(const char matrice[][101], int n, int m, Celula **lista)
{
    Celula *curent = *lista;
    while (curent != NULL)
    {
        Celula *temp = curent;
        curent = curent->urm;
        free(temp);
    }
    *lista = NULL;

    Celula **coada = lista;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++) 
        {
            if (matrice[i][j] == 'X') 
            {
                Celula *noua = (Celula*)malloc(sizeof(Celula));
                noua->x = i;
                noua->y = j;
                noua->urm = NULL;

                if (*lista == NULL) 
                {
                    *lista = noua;
                    coada = &(*lista)->urm;
                } 
                else 
                {
                    *coada = noua;
                    coada = &noua->urm;
                }
            }
        }
    }
}

int numaraVeciniVii(int x, int y, int n, int m, const char matrice[][101])
{
    int numarVecini = 0;
    for (int di = -1; di <= 1; di++)
    {
        for (int dj = -1; dj <= 1; dj++)
        {
            if (di == 0 && dj == 0 || x + di < 1 || y + dj < 1 || x + di > n || y + dj > m) continue;
            if (matrice[x + di][y + dj] == 'X')
                numarVecini++;
        }
    }
    return numarVecini;
}

void regulaVeche(Node *rad, int n, int m, Celula *lista)
{
    char matrice[101][101], copie[101][101];
    listaLaMatrice(lista, n, m, matrice);
    memcpy(copie, matrice, sizeof(matrice));

    for (int x = 1; x <= n; x++)
    {
        for (int y = 1; y <= m; y++)
        {
            int vecini = numaraVeciniVii(x, y, n, m, matrice);
            if (matrice[x][y] == 'X')
            {
                if (vecini < 2 || vecini > 3) copie[x][y] = '+';
            }
            else
            {
                if (vecini == 3) copie[x][y] = 'X';
            }
        }
    }

    matriceLaLista(copie, n, m, &rad->val);
}

void regulaNoua(Node *rad, int n, int m, Celula *lista)
{
    char matrice[101][101], copie[101][101];

    listaLaMatrice(lista, n, m, matrice);

    memcpy(copie, matrice, sizeof(matrice));

    for (int x = 1; x <= n; x++)
    {
        for (int y = 1; y <= m; y++)
        {
            int vecini = numaraVeciniVii(x, y, n, m, matrice);
            if (vecini == 2) copie[x][y] = 'X';
        }
    }

    matriceLaLista(copie, n, m, &rad->val);
}

void construiesteArbore(Node* nod, int adancimeActuala, int k, int n, int m)
{
    if (adancimeActuala >= k)
    {
        nod->stanga = NULL;
        nod->dreapta = NULL;
        return;
    }

    nod->stanga = creazaNod();
    regulaNoua(nod->stanga, n, m, nod->val);
    construiesteArbore(nod->stanga, adancimeActuala + 1, k, n, m);

    nod->dreapta = creazaNod();
    regulaVeche(nod->dreapta, n, m, nod->val);
    construiesteArbore(nod->dreapta, adancimeActuala + 1, k, n, m);
}

void parcurgerePreordine(FILE *output, Node *rad, int n, int m)
{
    if (rad != NULL)
    {
        char matrice[101][101];
        listaLaMatrice(rad->val, n, m, matrice);
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
                fprintf(output, "%c", matrice[i][j]);
            fprintf(output, "\n");
        }
        fprintf(output, "\n");
        parcurgerePreordine(output, rad->stanga, n, m);
        parcurgerePreordine(output, rad->dreapta, n, m);
    }
}

void elibereazaArbore(Node* rad)
{
    if (rad == NULL) return;
    elibereazaArbore(rad->stanga);
    elibereazaArbore(rad->dreapta);

    Celula *curent = rad->val;
    while (curent != NULL)
    {
        Celula *temp = curent;
        curent = curent->urm;
        free(temp);
    }
    free(rad);
}

void initializeazaMatrice(int N, int M, char matrice[][101])
{
    for (int x = 0; x <= N + 1; x++)
        for (int y = 0; y <= M + 1; y++)
            if (x == 0 || y == 0 || x == N + 1 || y == M + 1) matrice[x][y] = '+';
            else matrice[x][y] = ' ';
}

void citesteMacriceInitiala(int N, int M, char matrice[][101], FILE* input_file)
{
    for (int x = 1; x <= N; x++)
        for (int y = 1; y <= M; y++)
            fscanf(input_file, " %c", &matrice[x][y]);
}

void calculeazaGeneratieUrmatoare(int N, int M, const char matrice[][101], char copie[][101])
{
    for (int x = 1; x <= N; x++)
    {
        for (int y = 1; y <= M; y++)
        {
            int veciniVii = numaraVeciniVii(x, y, N, M, matrice);

            if (matrice[x][y] == '+')
            {
                if (veciniVii == 3) copie[x][y] = 'X';
                else copie[x][y] = '+';
            }
            else
            {
                if (veciniVii == 2 || veciniVii == 3) copie[x][y] = 'X';
                else copie[x][y] = '+';
            }
        }
    }

    for (int x = 0; x <= N + 1; x++)
    {
        copie[x][0] = '+';
        copie[x][M + 1] = '+';
    }

    for (int y = 0; y <= M + 1; y++)
    {
        copie[0][y] = '+';
        copie[N + 1][y] = '+';
    }
}

void actualizeazaMatrice(int N, int M, char matrice[][101], const char copie[][101])
{
    for (int x = 0; x <= N + 1; x++)
        for (int y = 0; y <= M + 1; y++)
            matrice[x][y] = copie[x][y];
}

int esteGoala(const Nod* varf)
{
    return varf == NULL;
}

Data pop(Nod** varf)
{
    Data aux = {NULL, INT_MIN, 0};
    if (esteGoala(*varf))
    {
        return aux;
    }

    Nod* temp = *varf;
    aux = temp->val;
    *varf = (*varf)->next;
    free(temp);
    return aux;
}

void stergeStiva(Nod** varf)
{
    while (*varf != NULL)
    {
        Nod* temp = *varf;
        *varf = (*varf)->next;
        free(temp);
    }
}

// Funcții pentru lista de coordonate - lista simplu înlănțuită ordonată
void adaugaCoordInLista(Coord** lista, int x, int y)
{
    Coord* noua = (Coord*)malloc(sizeof(Coord));
    noua->x = x;
    noua->y = y;
    noua->next = NULL;

    if (*lista == NULL)
    {
        *lista = noua;
        return;
    }

    // Inserare ordonată după linie, apoi coloană
    if (x < (*lista)->x || (x == (*lista)->x && y < (*lista)->y))
    {
        noua->next = *lista;
        *lista = noua;
        return;
    }

    Coord* curent = *lista;
    while (curent->next != NULL &&
           (curent->next->x < x || (curent->next->x == x && curent->next->y < y)))
    {
        curent = curent->next;
    }

    noua->next = curent->next;
    curent->next = noua;
}

void stergeListaCoord(Coord** lista)
{
    while (*lista != NULL)
    {
        Coord* temp = *lista;
        *lista = (*lista)->next;
        free(temp);
    }
}

// Funcție pentru a stoca pointer-ul la lista de coordonate in stiva
void pushCoordList(Nod** stiva, Coord* lista)
{
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->val.ptr_val = lista;  // Stocăm direct pointerul
    nodNou->val.i_val = 0;
    nodNou->val.c_val = 0;
    nodNou->next = *stiva;
    *stiva = nodNou;
}

// Construiește lista simplu înlănțuită cu coordonatele schimbărilor
Coord* construiesteListaSchimbari(int N, int M, const char matrice[][101], const char copie[][101])
{
    Coord* lista = NULL;

    for (int x = 1; x <= N; x++)
    {
        for (int y = 1; y <= M; y++)
        {
            if (copie[x][y] != matrice[x][y])
            {
                adaugaCoordInLista(&lista, x, y);
            }
        }
    }

    return lista;
}

void afiseazaStivaGeneratii(Nod** stiva, int K, FILE* output_file)
{
    // Folosim o stivă auxiliară pentru a păstra ordinea LIFO și a afișa corect
    Nod* stivaAux = NULL;

    // Mutăm tot din stiva principală în stiva auxiliară (inversăm ordinea)
    while (!esteGoala(*stiva))
    {
        Data coord_data = pop(stiva);
        Nod* nodNou = (Nod*)malloc(sizeof(Nod));
        nodNou->val = coord_data;
        nodNou->next = stivaAux;
        stivaAux = nodNou;
    }

    // Acum afișăm din stiva auxiliară în ordinea corectă (1, 2, ..., K)
    int generatie = 1;
    while (!esteGoala(stivaAux) && generatie <= K)
    {
        Data coord_data = pop(&stivaAux);
        Coord* lista = (Coord*)coord_data.ptr_val;  // Folosim ptr_val

        fprintf(output_file, "%d", generatie);

        Coord* coord = lista;
        while (coord != NULL)
        {
            fprintf(output_file, " %d %d", coord->x - 1, coord->y - 1);
            coord = coord->next;
        }

        fprintf(output_file, "\n");

        // Eliberăm memoria listei de coordonate
        stergeListaCoord(&lista);
        generatie++;
    }

    // Curățăm stiva auxiliară dacă mai sunt elemente
    stergeStiva(&stivaAux);
}

void stocheazaSchimbariInStiva(int N, int M, const char matrice[][101], const char copie[][101], Nod** stiva) {
    Coord* lista = construiesteListaSchimbari(N, M, matrice, copie);
    pushCoordList(stiva, lista);
}

// Funcție pentru curățarea completă a stivei cu eliberarea listelor de coordonate
void stergeStivaCuListeCoord(Nod** stiva) {
    while (*stiva != NULL) {
        Data coord_data = pop(stiva);
        Coord* lista = (Coord*)coord_data.ptr_val;
        stergeListaCoord(&lista);
    }
}

// Task 4 - Functii

// Construieste graful din matrice
Graph* creeazaGrafDinMatrice(const char matrice[][101], int rows, int cols) {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->rows = rows;
    g->cols = cols;

    // Aloca spatiu pentru maparea nodurilor
    g->hartaNoduri = (int**)malloc((rows + 1) * sizeof(int*));
    for(int i = 0; i <= rows; i++) {
        g->hartaNoduri[i] = (int*)malloc((cols + 1) * sizeof(int));
        for(int j = 0; j <= cols; j++) {
            g->hartaNoduri[i][j] = -1;
        }
    }

    // Prima trecere: numara nodurile valide (X) si le asigneaza numere
    int numarNoduri = 0;
    for(int i = 1; i <= rows; i++) {
        for(int j = 1; j <= cols; j++) {
            if(matrice[i][j] == 'X') {
                g->hartaNoduri[i][j] = numarNoduri++;
            }
        }
    }

    g->V = numarNoduri;
    
    if(g->V == 0) {
        g->E = 0;
        g->nodLaLinie = NULL;
        g->nodLaColoana = NULL;
        g->a = NULL;
        return g;
    }

    // Aloca spatiu pentru maparea inversa
    g->nodLaLinie = (int*)malloc(g->V * sizeof(int));
    g->nodLaColoana = (int*)malloc(g->V * sizeof(int));

    // Construieste maparea inversa
    for(int i = 1; i <= rows; i++) {
        for(int j = 1; j <= cols; j++) {
            if(matrice[i][j] == 'X') {
                int numarNod = g->hartaNoduri[i][j];
                g->nodLaLinie[numarNod] = i;
                g->nodLaColoana[numarNod] = j;
            }
        }
    }

    // Aloca matricea de adiacenta
    g->a = (int**)malloc(g->V * sizeof(int*));
    for(int i = 0; i < g->V; i++) {
        g->a[i] = (int*)calloc(g->V, sizeof(int));
    }

    // A doua trecere: construieste muchiile
    g->E = 0;
    for(int i = 1; i <= rows; i++) {
        for(int j = 1; j <= cols; j++) {
            if(matrice[i][j] == 'X') {
                int nodCurent = g->hartaNoduri[i][j];

                // Verifica toti cei 8 vecini
                for(int d = 0; d < 8; d++) {
                    int liniaNova = i + directiiX[d];
                    int coloanaNoua = j + directiiY[d];

                    if(liniaNova >= 1 && liniaNova <= rows && coloanaNoua >= 1 && coloanaNoua <= cols && 
                       matrice[liniaNova][coloanaNoua] == 'X') {
                        int nodVecin = g->hartaNoduri[liniaNova][coloanaNoua];
                        if(nodVecin >= 0 && nodVecin < g->V && nodCurent != nodVecin && g->a[nodCurent][nodVecin] == 0) {
                            g->a[nodCurent][nodVecin] = 1;
                            g->a[nodVecin][nodCurent] = 1;
                            g->E++;
                        }
                    }
                }
            }
        }
    }

    g->E /= 2;
    return g;
}

// Functia DFS_scan pentru parcurgerea unei componente conexe
void scanareDFS(Graph *g, int vizitat[], int i, int componenta[], int *dimensiune) {
    vizitat[i] = 1;
    componenta[*dimensiune] = i;
    (*dimensiune)++;

    for(int j = 0; j < g->V; j++) {
        if(g->a[i][j] == 1 && vizitat[j] == 0) {
            scanareDFS(g, vizitat, j, componenta, dimensiune);
        }
    }
}

// Functia principala DFS care gaseste toate componentele conexe
int** parcurgereDFS(Graph *g, int **dimensiuniComponente, int *numarComponente) {
    int componenteConexe = 0;
    int *vizitat = (int*)calloc(g->V, sizeof(int));

    int **componente = (int**)malloc(g->V * sizeof(int*));
    *dimensiuniComponente = (int*)calloc(g->V, sizeof(int));

    for(int i = 0; i < g->V; i++) {
        componente[i] = (int*)malloc(g->V * sizeof(int));
    }

    for(int i = 0; i < g->V; i++) {
        if(vizitat[i] == 0) {
            scanareDFS(g, vizitat, i, componente[componenteConexe], &(*dimensiuniComponente)[componenteConexe]);
            componenteConexe++;
        }
    }

    free(vizitat);
    *numarComponente = componenteConexe;
    return componente;
}

// Functie pentru compararea a doi noduri dupa coordonate (linie, coloana)
int comparaNoduri(Graph *g, int nod1, int nod2) {
    int linie1 = g->nodLaLinie[nod1];
    int coloana1 = g->nodLaColoana[nod1];
    int linie2 = g->nodLaLinie[nod2];
    int coloana2 = g->nodLaColoana[nod2];

    if(linie1 != linie2) return linie1 - linie2;
    return coloana1 - coloana2;
}

// Functie pentru sortarea nodurilor dupa coordonate
void sorteazaNoduriDupaCoordonate(Graph *g, int noduri[], int dimensiune) {
    for(int i = 0; i < dimensiune - 1; i++) {
        for(int j = 0; j < dimensiune - 1 - i; j++) {
            if(comparaNoduri(g, noduri[j], noduri[j+1]) > 0) {
                int temp = noduri[j];
                noduri[j] = noduri[j+1];
                noduri[j+1] = temp;
            }
        }
    }
}

// Functie recursiva pentru gasirea lantului Hamiltonian cu backtracking
int gasesteCalHamiltonian(Graph *g, int componenta[], int dimensiuneComp, int cale[], int pozitie, int folosit[]) {
    if(pozitie == dimensiuneComp) {
        return 1;
    }

    for(int i = 0; i < dimensiuneComp; i++) {
        int nodCurent = componenta[i];
        
        if(!folosit[nodCurent]) {
            if(pozitie == 0 || g->a[cale[pozitie-1]][nodCurent] == 1) {
                cale[pozitie] = nodCurent;
                folosit[nodCurent] = 1;

                if(gasesteCalHamiltonian(g, componenta, dimensiuneComp, cale, pozitie + 1, folosit)) {
                    return 1;
                }

                folosit[nodCurent] = 0;
            }
        }
    }

    return 0;
}

// Functie pentru gasirea unui lant Hamiltonian dintr-o componenta
int obtineCalaleHamiltoniana(Graph *g, const int componenta[], int dimensiuneComp, int caleRezultat[]) {
    if(dimensiuneComp <= 1) {
        if(dimensiuneComp == 1) {
            caleRezultat[0] = componenta[0];
        }
        return dimensiuneComp;
    }

    // Sorteaza nodurile din componenta dupa coordonate
    int *componentaSortata = (int*)malloc(dimensiuneComp * sizeof(int));
    for(int i = 0; i < dimensiuneComp; i++) {
        componentaSortata[i] = componenta[i];
    }
    sorteazaNoduriDupaCoordonate(g, componentaSortata, dimensiuneComp);

    int *cale = (int*)malloc(dimensiuneComp * sizeof(int));
    int *folosit = (int*)calloc(g->V, sizeof(int));

    // Incearca sa gaseasca o cale Hamiltoniasn incepand cu fiecare nod
    for(int start = 0; start < dimensiuneComp; start++) {
        // Reseteaza vectorul folosit
        for(int i = 0; i < g->V; i++) {
            folosit[i] = 0;
        }

        int nodPlecare = componentaSortata[start];
        cale[0] = nodPlecare;
        folosit[nodPlecare] = 1;

        if(gasesteCalHamiltonian(g, componentaSortata, dimensiuneComp, cale, 1, folosit)) {
            for(int i = 0; i < dimensiuneComp; i++) {
                caleRezultat[i] = cale[i];
            }
            
            free(componentaSortata);
            free(cale);
            free(folosit);
            return dimensiuneComp;
        }
    }

    free(componentaSortata);
    free(cale);
    free(folosit);
    return -1;
}

void parcurgerePreArboreGraf(FILE *output, Node *rad, int n, int m) {
    if (rad == NULL) return;
    
    char matrice[101][101];
    listaLaMatrice(rad->val, n, m, matrice);

    Graph* g = creeazaGrafDinMatrice(matrice, n, m);

    if(g->V == 0) {
        fprintf(output, "-1\n");
    } else {
        int *dimensiuniComponente;
        int numarComponente;
        int **componente = parcurgereDFS(g, &dimensiuniComponente, &numarComponente);

        int lungimeMaxima = -1;
        int *ceaMaiBunaCaleeGlobala = NULL;

        // Gaseste cea mai mare componenta conexa care are lant Hamiltonian
        for(int i = 0; i < numarComponente; i++) {
            int *caleaCurenta = (int*)malloc(dimensiuniComponente[i] * sizeof(int));
            int lungimeaCurenta = obtineCalaleHamiltoniana(g, componente[i], dimensiuniComponente[i], caleaCurenta);

            if(lungimeaCurenta == dimensiuniComponente[i] && lungimeaCurenta > lungimeMaxima) {
                lungimeMaxima = lungimeaCurenta;
                if(ceaMaiBunaCaleeGlobala != NULL) free(ceaMaiBunaCaleeGlobala);
                
                ceaMaiBunaCaleeGlobala = (int*)malloc(lungimeMaxima * sizeof(int));
                for(int j = 0; j < lungimeMaxima; j++) {
                    ceaMaiBunaCaleeGlobala[j] = caleaCurenta[j];
                }
            }
            free(caleaCurenta);
        }

        if(lungimeMaxima > 0 && ceaMaiBunaCaleeGlobala != NULL) {
            fprintf(output, "%d\n", lungimeMaxima - 1);
            for(int i = 0; i < lungimeMaxima; i++) {
                int nod = ceaMaiBunaCaleeGlobala[i];
                // Convert to 0-based indexing for output
                int linie = g->nodLaLinie[nod] - 1;
                int coloana = g->nodLaColoana[nod] - 1;
                fprintf(output, "(%d,%d)", linie, coloana);
                if(i < lungimeMaxima - 1) fprintf(output, " ");
            }
            fprintf(output, "\n");
        } else {
            fprintf(output, "-1\n");
        }

        if(ceaMaiBunaCaleeGlobala != NULL) free(ceaMaiBunaCaleeGlobala);
        for(int i = 0; i < numarComponente; i++) free(componente[i]);
        free(componente);
        free(dimensiuniComponente);
    }

    elibereazaGraf(g);

    parcurgerePreArboreGraf(output, rad->stanga, n, m);
    parcurgerePreArboreGraf(output, rad->dreapta, n, m);
}

// Functie pentru eliberarea memoriei
void elibereazaGraf(Graph *g) {
    if(g == NULL) return;
    
    if(g->a != NULL) {
        for(int i = 0; i < g->V; i++) {
            if(g->a[i] != NULL) {
                free(g->a[i]);
            }
        }
        free(g->a);
    }

    if(g->hartaNoduri != NULL) {
        for(int i = 0; i <= g->rows; i++) {
            if(g->hartaNoduri[i] != NULL) {
                free(g->hartaNoduri[i]);
            }
        }
        free(g->hartaNoduri);
    }

    if(g->nodLaLinie != NULL) {
        free(g->nodLaLinie);
    }
    
    if(g->nodLaColoana != NULL) {
        free(g->nodLaColoana);
    }

    free(g);
}


int main(int argc, const char* argv[])
{
    for (int i = 1; i < argc; i++) //Pentru fiecare dintre fiserele input/output!
    {
        FILE *input_file = fopen(argv[i], "r");

        char OutputFile[30];
        strcpy(OutputFile + 1, argv[i]);
        OutputFile[0] = 'O';
        OutputFile[1] = 'u';
        OutputFile[2] = 't';
        int len = strlen(OutputFile);
        OutputFile[len - 2] = 'o';
        OutputFile[len - 1] = 'u';
        OutputFile[len] = 't';
        OutputFile[len + 1] = '\0';

        FILE *output_file = fopen(OutputFile, "w");

        if (input_file == NULL || output_file == NULL)
        {
            printf("File could not open");
            if (input_file == NULL)
                fclose(output_file);
            else if (output_file == NULL)
                fclose(input_file);
            return 1;
        }

        int T, N, M, K; //Iitializare si citire variabile!
        fscanf(input_file, "%d", &T);

        if (T == 1)
        {
            fscanf(input_file, "%d %d %d", &N, &M, &K); //Citire dimensiuni si numar de generatii!

            char matrice[101][101];
            for (int x = 0; x <= N + 1; x++)
                for (int y = 0; y <= M + 1; y++)
                    if (x == 0 || y == 0 || x == N + 1 || y == M + 1) matrice[x][y] = '+';
                      else matrice[x][y] = ' '; //Construirea conturului matrice pentru vecini!

            for (int x = 1; x <= N; x++)
                for (int y = 1; y <= M; y++)
                    fscanf(input_file, " %c", &matrice[x][y]); //Citire matrice!

            for (int p = 0; p <= K; p++)
            {
                for (int x = 1; x <= N; x++)
                {
                    for (int y = 1; y <= M; y++)
                        fprintf(output_file, "%c", matrice[x][y]);
                    fprintf(output_file, "\n");
                } //Scriere matrice curenta!

                if (p != K) fprintf(output_file, "\n"); //Punem spatiul aferent!

                if (p < K)
                {
                    char copie[101][101];
                    for (int x = 1; x <= N; x++)
                    {
                        for (int y = 1; y <= M; y++)
                        {
                            int nrVecini = 0;
                            for (int di = -1; di <= 1; di++)
                                for (int dj = -1; dj <= 1; dj++)
                                    if (matrice[x + di][y + dj] == 'X') nrVecini++;
                            if (matrice[x][y] == 'X') nrVecini--; //Numarare vecini pentru fiecare celula!

                            if (matrice[x][y] == '+')
                            {
                              if (nrVecini == 3) copie[x][y] = 'X';
                              else copie[x][y] = '+';
                            }
                            else
                            {
                                if (nrVecini == 2 || nrVecini == 3) copie[x][y] = 'X';
                                else copie[x][y] = '+';
                            } //Implementarea regulilor jocului!
                        }
                    }
                    for (int x = 1; x <= N; x++)
                        for (int y = 1; y <= M; y++)
                            matrice[x][y] = copie[x][y]; //Noua matrice a fost creata!
                }
              if (p==K) fprintf(output_file, "\n"); //Adaugarea spatiului de final!
            }
        }
        else if (T == 2)
        {
            if (fscanf(input_file, "%d %d %d", &N, &M, &K) != 3) {
                printf("Error reading N, M, K from file: %s\n", argv[i]);
                fclose(input_file);
                fclose(output_file);
                continue;
            }

            char matrice[101][101];
            char copie[101][101];

            // Initializare matrice
            initializeazaMatrice(N, M, matrice);

            // Citire matrice
            citesteMacriceInitiala(N, M, matrice, input_file);

            // O SINGURĂ stivă pentru toate generațiile
            Nod* stivaGeneratii = NULL;

            // Generează K generații și stochează în stivă
            for (int k = 1; k <= K; k++)
            {
                calculeazaGeneratieUrmatoare(N, M, matrice, copie);
                stocheazaSchimbariInStiva(N, M, matrice, copie, &stivaGeneratii);
                actualizeazaMatrice(N, M, matrice, copie);
            }

            // Afișează folosind funcția care inversează ordinea corect
            afiseazaStivaGeneratii(&stivaGeneratii, K, output_file);
            
            // Curățăm memoria rămasă (dacă există)
            stergeStivaCuListeCoord(&stivaGeneratii);
        }
        else if (T == 3)
        {
            fscanf(input_file, "%d %d %d", &N, &M, &K);

            Node* rad = creazaNod();
            char a[101];
            for (int e = 1; e <= N; e++)
            {
                fscanf(input_file, "%100s", a);
                for (int j = 1; j <= M; j++)
                {
                    if (a[j - 1] == 'X')
                    {
                        Celula *noua = (Celula*)malloc(sizeof(Celula));
                        noua->x = e;
                        noua->y = j;
                        noua->urm = rad->val;
                        rad->val = noua;
                    }
                }
            }

            construiesteArbore(rad, 0, K, N, M);
            parcurgerePreordine(output_file, rad, N, M);
            elibereazaArbore(rad);
        }
        else if (T == 4)
        {
            fscanf(input_file, "%d %d %d", &N, &M, &K);

            Node* rad = creazaNod();
            char a[101];
            for (int e = 1; e <= N; e++)
            {
                fscanf(input_file, "%100s", a);
                for (int j = 1; j <= M; j++)
                {
                    if (a[j - 1] == 'X')
                    {
                        Celula *noua = (Celula*)malloc(sizeof(Celula));
                        noua->x = e;
                        noua->y = j;
                        noua->urm = rad->val;
                        rad->val = noua;
                    }
                }
            }

            construiesteArbore(rad, 0, K, N, M);

            parcurgerePreArboreGraf(output_file, rad, N, M);

            elibereazaArbore(rad);
        }
        fclose(output_file);
        fclose(input_file);
    }
    return 0;
}

