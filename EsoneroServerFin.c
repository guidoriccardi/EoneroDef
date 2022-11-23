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
#include <stdbool.h>

#define BUFFERSIZE 512		// Dimensione Buffer
#define PROTOPORT 27015 // default protocol port number
#define QLEN 6 // size of request queue


	//funzione stampa errore
	void ErrorHandler(char *errorMessage) {
	printf ("%s", errorMessage);
	}


 	//funzione che pulisce la socket
	void ClearWinSock() {
	#if defined WIN32
	WSACleanup();
	#endif
	}





	//controllo della porta
	int main(int argc, char *argv[])
    {
	int port;
	if (argc > 1){
		port = atoi(argv[1]);
	}
	else port = PROTOPORT;	// Default
	if (port < 0){
		printf ("Bad port number %s \n",argv[1]);
		return 0;
	}
	
	



//funzione per verificare sistema operativo
#if defined WIN32
	WSADATA WSAData;
	int iResult = WSAStartup(MAKEWORD(2,2), &WSAData);
	
	if (iResult !=0){
		ErrorHandler("Error at WSAStartup\n");
		return 0;
	}
#endif




	// CREAZIONE DELLA SOCKET
	int Ssocket;
	Ssocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//Valutare la presenza di errori per assicurarsi che la socket sia valida.
	if (Ssocket < 0) {
	ErrorHandler("socket creation failed.\n");
	ClearWinSock();
	return -1;
	}




	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(port); //codice della porta presente nel client (5193)



	//BINDING (and errorhandler): ASSOCIA ALLA SOCKET (Ssocket) IL CONTENUTO DELLA STRUCT (sad)
	if (bind(Ssocket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {

	ErrorHandler("bind() failed.\n");
	closesocket(Ssocket);
	ClearWinSock();
	return -1;
	}




	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if (listen (Ssocket, QLEN) < 0) {
		
	ErrorHandler("listen() failed.\n");
	closesocket(Ssocket);
	ClearWinSock();
	return -1;
    }





	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in cad; 
	
	int clientSocket;
	int clientLen;
	
	printf("Waiting for a client to connect...");
	
	
	
while (1) 
{ 

	clientLen = sizeof(cad); // set the size of the client address
	clientSocket = accept(Ssocket, (struct sockaddr *)&cad,&clientLen);
	
	printf("\n clientSocket %d\n",clientSocket);
	if (clientSocket < 0) 
	{
		//chiudi la connessione
		ErrorHandler("accept() failed.\n");
		closesocket(Ssocket);	
		ClearWinSock();
		return 0;
	}
	
	printf("Connessione stabilita con  il client con indirizzo IP: %s\n", inet_ntoa(cad.sin_addr));
	
	
     //creazione variabili per ricezione 
     int ricezione_A;
     char string_A[BUFFERSIZE];
     
     int ricezione_B;
     char string_B[BUFFERSIZE];
	 
	 char buf[BUFFERSIZE];
	 char bufA[BUFFERSIZE];
	 char bufB[BUFFERSIZE];
     
     char string_C[BUFFERSIZE];
     
	 char *fraseCA="Connessione avvenuta :)";
	
	
	 char *msgBYE="bye";
	
	//INVIO STRINGA AL CLIENT per Connessione avvenuta +verifica che sia arrivata

    
   
   	int i=0;
	if (send(clientSocket,fraseCA,strlen(fraseCA),0)!=strlen(fraseCA)){
			ErrorHandler("Byte non ricevuti correttamente");
			closesocket(clientSocket);
			ClearWinSock();
			system("PAUSE");
			return -1;
		} else {
			    do {
				    
				    strcpy(bufA,"");
				    strcpy(bufB,"");
				    
								     
				     strcpy(string_A,"");  //svuotare stringa
				     strcpy(string_B,"");  //svuotare stringa
				
				     
				     //ricezione stringa A
				     ricezione_A=recv(clientSocket,bufA,BUFFERSIZE-1,0);
                     if(ricezione_A==-1)
					 {
                        ErrorHandler("Errore ricezione della stringa A\n");
                        closesocket(Ssocket);
                        ClearWinSock();
					 }
                     else {
                     bufA[ricezione_A]="\0";
					 strcpy(string_A,bufA);
					 printf("Stringa ricevuta A: %s\n",string_A);
				     strcpy(bufA,"\0");  //svuotare stringa	
					 } 
				     				    
				     
				     //ricezione stringa B
				    ricezione_B=recv(clientSocket,bufB,BUFFERSIZE-1,0);
                     if(ricezione_B==-1)
					 {
                        ErrorHandler("Errore ricezione della stringa B\n");
                        closesocket(Ssocket);
                        ClearWinSock();
                     }
                    else {
                     bufB[ricezione_B]="\0";
					 strcpy(string_B,bufB);
					 printf("Stringa ricevuta B: %s\n",string_B);
				     strcpy(bufB,"\0");  //svuotare stringa	
	
					 } 
				     
				     
				     
				     //CONTROLLO DEL QUIT
				     if(strcmp(string_A,"QUIT")==0|| strcmp(string_B, "QUIT")==0)
				     {
				     	printf("Ricevuta stringa per disconnessione dal server...\n\n");
				     	if(send(clientSocket,msgBYE,strlen(msgBYE),0)!=strlen(msgBYE))
				     	{
				     	   	ErrorHandler("Errore invio messaggio del BYE\n\n");
                            closesocket(Ssocket);
                            ClearWinSock();
						}
					 } else               //concatenazione di string A e B
					        {
					 	        strcpy(string_C,"\0");
					            strcat(string_C,string_A);
					            strcat(string_C,string_B);
					            printf("STRINGA CONCATENATA --> %s \n",string_C);
					            
					            if(send(clientSocket,string_C,strlen(string_C),0)!=strlen(string_C))
					            {
					            	ErrorHandler("Errore invio della stringa C\n\n");
                                    closesocket(Ssocket);
                                    ClearWinSock();
								}
					        }
				     
		        	} while(strcmp(string_A, "QUIT")!=0 && strcmp(string_B, "QUIT")!=0);
		
		} printf("Connessione terminata con il client %s    :( \n",inet_ntoa(cad.sin_addr));





}


}    //chiusura del main



