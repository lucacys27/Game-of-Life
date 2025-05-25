#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

// TASK 1 - Structuri de baza
typedef struct celula
{
    int x, y;
    struct celula *urm;
} Celula;

// TASK 1 - Functii
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

// TASK 2 - Structuri pentru stiva
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

// Structura pentru coordonate - lista simplu inlantuita (Task 2)
typedef struct Coord
{
    int x, y;
    struct Coord* next;
} Coord;

// TASK 2 - Functii pentru stiva
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

// TASK 2 - Functii pentru lista de coordonate
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

    // Inserare ordonata dupa linie, apoi coloana
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

void pushCoordList(Nod** stiva, Coord* lista)
{
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->val.ptr_val = lista;  // Stocam direct pointerul
    nodNou->val.i_val = 0;
    nodNou->val.c_val = 0;
    nodNou->next = *stiva;
    *stiva = nodNou;
}

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

void stocheazaSchimbariInStiva(int N, int M, const char matrice[][101], const char copie[][101], Nod** stiva) {
    Coord* lista = construiesteListaSchimbari(N, M, matrice, copie);
    pushCoordList(stiva, lista);
}

void afiseazaStivaGeneratii(Nod** stiva, int K, FILE* output_file)
{
    // Folosim o stiva auxiliara pentru a pastra ordinea LIFO si a afisa corect
    Nod* stivaAux = NULL;

    // Mutam tot din stiva principala in stiva auxiliara (inversam ordinea)
    while (!esteGoala(*stiva))
    {
        Data coord_data = pop(stiva);
        Nod* nodNou = (Nod*)malloc(sizeof(Nod));
        nodNou->val = coord_data;
        nodNou->next = stivaAux;
        stivaAux = nodNou;
    }

    // Acum afisam din stiva auxiliara in ordinea corecta (1, 2, ..., K)
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

        // Eliberam memoria listei de coordonate
        stergeListaCoord(&lista);
        generatie++;
    }

    // Curatam stiva auxiliara daca mai sunt elemente
    stergeStiva(&stivaAux);
}

void stergeStivaCuListeCoord(Nod** stiva) {
    while (*stiva != NULL) {
        Data coord_data = pop(stiva);
        Coord* lista = (Coord*)coord_data.ptr_val;
        stergeListaCoord(&lista);
    }
}

// MAIN FUNCTION
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

            // O SINGURA stiva pentru toate generatiile
            Nod* stivaGeneratii = NULL;

            // Genereaza K generatii si stocheaza in stiva
            for (int k = 1; k <= K; k++)
            {
                calculeazaGeneratieUrmatoare(N, M, matrice, copie);
                stocheazaSchimbariInStiva(N, M, matrice, copie, &stivaGeneratii);
                actualizeazaMatrice(N, M, matrice, copie);
            }

            // Afiseaza folosind functia care inverseaza ordinea corect
            afiseazaStivaGeneratii(&stivaGeneratii, K, output_file);
            
            // Curatam memoria ramasa (daca exista)
            stergeStivaCuListeCoord(&stivaGeneratii);
        }
        
        fclose(output_file);
        fclose(input_file);
    }
    return 0;
}

