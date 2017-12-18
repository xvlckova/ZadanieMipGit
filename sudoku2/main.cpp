#include <cstdio>

int pocetRieseni = 0;  // globalne pocitadlo rieseni
int pocetVolani = 0;   // globalne pocitadlo volani funkcie generuj

void vypis(int **a)
{
//vypis riesenie sudoku a zvys pocitadlo rieseni
    pocetRieseni++;
    for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                printf(" %d", a[i][j]);
            }
            printf("\n");
        }
    printf("\n");
}

bool moze(int **a, int riadok, int stlpec, int hodnota)
 {
//Mozeme ulozit danu hodnotu na dane policko? Da sa to, ak riadok, stlpec, ani stvorec nema tuto hodnotu este pouzitu.
    for (int i = 0; i < 9; i++)
        {
            if (a[riadok][i] == hodnota)
                return false;
            if (a[i][stlpec] == hodnota)
                return false;
        }
//lavy horny roh stvorca
    int riadok1 = riadok - riadok % 3;
    int stlpec1 = stlpec - stlpec % 3;
    for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (a[riadok1 + i][stlpec1 + j] == hodnota)
                    return false;
            }
        }
    return true;
}

int kolkoMoze(int **a, int r, int s)
{
//kolko moznych hodnot x mozeme ulozit na policko a[r][s] ?
    int pocet = 0;
    for (int x = 1; x <= 9; x++)
        {
            if (moze(a, r, s, x))
            {
                pocet++;
            }
        }
    return pocet;
}

void najdiVolne(int **a, int &r, int &s)
{
//najdi volne policko na ploche. Vyberame take, ktore ma najmenej moznych hodnot, ktore sa na neho daju polozit.
    int min = 10;
    r = s = -1;
    for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (a[i][j] == 0)
                {
                    int pocet = kolkoMoze(a, i, j);
                    if (pocet < min)
                    {
                        min = pocet;
                        r = i;
                        s = j;
                    }
                }
            }
        }
    return;
}

void generuj(int **a)
{
//rekurzivne doplnaj vsetky moznosti za volne policka na ploche a

    pocetVolani++;  // zvys pocitadlo volani
    int r, s;       // skus najst volne policko na ploche
    najdiVolne(a, r, s);
    if (r < 0)
    // ak volne policko nie je, vypis riesenie
        {
            vypis(a);
        }
    else
    // ak mame volne policko, skus dosadit vsetky hodnoty
        {
            for (int x = 1; x <= 9; x++)
            {
                if (moze(a, r, s, x))
                {
                    a[r][s] = x;
                    generuj(a);    // po dosadeni volaj rekurziu
                    a[r][s] = 0;   // po navrate z rekurzie uprac
                }
            }
        }
}

int main(void)
{
    //alokujeme a nacitame 2D maticu so vstupom
    int **a = new int *[9];
    for (int i = 0; i < 9; i++)
        {
            a[i] = new int[9];
        }

    for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                scanf("%d ", &a[i][j]);
            }
        }

    //rekurzivne prehladavanie s navratom
    generuj(a);
    printf("Pocet rieseni %d, pocet volani %d\n", pocetRieseni, pocetVolani); //vypis riesenia

    //odalokovanie poli v sudoku
    for (int i = 0; i < 9; i++)
        {
            delete[] a[i];
        }
    delete[] a;
}
