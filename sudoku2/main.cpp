#include <cstdio>
using namespace std;

/* kazde policko patri do 3 skupin: riadok, stlpec a stvorec */
const int SKUPIN = 3;

/* udaje o jednom policku plochy: suradnice,
 * hodnota (0 ak prazdne, alebo 1..9),
 * a zoznam skupin, do ktorych patri. */
struct policko
{
    int riadok, stlpec;
    int hodnota;
    int skupiny[SKUPIN];
};

struct sudoku
{
    policko *plocha;  /* pole vsetkych policok plochy */
    bool **obsadene;  /* pre kazdy skupiny ci je dana cifra uz pouzita */
    int pocetPolicok; /* celkovy pocet policok (9*9) */
    int pocetSkupin;  /* celkovy pocet skupin (3*9) */
    int rieseni;      /* pocet najdenych rieseni */
};

void vypis(FILE *f, sudoku &s)
{
    /* vypis riesenie sudoku a zvys pocitadlo rieseni */
    s.rieseni++;
    int pozicia = 0;
    for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                fprintf(f," %d",s.plocha[pozicia].hodnota);
                pozicia++;
            }
            fprintf(f,"\n");
        }
    fprintf(f,"\n");
}

int najdiVolne(sudoku &s)
{
    /* najdi volne policko na ploche */
    for (int i = 0; i < s.pocetPolicok; i++)
        {
            if (s.plocha[i].hodnota == 0)
                return i;
        }
    return -1;
}

bool moze(sudoku &s, int pozicia, int hodnota)
{
    /* Mozeme ulozit danu hodnotu na policko s poradovym cislom pozicia?
     * Da sa to, ak ziadna zo skupin tohto policka nema
     * tuto hodnotu uz pouzitu. */
    for (int i = 0; i < SKUPIN; i++)
        {
            /* cislo i-tej skupiny pre policko */
            int skupina = s.plocha[pozicia].skupiny[i];
            /* ak je uz hodnota obsadena, neda sa pouzit */
            if (s.obsadene[skupina][hodnota])
                return false;
        }
    /* vo vsetkych skupinach je hdonota neobsadena */
    return true;
}

void uloz(sudoku &s, int pozicia, int hodnota)
{
    /* na policko s poradovym cislom pozicia uloz danu
     * hodnotu. Ak je hodnota > 0, zaregistruj ju
     * tiez ako obsadenu vo vsetkych skupinach,
     * kam policko patri. */
    s.plocha[pozicia].hodnota = hodnota;
    if (hodnota > 0)
        {
            for (int i = 0; i < SKUPIN; i++)
            {
                int skupina = s.plocha[pozicia].skupiny[i];
                s.obsadene[skupina][hodnota] = true;
            }
        }
}

void zmaz(sudoku &s, int pozicia)
{
    /* hodnotu v policku s pozicia zmen na nulu
     * a povodnu hodnotu odznac v poliach obsadene pre vsetky
     * skupiny tohto policka */
    int hodnota = s.plocha[pozicia].hodnota;
    s.plocha[pozicia].hodnota = 0;
    for (int i = 0; i < SKUPIN; i++)
        {
            int skupina = s.plocha[pozicia].skupiny[i];
            s.obsadene[skupina][hodnota] = false;
        }
}

void generuj(FILE *f,sudoku &s)
{
    /* mame ciastocne vyplnenu plochu sudoku,
     * chceme najst vsetky moznosti, ako ho dovyplnat. */
    int i = najdiVolne(s);
    if (i < 0)
        {
            vypis(f,s);
        }
    else
        {
            for (int x = 1; x <= 9; x++)
            {
                if (moze(s, i, x))
                {
                    uloz(s, i, x);
                    generuj(f,s);
                    zmaz(s, i);
                }
            }
        }
}

void inicializuj(sudoku &s, int **a)
{
    /* inicializuj strukturu sudoku na zaklade
     * vstupnej matice s cislami 0..9*/
    s.pocetPolicok = 9 * 9;
    s.pocetSkupin = 3 * 9;
    /* alokujeme polia a oznacime cifry ako neobsadene */
    s.plocha = new policko[s.pocetPolicok];
    s.obsadene = new bool*[s.pocetSkupin];
    for (int i = 0; i < s.pocetSkupin; i++)
        {
            s.obsadene[i] = new bool[10];
            for (int j = 1; j <= 9; j++)
            {
                s.obsadene[i][j] = false;
            }
        }
    /* pre kazde policko naplnime jeho strukturu
     * a vyplnime obsadene cifry */
    int pozicia = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            s.plocha[pozicia].riadok = i;
            s.plocha[pozicia].stlpec = j;
            s.plocha[pozicia].skupiny[0] = i;
            s.plocha[pozicia].skupiny[1] = 9 + j;
            s.plocha[pozicia].skupiny[2] = (i / 3)*3 + (j / 3) + 18;
            uloz(s, pozicia, a[i][j]);
            pozicia++;
        }
    }
    /* este sme nenasli ziadne riesenie */
    s.rieseni = 0;
}

void uvolni(sudoku &s)
{
    /* alokujeme polia a oznacime cifry ako neobsadene */
    for (int i = 0; i < s.pocetSkupin; i++)
        {
            delete[] s.obsadene[i];
        }
    delete[] s.obsadene;

    delete[] s.plocha;
}

int main(void)
{
    /* alokujeme a nacitame 2D maticu so vstupom */
    int **a = new int *[9];
    for (int i = 0; i < 9; i++)
        {
            a[i] = new int[9];
        }

    FILE *f;
    f=fopen("vstup.txt","r");
    if (!f) return -1;

    for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                fscanf(f,"%d ",&a[i][j]);
            }
        }
    fclose(f);

    /* vytvorime struktury pre sudoku */
    sudoku s;
    inicializuj(s, a);

    f=fopen("vystup.txt","w");
    if (!f) return -1;

    /* rekurzivne prehladavanie s navratom */
    generuj(f,s);

    fclose(f);
    /* vypis riesenia */
    printf("Pocet rieseni: %d\n",s.rieseni);

    /* este by sme mali odalokovat polia v sudoku */
    for (int i = 0; i < 9; i++)
        {
            delete[] a[i];
        }
    delete[] a;
    uvolni(s);

}
