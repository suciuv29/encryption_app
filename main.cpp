#include <graphics.h> //-lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
#include <string.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
using namespace std;

int lungime_ecran=getmaxwidth(),inaltime_ecran=getmaxheight();

//Toate meniurile au 4 butoane. pentru eventuale adaugari se modifica functia.
void afisare_4_butoane_meniu(char *text_1,char *text_2,char *text_3,char *text_4);

//din acelasi motiv se foloseste o singura functie de identificare buton
int gaseste_buton_meniu(int x,int y);

//la editarea fisierelor de descriere sa nu fie mai mult de 65 caractere pe linie
//layoutul pentru descriere este similar cu celelalte meniuri si se foloseste functia
//gaseste_buton_meniu. trebuie avut grija cnad se modifica!
void citeste_descriere(char *nume_fisier,char *titlu_window)
{
    int window=initwindow(lungime_ecran,inaltime_ecran,titlu_window);

    setfillstyle(1,GREEN);
    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(BLACK);
    setbkcolor(GREEN);
    readimagefile("matrix-background-dark.jpg",0,0,lungime_ecran,inaltime_ecran);
    bar(lungime_ecran/4,7*inaltime_ecran/9,3*lungime_ecran/4,8*inaltime_ecran/9);
    outtextxy(lungime_ecran/2,15*inaltime_ecran/18,"Inapoi");

    setfillstyle(1,BLACK);
    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(GREEN);
    setbkcolor(BLACK);
    bar(lungime_ecran/5,1*inaltime_ecran/9,4*lungime_ecran/5,6*inaltime_ecran/9);

    ifstream in(nume_fisier);
    char linie_citita[70];
    int i=0;
    while (!in.eof())
    {
        in.getline(linie_citita,70);
        outtextxy(lungime_ecran/2,3*inaltime_ecran/18+i*20,linie_citita);
        i++;
    }
    in.close();
    int iesire_meniu=0,coord_mouse_x,coord_mouse_y,buton_apasat;
    while (!iesire_meniu)
    {
        while (!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, coord_mouse_x,coord_mouse_y);
        buton_apasat=gaseste_buton_meniu(coord_mouse_x,coord_mouse_y);
        if (buton_apasat==4)
            iesire_meniu=1;
    }
    closegraph(window);
}

void goleste_buffer_taste_mouse()
{
    //elimina din buffer tastele si clickurile apasate in plus
    while (kbhit())
        getch();
    while (ismouseclick(WM_LBUTTONDOWN))
        clearmouseclick(WM_LBUTTONDOWN);
}

void cripteaza_fisier_XOR(char* cheie)
{
    int numar=0,lungime_cheie=strlen(cheie);
    for (int i=0;i<lungime_cheie;i++)//formam cheia ca si numar
    {
        numar=numar*10+cheie[i]-48;
    }
    ifstream in("mesaj.in");
    ofstream out("mesaj.out");
    char caracter_citit,caracter_nou;
    caracter_citit=in.get();
    while (!in.eof())
    {
        caracter_nou=caracter_citit^numar;
        out.put(caracter_nou);
        caracter_citit=in.get();
    }
    in.close();
    out.close();
}

void cripteaza_fisier_caesar(char *cheie)
{
    int numar=0,lungime_cheie=strlen(cheie);
    for (int i=0;i<lungime_cheie;i++)//formam cheia ca si numar
    {
        numar=numar*10+cheie[i]-48;
    }
    ifstream in("mesaj.in");
    ofstream out("mesaj.out");
    char caracter_citit,caracter_nou;
    caracter_citit=in.get();
    while (!in.eof())
    {
        if (isalpha(caracter_citit))//criptam doar literele
        {
            caracter_nou=caracter_citit+numar;
            //verificam cazurile cand se trece de litera Z in functie de litera mare sau mica
            if (isupper(caracter_citit))
            {
                caracter_nou-=64;
                if (caracter_nou>26)
                    caracter_nou-=26;
                caracter_nou+=64;
            }
            else
            {
                caracter_nou-=96;
                if (caracter_nou>26)
                    caracter_nou-=26;
                caracter_nou+=96;
            }
        }
        else
        {
            caracter_nou=caracter_citit;
        }
        out.put(caracter_nou);
        caracter_citit=in.get();
    }
    in.close();
    out.close();
}

void cripteaza_fisier_OTP(char *cheie)
{
    int lungime_cheie=strlen(cheie);
    int pozitie_curenta_cheie=0,reseteaza_cuvant=0;//reseteaza cuvant devine 1 la intalnirea unui caracter ce nu e litera si 0 cand e litera
    int valori_litere_cheie[11];
    for (int i=0;i<lungime_cheie;i++)//ne aflam valorile fiecarei litere din cheie in functie de litera mare sau mica
    {
        if (isupper(cheie[i]))
            valori_litere_cheie[i]=cheie[i]-64;
        else
            valori_litere_cheie[i]=cheie[i]-96;
    }
    ifstream in("mesaj.in");
    ofstream out("mesaj.out");
    char caracter_citit,caracter_nou;
    caracter_citit=in.get();
    while (!in.eof())
    {
        if (isalpha(caracter_citit))//prelucram daca avem litera
        {
            if (reseteaza_cuvant==1)//verificam daca litera curenta este inceput de cuvant
            {
                reseteaza_cuvant=0;
                pozitie_curenta_cheie=0;
            }
            if (pozitie_curenta_cheie==lungime_cheie)//verificam daca am ajuns la sfarsitul cheii
            {
                pozitie_curenta_cheie=0;
            }
            caracter_nou=caracter_citit+valori_litere_cheie[pozitie_curenta_cheie];
            pozitie_curenta_cheie++;
            //verificam cazurile cand se trece de litera Z in functie de litera mare sau mica
            if (isupper(caracter_citit))
            {
                caracter_nou-=64;
                if (caracter_nou>26)
                    caracter_nou-=26;
                caracter_nou+=64;
            }
            else
            {
                caracter_nou-=96;
                if (caracter_nou>26)
                    caracter_nou-=26;
                caracter_nou+=96;
            }
        }
        else
        {
            caracter_nou=caracter_citit;
            reseteaza_cuvant=1;
        }
        out.put(caracter_nou);
        caracter_citit=in.get();
    }
    in.close();
    out.close();
}

int verifica_cheie_XOR(char *cheie,int lungime_cheie)//cheia trebuie sa fie un numar intre 0 si 255
{
    int numar=0;
    if (lungime_cheie==0 || lungime_cheie>3)//eliminam cheia goala(dupa ce se sterge ceva introdus; eliminam cheia prea lunga
        return 0;
    if (cheie[0]==48 && lungime_cheie>1)//eliminam numere ce au inaintea lor cifra 0(chiea 0 este cheie valida)
        return 0;
    for (int i=0;i<lungime_cheie;i++)//verificam sa se contina numai cifre. in acelasi timp si convertim numarul daca e posibil
    {
        if (!(cheie[i]>='0' && cheie[i]<='9'))
        {
            return 0;
        }
        numar=numar*10+cheie[i]-48;
    }
    if (numar>255)
        return 0;
    return 1;

}

int verifica_cheie_OTP(char *cheie,int lungime_cheie)//cheia trebuie sa fie un cuvant
{
    if (lungime_cheie==0)
        return 0;
    for (int i=0;i<lungime_cheie;i++)//verificam sa fie numai caractere in cheie
    {
        if (!isalpha(cheie[i]))
            return 0;
    }
    return 1;
}

int verifica_cheie_caesar(char *cheie,int lungime_cheie)//cheia trebuie sa fie un numar intre 1 si 25
{
    int numar=0;
    if (lungime_cheie==0 || lungime_cheie>2)//eliminam cheia goala(dupa ce se sterge ceva introdus; eliminam cheia prea lunga
        return 0;
    if (cheie[0]==48)//eliminam numere ce au inaintea lor cifra 0(chiea 0 este cheie invalida)
        return 0;
    for (int i=0;i<lungime_cheie;i++)//verificam sa se contina numai cifre. in acelasi timp si convertim numarul daca e posibil
    {
        if (!(cheie[i]>='0' && cheie[i]<='9'))
        {
            return 0;
        }
        numar=numar*10+cheie[i]-48;
    }
    if (numar>25)
        return 0;
    return 1;
}

void afiseaza_cheie(char *cheie)
{
    setfillstyle(1,BLACK);
    bar(lungime_ecran/4,3*inaltime_ecran/18,3*lungime_ecran/4,5*inaltime_ecran/18);

    settextstyle(4,0,2);
    settextjustify(1,1);
    setcolor(RED);
    setbkcolor(BLACK);
    outtextxy(lungime_ecran/2,4*inaltime_ecran/18,cheie);
}

int verifica_cheie_metoda(int metoda,char *cheie,int lungime_cheie)
{
    if (metoda==1)
        return verifica_cheie_caesar(cheie,lungime_cheie);
    if (metoda==2)
        return verifica_cheie_OTP(cheie,lungime_cheie);
    if (metoda==3)
        return verifica_cheie_XOR(cheie,lungime_cheie);
    return 1;//aici nu se ajunge niciodata
}

int prelucrare_cheie(char *cheie,int *lungime_cheie,int metoda)
{
    int c,cheie_corecta=0;
    c=getch();
    if (c==0)//eliminam tastele speciale apasate
        getch();
    if (c==8)//daca a fost sters ultimul caracter
    {
        if (*lungime_cheie>0)
        {
            cheie[*lungime_cheie-1]=0;
            (*lungime_cheie)--;
            afiseaza_cheie (cheie);
        }
        cheie_corecta=verifica_cheie_metoda(metoda,cheie,*lungime_cheie);
    }
    else
    {
        if (*lungime_cheie<10)
        {
            cheie[*lungime_cheie]=c;
            (*lungime_cheie)++;
            cheie[*lungime_cheie]=0;
            cheie_corecta=verifica_cheie_metoda(metoda,cheie,*lungime_cheie);
            if (cheie_corecta || *lungime_cheie==0)
                afiseaza_cheie (cheie);
            if (!cheie_corecta && *lungime_cheie>0)//eliminam ultimul caracter introdus gresit
            {
                cheie[*lungime_cheie-1]=0;
                (*lungime_cheie)--;
                //verificam inca odata ce era inainte
                cheie_corecta=verifica_cheie_metoda(metoda,cheie,*lungime_cheie);
            }
        }
    }
    return cheie_corecta;
}

void afiseaza_text1(char *text)
{
    setfillstyle(1,BLACK);
    bar(lungime_ecran/4,7*inaltime_ecran/36,3*lungime_ecran/4,11*inaltime_ecran/36);

    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(RED);
    setbkcolor(BLACK);
    outtextxy(lungime_ecran/2,9*inaltime_ecran/36,text);
}

void prelucrare_text_citit(char *text,int *lungime_text)
{
    int c;
    c=getch();
    if (c==0)//eliminam tastele speciale apasate
        getch();
    if (c==8)//daca a fost sters ultimul caracter
    {
        if (*lungime_text>0)
        {
            text[*lungime_text-1]=0;
            (*lungime_text)--;
            afiseaza_text1 (text);
        }
    }
    if (c>=32)
    {
        if (*lungime_text<30)
        {
            text[*lungime_text]=c;
            (*lungime_text)++;
            text[*lungime_text]=0;
            afiseaza_text1 (text);
        }
    }
}

void cripteaza_text_caesar(char *cheie,char *text,char *text_criptat)
{
    int numar=0,lungime_cheie=strlen(cheie),lungime_text=strlen(text);
    for (int i=0;i<lungime_cheie;i++)//formam cheia ca si numar
    {
        numar=numar*10+cheie[i]-48;
    }
    char caracter_curent,caracter_nou;
    int index_curent_text=0;
    while (index_curent_text<lungime_text)
    {
        caracter_curent=text[index_curent_text];
        if (isalpha(caracter_curent))//criptam doar literele
        {
            caracter_nou=caracter_curent+numar;
            //verificam cazurile cand se trece de litera Z in functie de litera mare sau mica
            if (isupper(caracter_curent))
            {
                caracter_nou-=64;
                if (caracter_nou>26)
                    caracter_nou-=26;
                caracter_nou+=64;
            }
            else
            {
                caracter_nou-=96;
                if (caracter_nou>26)
                    caracter_nou-=26;
                caracter_nou+=96;
            }
        }
        else
        {
            caracter_nou=caracter_curent;
        }
        text_criptat[index_curent_text]=caracter_nou;
        index_curent_text++;
    }
}

void cripteaza_text_OTP(char *cheie,char *text,char *text_criptat)
{
    int lungime_cheie=strlen(cheie),lungime_text=strlen(text);
    int pozitie_curenta_cheie=0,reseteaza_cuvant=0;//reseteaza cuvant devine 1 la intalnirea unui caracter ce nu e litera si 0 cand e litera
    int valori_litere_cheie[11];
    for (int i=0;i<lungime_cheie;i++)//ne aflam valorile fiecarei litere din cheie in functie de litera mare sau mica
    {
        if (isupper(cheie[i]))
            valori_litere_cheie[i]=cheie[i]-64;
        else
            valori_litere_cheie[i]=cheie[i]-96;
    }
    char caracter_curent,caracter_nou;
    int index_curent_text=0;
    while (index_curent_text<lungime_text)
    {
        caracter_curent=text[index_curent_text];
        if (isalpha(caracter_curent))//prelucram daca avem litera
        {
            if (reseteaza_cuvant==1)//verificam daca litera curenta este inceput de cuvant
            {
                reseteaza_cuvant=0;
                pozitie_curenta_cheie=0;
            }
            if (pozitie_curenta_cheie==lungime_cheie)//verificam daca am ajuns la sfarsitul cheii
            {
                pozitie_curenta_cheie=0;
            }
            caracter_nou=caracter_curent+valori_litere_cheie[pozitie_curenta_cheie];
            pozitie_curenta_cheie++;
            //verificam cazurile cand se trece de litera Z in functie de litera mare sau mica
            if (isupper(caracter_curent))
            {
                caracter_nou-=64;
                if (caracter_nou>26)
                    caracter_nou-=26;
                caracter_nou+=64;
            }
            else
            {
                caracter_nou-=96;
                if (caracter_nou>26)
                    caracter_nou-=26;
                caracter_nou+=96;
            }
        }
        else
        {
            caracter_nou=caracter_curent;
            reseteaza_cuvant=1;
        }
        text_criptat[index_curent_text]=caracter_nou;
        index_curent_text++;
    }
}

void cripteaza_text_XOR(char *cheie,char *text,char *text_criptat)
{
    int numar=0,lungime_cheie=strlen(cheie),lungime_text=strlen(text);
    for (int i=0;i<lungime_cheie;i++)//formam cheia ca si numar
    {
        numar=numar*10+cheie[i]-48;
    }
    char caracter_curent,caracter_nou;
    int index_curent_text=0;
    while (index_curent_text<lungime_text)
    {
        caracter_curent=text[index_curent_text];
        caracter_nou=caracter_curent^numar;
        text_criptat[index_curent_text]=caracter_nou;//la acest moment se pot adauga si valori ce nu pot fi afisate. se rezolva la afisare
        index_curent_text++;
    }
}

void afiseaza_texte(char *text1,char *text_criptat)
{
    int lungime_text=strlen(text1),pozitie_curenta_culori=0,vector_culori[9]={BLUE,LIGHTMAGENTA,GREEN,LIGHTCYAN,MAGENTA,LIGHTGREEN,YELLOW,LIGHTBLUE,CYAN};
    int nr_culori=9,pozitie_curenta_text=0,pozitie_orizontala_text=lungime_ecran/4;
    char text_de_afisat[3]={0,0,0};
    int caracter_special=0;

    setfillstyle(1,BLACK);
    bar(lungime_ecran/4,15*inaltime_ecran/36,24*lungime_ecran/25,19*inaltime_ecran/36);
    bar(lungime_ecran/4,7*inaltime_ecran/36,3*lungime_ecran/4,11*inaltime_ecran/36);
    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(WHITE);
    setbkcolor(BLACK);
    outtextxy(lungime_ecran/2,13*inaltime_ecran/36,"Textul criptat este:");

    settextjustify(0,1);
    while (pozitie_curenta_text<lungime_text)//afisam primul text
    {
        if (pozitie_curenta_culori==nr_culori)//resetam culorile de la inceput
            pozitie_curenta_culori=0;
        text_de_afisat[0]=text1[pozitie_curenta_text];
        if (text_criptat[pozitie_curenta_text]<32) setcolor(RED);
        else setcolor(vector_culori[pozitie_curenta_culori]);
        outtextxy(pozitie_orizontala_text,9*inaltime_ecran/36,text_de_afisat);
        pozitie_orizontala_text+=textwidth(text_de_afisat);
        pozitie_curenta_text++;
        pozitie_curenta_culori++;
    }
    pozitie_curenta_culori=pozitie_curenta_text=0;
    pozitie_orizontala_text=lungime_ecran/4;
    while (pozitie_curenta_text<lungime_text)//afisam primul text
    {
        text_de_afisat[0]=text_de_afisat[1]=0;
        if (pozitie_curenta_culori==nr_culori)//resetam culorile de la inceput
            pozitie_curenta_culori=0;
        if (text_criptat[pozitie_curenta_text]>=32)//verificam daca putem afisa caracterul criptat. daca nu afisam numarul rezultat
            text_de_afisat[0]=text_criptat[pozitie_curenta_text];
        else
        {
            //verificam daca are 1 sau 2 cifre caracterul curent
            if (text_criptat[pozitie_curenta_text]<10)
                text_de_afisat[0]=text_criptat[pozitie_curenta_text]+48;
            else
            {
                text_de_afisat[0]=text_criptat[pozitie_curenta_text]/10+48;
                text_de_afisat[1]=text_criptat[pozitie_curenta_text]%10+48;
            }
            caracter_special=1;
        }
        if (caracter_special)setcolor(RED);
        else setcolor(vector_culori[pozitie_curenta_culori]);
        outtextxy(pozitie_orizontala_text,17*inaltime_ecran/36,text_de_afisat);
        pozitie_orizontala_text+=textwidth(text_de_afisat);
        pozitie_curenta_text++;
        pozitie_curenta_culori++;
        caracter_special=0;
    }
}

void meniu_mesaj_tastatura(int metoda,char *cheie)
{
    int window=initwindow(lungime_ecran,inaltime_ecran,"Criptare text");

    setfillstyle(1,GREEN);
    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(BLACK);
    setbkcolor(GREEN);
    readimagefile("matrix-background-dark.jpg",0,0,lungime_ecran,inaltime_ecran);
    bar(lungime_ecran/4,7*inaltime_ecran/9,3*lungime_ecran/4,8*inaltime_ecran/9);
    bar(lungime_ecran/4,5*inaltime_ecran/9,3*lungime_ecran/4,6*inaltime_ecran/9);
    outtextxy(lungime_ecran/2,15*inaltime_ecran/18,"Inapoi");
    outtextxy(lungime_ecran/2,11*inaltime_ecran/18,"Cripteaza!");

    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(WHITE);
    setbkcolor(BLACK);
    char de_afisat[30]="Cheia este:";
    strcat(de_afisat,cheie);
    outtextxy(lungime_ecran/2,3*inaltime_ecran/36,de_afisat);

    setfillstyle(1,BLACK);
    bar(lungime_ecran/4,7*inaltime_ecran/36,3*lungime_ecran/4,11*inaltime_ecran/36);
    outtextxy(lungime_ecran/2,5*inaltime_ecran/36,"Introduceti textul(maxim 30 caractere):");


    int iesire_meniu=0,coord_mouse_x,coord_mouse_y,buton_apasat,tasta_apasata;
    char text[31]="",text_criptat[31];
    int lungime_text=0;
    while (!iesire_meniu)
    {
        while (!(ismouseclick(WM_LBUTTONDOWN) || kbhit()));
        tasta_apasata=kbhit();
        if (tasta_apasata)//daca a fost apasata o tasta verificam ce a fost apasat si prelucram
        {
            prelucrare_text_citit(text,&lungime_text);
        }
        else
        {
            getmouseclick(WM_LBUTTONDOWN, coord_mouse_x,coord_mouse_y);
            buton_apasat=gaseste_buton_meniu(coord_mouse_x,coord_mouse_y);
            if (buton_apasat==4)
                iesire_meniu=1;
            if (buton_apasat==3)
            {
                if (metoda==1)
                    cripteaza_text_caesar(cheie,text,text_criptat);
                if (metoda==2)
                    cripteaza_text_OTP(cheie,text,text_criptat);
                if (metoda==3)
                    cripteaza_text_XOR(cheie,text,text_criptat);
                afiseaza_texte(text,text_criptat);
            }
        }
    }
    closegraph(window);
}

void meniu_criptare_mesaj(int metoda)
{
    //metoda=1 pt caesar, 2 pt OTP, 3 pentru XOR
    int window=initwindow(lungime_ecran,inaltime_ecran,"Criptare text(cheie)");

    setfillstyle(1,GREEN);
    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(BLACK);
    setbkcolor(GREEN);
    readimagefile("matrix-background-dark.jpg",0,0,lungime_ecran,inaltime_ecran);
    bar(lungime_ecran/4,7*inaltime_ecran/9,3*lungime_ecran/4,8*inaltime_ecran/9);
    bar(lungime_ecran/4,5*inaltime_ecran/9,3*lungime_ecran/4,6*inaltime_ecran/9);
    outtextxy(lungime_ecran/2,15*inaltime_ecran/18,"Inapoi");
    outtextxy(lungime_ecran/2,11*inaltime_ecran/18,"Mai departe!");

    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(WHITE);
    setbkcolor(BLACK);
    outtextxy(lungime_ecran/2,2*inaltime_ecran/18,"Introduceti cheia:");

    setfillstyle(1,BLACK);
    bar(lungime_ecran/4,3*inaltime_ecran/18,3*lungime_ecran/4,5*inaltime_ecran/18);

    int iesire_meniu=0,coord_mouse_x,coord_mouse_y,buton_apasat,tasta_apasata;
    char cheie[11]="";
    int lungime_cheie=0,cheie_corecta=0;
    while (!iesire_meniu)
    {
        while (!(ismouseclick(WM_LBUTTONDOWN) || kbhit()));
        tasta_apasata=kbhit();
        if (tasta_apasata)//daca a fost apasata o tasta verificam ce a fost apasat si prelucram
        {
            cheie_corecta=prelucrare_cheie(cheie,&lungime_cheie,metoda);
        }
        else
        {
            getmouseclick(WM_LBUTTONDOWN, coord_mouse_x,coord_mouse_y);
            buton_apasat=gaseste_buton_meniu(coord_mouse_x,coord_mouse_y);
            if (buton_apasat==4)
                iesire_meniu=1;
            if (buton_apasat==3 && cheie_corecta)
            {
                meniu_mesaj_tastatura(metoda,cheie);
                setcurrentwindow(window);
                goleste_buffer_taste_mouse();
            }
            if (buton_apasat==3 && cheie_corecta==0)
            {
                settextstyle(4,0,1);
                settextjustify(1,1);
                setcolor(WHITE);
                setbkcolor(BLACK);
                setfillstyle(1,BLACK);
                bar(lungime_ecran/4,17*inaltime_ecran/36,3*lungime_ecran/4,19*inaltime_ecran/36);
                outtextxy(lungime_ecran/2,9*inaltime_ecran/18,"Cheie incorecta!");
            }
        }
    }
    closegraph(window);
}

void meniu_criptare_fisier(int metoda)
{
    //metoda=1 pt caesar, 2 pt OTP, 3 pentru XOR
    int window=initwindow(lungime_ecran,inaltime_ecran,"Criptare fisier");

    setfillstyle(1,GREEN);
    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(BLACK);
    setbkcolor(GREEN);
    readimagefile("matrix-background-dark.jpg",0,0,lungime_ecran,inaltime_ecran);
    bar(lungime_ecran/4,7*inaltime_ecran/9,3*lungime_ecran/4,8*inaltime_ecran/9);
    bar(lungime_ecran/4,5*inaltime_ecran/9,3*lungime_ecran/4,6*inaltime_ecran/9);
    outtextxy(lungime_ecran/2,15*inaltime_ecran/18,"Inapoi");
    outtextxy(lungime_ecran/2,11*inaltime_ecran/18,"Cripteaza!");

    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(WHITE);
    setbkcolor(BLACK);
    outtextxy(lungime_ecran/2,2*inaltime_ecran/18,"Introduceti cheia:");

    setfillstyle(1,BLACK);
    bar(lungime_ecran/4,3*inaltime_ecran/18,3*lungime_ecran/4,5*inaltime_ecran/18);

    int iesire_meniu=0,coord_mouse_x,coord_mouse_y,buton_apasat,tasta_apasata;
    char cheie[11]="";
    int lungime_cheie=0,cheie_corecta=0;
    while (!iesire_meniu)
    {
        while (!(ismouseclick(WM_LBUTTONDOWN) || kbhit()));
        tasta_apasata=kbhit();
        if (tasta_apasata)//daca a fost apasata o tasta verificam ce a fost apasat si prelucram
        {
            cheie_corecta=prelucrare_cheie(cheie,&lungime_cheie,metoda);
        }
        else
        {
            getmouseclick(WM_LBUTTONDOWN, coord_mouse_x,coord_mouse_y);
            buton_apasat=gaseste_buton_meniu(coord_mouse_x,coord_mouse_y);
            if (buton_apasat==4)
                iesire_meniu=1;
            if (buton_apasat==3 && cheie_corecta)
            {
                if (metoda==1)
                    cripteaza_fisier_caesar(cheie);
                if (metoda==2)
                    cripteaza_fisier_OTP(cheie);
                if (metoda==3)
                    cripteaza_fisier_XOR(cheie);
                settextstyle(4,0,1);
                settextjustify(1,1);
                setcolor(WHITE);
                setbkcolor(BLACK);
                char mesaj[40]="Fisierul a fost criptat cu cheia ";
                strcat(mesaj,cheie);
                setfillstyle(1,BLACK);
                bar(lungime_ecran/4,17*inaltime_ecran/36,3*lungime_ecran/4,19*inaltime_ecran/36);
                outtextxy(lungime_ecran/2,9*inaltime_ecran/18,mesaj);
            }
            if (buton_apasat==3 && cheie_corecta==0)
            {
                settextstyle(4,0,1);
                settextjustify(1,1);
                setcolor(WHITE);
                setbkcolor(BLACK);
                setfillstyle(1,BLACK);
                bar(lungime_ecran/4,17*inaltime_ecran/36,3*lungime_ecran/4,19*inaltime_ecran/36);
                outtextxy(lungime_ecran/2,9*inaltime_ecran/18,"Cheie incorecta!");
            }
        }
    }
    closegraph(window);
}

void meniu_caesar_graphic()
{
    int i,iesire_meniu=0,coord_mouse_x,coord_mouse_y,buton_apasat;
    char t1[25]="Despre Caesar cipher",t2[25]="Cripteaza fisier",t3[25]="Cripteaza mesaj",t4[25]="Inapoi";
    i=initwindow(lungime_ecran,inaltime_ecran,"Metoda Caesar cipher");
    readimagefile("matrix-background-dark.jpg",0,0,lungime_ecran,inaltime_ecran);
    afisare_4_butoane_meniu(t1,t2,t3,t4);
    while (!iesire_meniu)
    {
        while (!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, coord_mouse_x,coord_mouse_y);
        buton_apasat=gaseste_buton_meniu(coord_mouse_x,coord_mouse_y);
        if (buton_apasat==4)
            iesire_meniu=1;
        if (buton_apasat==1)
        {
            citeste_descriere("despre caesar.txt","Despre Caesar Cipher");
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
        if (buton_apasat==2)
        {
            meniu_criptare_fisier(1);
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
        if (buton_apasat==3)
        {
            meniu_criptare_mesaj(1);
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
    }

    closegraph(i);
}

void meniu_OTP_graphic()
{
    int i,iesire_meniu=0,coord_mouse_x,coord_mouse_y,buton_apasat;
    char t1[25]="Despre one time pad",t2[25]="Cripteaza fisier",t3[25]="Cripteaza mesaj",t4[25]="Inapoi";
    i=initwindow(lungime_ecran,inaltime_ecran,"Metoda One time pad");
    readimagefile("matrix-background-dark.jpg",0,0,lungime_ecran,inaltime_ecran);
    afisare_4_butoane_meniu(t1,t2,t3,t4);
    while (!iesire_meniu)
    {
        while (!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, coord_mouse_x,coord_mouse_y);
        buton_apasat=gaseste_buton_meniu(coord_mouse_x,coord_mouse_y);
        if (buton_apasat==4)
            iesire_meniu=1;
        if (buton_apasat==1)
        {
            citeste_descriere("despre OTP.txt","Despre OTP");
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
        if (buton_apasat==2)
        {
            meniu_criptare_fisier(2);
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
        if (buton_apasat==3)
        {
            meniu_criptare_mesaj(2);
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
    }

    closegraph(i);
}

void meniu_XOR_graphic()
{
    int i,iesire_meniu=0,coord_mouse_x,coord_mouse_y,buton_apasat;
    char t1[25]="Despre metoda XOR",t2[25]="Cripteaza fisier",t3[25]="Cripteaza mesaj",t4[25]="Inapoi";
    i=initwindow(lungime_ecran,inaltime_ecran,"Metoda XOR");
    readimagefile("matrix-background-dark.jpg",0,0,lungime_ecran,inaltime_ecran);
    afisare_4_butoane_meniu(t1,t2,t3,t4);
    while (!iesire_meniu)
    {
        while (!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, coord_mouse_x,coord_mouse_y);
        buton_apasat=gaseste_buton_meniu(coord_mouse_x,coord_mouse_y);
        if (buton_apasat==4)
            iesire_meniu=1;
        if (buton_apasat==1)
        {
            citeste_descriere("despre XOR.txt","Despre Xor");
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
        if (buton_apasat==2)
        {
            meniu_criptare_fisier(3);
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
        if (buton_apasat==3)
        {
            meniu_criptare_mesaj(3);
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
    }

    closegraph(i);
}

void afisare_4_butoane_meniu(char *text_1,char *text_2,char *text_3,char *text_4)
{
    //setari buton
    setfillstyle(1,GREEN);
    settextstyle(4,0,1);
    settextjustify(1,1);
    setcolor(BLACK);
    setbkcolor(GREEN);

    bar(lungime_ecran/4,inaltime_ecran/9,3*lungime_ecran/4,2*inaltime_ecran/9);
    outtextxy(lungime_ecran/2,3*inaltime_ecran/18,text_1);

    bar(lungime_ecran/4,3*inaltime_ecran/9,3*lungime_ecran/4,4*inaltime_ecran/9);
    outtextxy(lungime_ecran/2,7*inaltime_ecran/18,text_2);

    bar(lungime_ecran/4,5*inaltime_ecran/9,3*lungime_ecran/4,6*inaltime_ecran/9);
    outtextxy(lungime_ecran/2,11*inaltime_ecran/18,text_3);

    bar(lungime_ecran/4,7*inaltime_ecran/9,3*lungime_ecran/4,8*inaltime_ecran/9);
    outtextxy(lungime_ecran/2,15*inaltime_ecran/18,text_4);
}

void meniu_graphic()
{
    int i;
    char t1[20]="Caesar cipher",t2[20]="One time pad",t3[20]="XOR",t4[20]="Iesire";
    i=initwindow(lungime_ecran,inaltime_ecran,"Metode de criptare");
    readimagefile("intrare.jpg",0,0,1300,1100);
    delay(3000);
    readimagefile("matrix-background-dark.jpg",0,0,lungime_ecran,inaltime_ecran);
    afisare_4_butoane_meniu(t1,t2,t3,t4);
    int iesire_program=0,coord_mouse_x,coord_mouse_y,buton_apasat;
    while (!iesire_program)
    {
        while (!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, coord_mouse_x,coord_mouse_y);
        buton_apasat=gaseste_buton_meniu(coord_mouse_x,coord_mouse_y);
        if (buton_apasat==4)
        {
            int k;
                initwindow(getmaxwidth(),getmaxheight(),"pagina_iesire",0,0);
                readimagefile("iesire.jpg",0,0,1300,1100);
                delay(2000);
                iesire_program=1;
        }
        if (buton_apasat==1)
        {
            meniu_caesar_graphic();
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
        if (buton_apasat==2)
        {
            meniu_OTP_graphic();
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
        if (buton_apasat==3)
        {
            meniu_XOR_graphic();
            setcurrentwindow(i);
            goleste_buffer_taste_mouse();
        }
    }
    closegraph(i);
}

int gaseste_buton_meniu(int x,int y)
{
    int buton_sus=0,buton_stg=0;
    if (lungime_ecran/4<=x && x<=3*lungime_ecran/4)
        buton_stg=1;

    if (inaltime_ecran/9<=y && y<=2*inaltime_ecran/9)
        buton_sus=1;
    if (3*inaltime_ecran/9<=y && y<=4*inaltime_ecran/9)
        buton_sus=2;
    if (5*inaltime_ecran/9<=y && y<=6*inaltime_ecran/9)
        buton_sus=3;
    if (7*inaltime_ecran/9<=y && y<=8*inaltime_ecran/9)
        buton_sus=4;

    if (buton_stg==0 || buton_sus==0)
        return 0;
    else
        return buton_sus;
}

int main()
{
    meniu_graphic();
    return 0;
}
