#ifndef TESTFILE
#define TESTFILE

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>

double randomchoice() 
{
    /*int randomInt = rand();
    double randomDouble = ((double)randomInt / RAND_MAX) - 1;*/
    // Initialise le générateur de nombres aléatoires avec une graine basée sur l'heure actuelle
    // Génère un nombre aléatoire entre -1 et 1
    double randomValue = (double)rand() / RAND_MAX; // Valeur entre 0 et 1
    randomValue = 2 * randomValue - 1; // Étend la plage de 0 à 1 à -1 à 1
    //printf("%f\n",randomValue);
    return randomValue;
    //return randomDouble;
}



void insert_file(size_t a, size_t b, char* file) //insert des valeurs aleatoires dans le fichier file
{
    int fd = open(file, O_WRONLY);
    if (fd == -1)
        err(1,"open()1");
    size_t i = 0;
    size_t j = 0;
    char str[80];
    long nbr = 0;
    while(i<a)
    {
        j =0;
        while(j<b)
        {
            double random = randomchoice();
            sprintf(str, "%f\n", random);
            write(fd, str, strlen(str));
            j++;
            nbr++;
        }
        i++;
    }
    close(fd);
}



void insert_list(size_t a, size_t b, char* file, double** tab) // recupere les donnees de file et les mets dans tab
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
        err(1,"open()");

    int r,w;
    char buffer[64];
    char nbr[80];
    size_t k =0;
    size_t z =0;
    size_t i = 0;
    while((r = read(fd, buffer, 64)) != 0)
    {
        size_t j = 0;
        while(j < r)
        {
            char* eptr;
            double result;
            if(buffer[j] == '\n')
            {
                nbr[i] = 0;
                result = strtod(nbr, &eptr);
                i =0;
                j++;
                tab[k][z] = result;
                z++;
                if(z == b)
                {
                    z = 0;
                    k++;
                }
            }
            else
            {
                nbr[i] = buffer[j];
                i++;
                j++;
            }
        }
    }
    close(fd);

}



void replace(size_t a, size_t b, char* file, double** tab) // detruit le fichier file et le remplace par un nouveau avec les valeurs de tab
{
    if(remove(file) ==-1)
        err(1,"remove()");
    char str[80];
    int fd = open(file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        err(1, "open()");
    for( size_t i =0; i<a;i++)
    {
        for(size_t j =0; j <b;j++)
        {

            sprintf(str, "%f\n", tab[i][j]);
            if(write(fd, str, strlen(str)) == -1)
                err(1, "write()");

        }
    }
    close(fd);
}

void remplace(char* file)
{
    if(remove(file) == -1)
        err(1,"remove()");
    int fd = open(file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd ==-1)
        err(1,"open()");
    close(fd);

}

#endif
