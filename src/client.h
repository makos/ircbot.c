typedef struct WSAData WSADATA;

typedef struct {
    char* port;
    char* ipv4_addr;
    char* recvbuf;
    int recvbuflen;
    char* sendbuf;
    int sendbuflen;
    WSADATA* wsaData;
    int i_result;
} IRC_Connection;

IRC_Connection* client_create(char addr[], char port[], int recvbuflen,
                              int sendbuflen);
