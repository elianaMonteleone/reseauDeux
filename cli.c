/*---------------------------------------------------------------
   Vanstapel Herman
   ex02\cli.c

 Le client dit bonjour en utilisant un structure  et
 le serveur fait de même
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "../udplib/udplib.h"
#include "requeteme_rc.h"

void die(char *s){
   perror(s);
   exit(1);
}

int main(int argc, char *argv[]){
   int rc;
   int Desc;
   int tm; 

   u_long  IpSocket , IpServer;
   u_short PortSocket, PortServer; 

   struct sockaddr_in sthis; /* this ce programme */
   struct sockaddr_in sos; /* s = serveur */
   struct sockaddr_in sor; /* r = remote */
   struct RequeteME_RC UneRequete;

   memset(&sthis, 0, sizeof(struct sockaddr_in));
   memset(&sos, 0, sizeof(struct sockaddr_in)); 
   memset(&sor, 0, sizeof(struct sockaddr_in));
   memset(&UneRequete, 0, sizeof(struct RequeteME_RC));

   if (argc!=5){
      printf("cli client portc serveur ports\n");
      exit(1);
   }

   /* Récupération IP & port   */
   IpSocket= inet_addr(argv[1]);
   PortSocket = atoi(argv[2]);

   IpServer = inet_addr(argv[3]);
   PortServer = atoi(argv[4]);

   // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4]));
   Desc=creer_socket(SOCK_DGRAM, &IpSocket, PortSocket, &sthis);

   if (Desc == -1)
      die("CreateSockets:"); 
   else
      printf("CreateSockets %d\n", Desc);

   sos.sin_family = AF_INET;
   sos.sin_addr.s_addr= IpServer;
   sos.sin_port = htons(PortServer);


   //Gestion entrée utilisateur de la ref du vehicule à rechercher
   char tampon[8];
   printf("Sasie reference vehicule:");
   fgets(tampon, sizeof(tampon), stdin);
   

   UneRequete.Type = Question; 
   UneRequete.Reference = atoi(tampon);

   rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteME_RC), &sos);

   if (rc == -1)
      die("SendDatagram");
   else
      fprintf(stderr, "Envoi de %d bytes\n", rc);

   memset(&UneRequete, 0, sizeof(struct RequeteME_RC));
   tm = sizeof(struct RequeteME_RC);

   rc = ReceiveDatagram(Desc, &UneRequete, tm, &sor);
   if (rc == -1)
      die("ReceiveDatagram");
   else{
      printf("bytes lus:%d\n", rc);
      //Si la requête n'a pas échouée alors affichage du véhicule
      if(UneRequete.Type != Fail)
         printf("Constructeur, Modele, Puissance:%s %s %d\n", UneRequete.Constructeur, UneRequete.Modele, UneRequete.Puissance);
      else
         printf("Echec de la requête\n");
      
      AfficheRequeteME_RC(stderr, UneRequete);
   }

   close(Desc);
}
