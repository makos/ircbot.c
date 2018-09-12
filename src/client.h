typedef struct WSAData WSADATA;
struct addrinfo;
typedef unsigned long long UINT_PTR;
typedef UINT_PTR SOCKET;

typedef struct {
    char* port;
    char* ipv4_addr;
    char* recvbuf;
    int recvbuflen;
    char* sendbuf;
    int sendbuflen;
    WSADATA* wsaData;
    struct addrinfo* ai_result;
    struct addrinfo* ai_ptr;
    struct addrinfo hints;
    int i_result;
} IRC_Connection;

IRC_Connection* client_create(char addr[], char port[], int recvbuflen,
                              int sendbuflen);
