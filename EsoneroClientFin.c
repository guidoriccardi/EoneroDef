#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFERSIZE 512
#define PROTOPROT 27015



    //funzione stampa errore
    void ErrorHandler (char *errorMessage){
	printf ("%s",errorMessage);
    }

    //funzione che pulisce la socket
    void ClearWinSock() {
    #if defined WIN32
    WSACleanup();
    #endif
    }
    
    
    //funzione per verificare sistema operativo
    int main(void)
	{
    #if defined WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
    if (iResult != 0) {
    printf ("error at WSASturtup\n");
    return -1;
    }
    #endif




    //Creazione della socket
    int Csocket;
    Csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Csocket < 0) {
    ErrorHandler("socket creation failed.\n");
    closesocket(Csocket);
    ClearWinSock();
    return -1;
    }



    




    // COSTRUZIONE DELL INDIRIZZO DEL SERVER
    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;                   
    sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
    sad.sin_port = htons(PROTOPROT); // Server port
    
    
    
    
    
    //connessione al server
    if (connect(Csocket, (struct sockaddr *)&sad, sizeof(sad)) < 0)
    {
    ErrorHandler( "Connessione al server fallita\n" ); 
    closesocket(Csocket);
    ClearWinSock();
    return -1;
    }
    
    
   //risposta ricezione connessione dal server
   
    int ris_recv1;
	char risposta_s1[BUFFERSIZE];
	
	strcpy(risposta_s1,"\0");
	   
    int ris_recv2;
	char risposta_s2[BUFFERSIZE];
	 
	   
   if ((ris_recv1=recv(Csocket, risposta_s1, BUFFERSIZE, 0))<0)
   {
   	 ErrorHandler("Ricezione risposta del server fallita\n"); 
     closesocket(Csocket);
     ClearWinSock();
     return -1;  
   } else {
   	 risposta_s1[ris_recv1]='\0';
   	 printf("%s\n",risposta_s1);
   }

    
    char string_A[BUFFERSIZE];
    char string_B[BUFFERSIZE];
    
    
//do while per il controllo del loop
do {
	
  
    strcpy(string_A,"");    
    printf("Inserisci stringa A:  ");
    scanf("%s",string_A);
   
      if (send(Csocket,string_A,strlen(string_A),0)!=strlen(string_A)){
		ErrorHandler("Errore nell'invio della stringa A");
		closesocket(Csocket);
		ClearWinSock();
		return -1;
	} else {
		printf("STRINGA A inviata correttamente; ");
        printf("Hai scritto --> %s\n\n",string_A);
	}
    
    
    
    strcpy(string_B,""); 
    printf("Inserisci stringa B:  ");
    scanf("%s",string_B);
    
     if (send(Csocket,string_B,strlen(string_B),0)!=strlen(string_B)){
		ErrorHandler("Errore nell'invio della stringa B");
		closesocket(Csocket);
		ClearWinSock();
		return -1;
 	} else {
		printf("STRINGA B inviata correttamente; ");
        printf("Hai scritto --> %s\n\n",string_B);
	}



  //ricezione stringa c / bye

  
  if ((ris_recv2=recv(Csocket, risposta_s2, BUFFERSIZE, 0))<0)
   {
   	 ErrorHandler("Ricezione risposta del server fallita\n"); 
     closesocket(Csocket);
     ClearWinSock();
     return -1;  
   } else {
   	 risposta_s2[ris_recv2]="\0";
   	 
   	 printf("concatenata --> %s\n",risposta_s2);
   }
  
	
	
}while (strcmp(risposta_s2,"bye")!=0);
   

    
  
  


// CHIUSURA DELLA CONNESSIONE
closesocket(Csocket);
ClearWinSock();
printf("\n");
// Print a final linefeed
system("pause");  
return(0);


}
