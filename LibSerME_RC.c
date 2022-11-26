#include "LibSerME_RC.h"

#include <stdlib.h>

void AProposServeurHV(char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version);
 printf("Nom1 : %s \n",Nom1);
 printf("Nom2 : %s \n",Nom2);
}

int RechercheME_RC(char *NomFichier, int Reference, struct VehiculeHV *UnRecord){
    
    //Ouverture fichier dans lequel rechercher le vehicule
    FILE *fichier = fopen(NomFichier, "r");
    if(fichier == NULL){
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }

    int count = 0;

    while(1){
        if((count = fread(UnRecord, sizeof(struct VehiculeHV), 1, fichier)) == 0)
            return 0;
    
        if(UnRecord->Reference == Reference)
            break;
    }

    fclose(fichier);

    return 1;
}

