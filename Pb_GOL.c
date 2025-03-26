#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char* argv[]) 
{
    int i, q;
    for (i = 1; i < argc; i++) //Pentru fiecare dintre fiserele input/output!
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

        for (q = 1; q <= T; q++)
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
        fclose(output_file);
        fclose(input_file);
    }
    return 0;
}

