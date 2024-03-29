#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#ifdef __VMS
#include <socket.h>
#include <inet.h>

#include <in.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define RETURN_NULL(x) if ((x)==NULL) exit (1)
#define RETURN_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define RETURN_SSL(err) if ((err)==-1) { printf("\n%d\n",err);ERR_print_errors_fp(stderr); exit(1); }

static int verify_callback(int ok, X509_STORE_CTX *ctx);

#define RSA_CLIENT_CERT       "client.crt"
#define RSA_CLIENT_KEY  "client.key"

#define RSA_CLIENT_CA_CERT      "ca.crt"
#define RSA_CLIENT_CA_PATH      "sys$common:[syshlp.examples.ssl]"

#define ON      1
#define OFF     0

extern char   __BUILD_NUMBER;

void init_col();// init color vars
void update_screen();
void display_welcome();
void init_key();

void SetSignals(void);
void handler(int signum);
void SetTimer(void);
void display_time();
void display_controller();

void* read_cont(void* in);
void* ssl_cli(void* in);

#define JOY_DEV "/dev/input/js0"


struct joystat
{
    int x1,y1,x2,y2,lt,rt;
    int buts[11];
    //[A,B,X,Y,LB,RB,back,start,xbox,l3,r3]
    //[0,1,2,3,4 ,5 ,6,  ,7    ,8   ,9 ,10]
};

struct joystat joy;
int stop=0;
int newdat=0;
pthread_mutex_t  read_mux;
pthread_mutex_t  data_mux;
int joy_fd=0;


WINDOW * mainwin;
int KK=0;

char key=0;
int main(void)
{
    long tim;
    int n = 0;

    if ( (mainwin = initscr()) == NULL )
    {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    pthread_t read_thread;
    pthread_t ssl_thread;
    pthread_mutex_init(&read_mux,NULL);
    pthread_mutex_init(&data_mux,NULL);
    pthread_create(&read_thread, NULL , read_cont, NULL);
    pthread_create(&ssl_thread, NULL , ssl_cli, NULL);

    start_color();
    init_col();
    init_key();
    srand( (unsigned) time(NULL) );
    update_screen();
    box(mainwin, 0, 0);
    SetTimer();
    SetSignals();

    while ( (key=getch()) != 'q' ) {}
    stop=1;
    usleep(50000);

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}


void SetTimer(void)
{

    struct itimerval it;


    /*  Clear itimerval struct members  */

    timerclear(&it.it_interval);
    timerclear(&it.it_value);


    /*  Set timer  */

    it.it_interval.tv_usec = 50000;
    it.it_value.tv_usec    = 50000;
    setitimer(ITIMER_REAL, &it, NULL);
}

void handler(int signum)
{

    /*  Switch on signal number  */

    switch ( signum ) {

    case SIGALRM:

        /*  Received from the timer  */
        update_screen();
        color_set(1, NULL);
        mvprintw(23, 50, "ticks  %d",KK);
        KK++;

        return;

    case SIGTERM:
    case SIGINT:

        /*  Clean up nicely  */

        delwin(mainwin);
        endwin();
        refresh();
        exit(EXIT_SUCCESS);

    }
}

void SetSignals(void)
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGTSTP, &sa, NULL);
}


void update_screen()
{
    display_welcome();
    display_time();
    display_controller();
    static n=0;
    if ( n <= 13) {
        refresh();
        //usleep(5000000);
        //tim=time(NULL)+1;
        //while(tim>=time(NULL)){}
        color_set(n%13, NULL);
        mvprintw(2+n%20, 1, "HELLACAT %d,    %d ",time(NULL), n);
        //getch();
        n++;
    }
    curs_set(0);

}

void display_time()
{

    time_t rawtime;
    time (&rawtime);
    color_set(11, NULL);
    mvprintw(24,56,"%s",ctime (&rawtime));
}


void display_welcome()
{
    //box(mainwin, 0, 0);
    color_set(7, NULL);
    mvprintw(1, 1, " HELLACAT Controller Version #0.5.8 alpha                     ");//50
    color_set(8, NULL);
    mvprintw(1, 63, " Press q to Quit");
}

void display_controller()
{

    pthread_mutex_lock(&data_mux);
    color_set(11, NULL);
    mvprintw(3,43 ," %4d\\______%d      %d______/%4d   ",joy.lt>>8,joy.buts[4],joy.buts[5],joy.rt>>8);
    mvprintw(4,43 ,"      |     |______|   %d  |       ",joy.buts[3]);
    mvprintw(5,43 ,"     |     |    X    %d   %d |      ",joy.buts[2],joy.buts[1]);
    mvprintw(6,43 ,"    |%4d--%d--         %d    |     ",joy.x1>>8,joy.buts[9],joy.buts[0]);
    mvprintw(7,43 ,"   |       |  %d %d %d      |   |    ",joy.buts[6],joy.buts[8],joy.buts[7]);
    mvprintw(8,43 ,"  |     %4d       %4d--%d--  |   ",joy.y1>>8,joy.x2>>8,joy.buts[10]);
    mvprintw(9,43 ," |            ____       |     |  ");
    mvprintw(10,43,"|            /    \\   %4d      | ",joy.y2>>8);
    mvprintw(11,43,"------------|      |------------- ");
    mvprintw(12,43,"     Xbox Controller Status       ");
    pthread_mutex_unlock(&data_mux);
    //mvprintw(22,1,"ax [%d,%d] [%d,%d] [%d] [%d] buttons %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", joy.x1>>7, joy.y1>>7,joy.x2>>7,joy.y2>>7,joy.lt>>7,joy.rt>>7,joy.buts[0],joy.buts[1],joy.buts[2],joy.buts[3],joy.buts[4],joy.buts[5],joy.buts[6],joy.buts[7],joy.buts[8],joy.buts[9],joy.buts[10]);
}

void init_key()
{
    noecho();                  /*  Turn off key echoing                 */
    keypad(mainwin, TRUE);     /*  Enable the keypad for non-char keys  */
}

void init_col()
{
    init_pair(1,  COLOR_RED,     COLOR_BLACK);
    init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
    init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
    init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
    init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
    init_pair(7,  COLOR_BLUE,    COLOR_WHITE);
    init_pair(8,  COLOR_WHITE,   COLOR_RED);
    init_pair(9,  COLOR_BLACK,   COLOR_CYAN);
    init_pair(10, COLOR_BLUE,    COLOR_YELLOW);
    init_pair(11, COLOR_WHITE,   COLOR_BLUE);
    init_pair(12, COLOR_WHITE,   COLOR_CYAN);
    init_pair(13, COLOR_BLACK,   COLOR_CYAN);
}


void* read_cont(void* in)
{
    int  *axis=NULL, num_of_axis=0, num_of_buttons=0, k;
    char *button=NULL, name_of_joystick[80];
    struct js_event js;

    if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
    {
        //printf( "Couldn't open joystick\n" );
        return NULL;
    }

    ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
    ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
    ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

    axis = (int *) calloc( num_of_axis, sizeof( int ) );
    button = (char *) calloc( num_of_buttons, sizeof( char ) );

    //printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n", name_of_joystick, num_of_axis, num_of_buttons );

    fcntl( joy_fd, F_SETFL, O_NONBLOCK );  /* use non-blocking mode */
    while( stop==0 )   /* infinite loop */
    {
        /* read the joystick state */
        read(joy_fd, &js, sizeof(struct js_event));

        /* see what to do with the event */
        pthread_mutex_lock(&data_mux);
        switch (js.type & ~JS_EVENT_INIT)
        {
        case JS_EVENT_AXIS:
            axis   [ js.number ] = js.value;
            break;
        case JS_EVENT_BUTTON:
            button [ js.number ] = js.value;
            break;
        }


        joy.x1=axis[0];
        joy.y1=axis[1];
        joy.x2=axis[2];
        joy.y2=axis[3];
        joy.rt=axis[4];
        joy.lt=axis[5];

        for( k=0 ; k<num_of_buttons ; ++k )
        {
            joy.buts[k]=button[k];
        }
        pthread_mutex_unlock(&data_mux);
        //pthread_mutex_lock(&read_mux);
        //newdat=1;
        //pthread_mutex_unlock(&read_mux);
        fflush(stdout);


    }

    close( joy_fd );   /*too bad we never get here */
}

void* ssl_cli(void* in)
{
    int     err;

    int     verify_client = OFF; /* To verify a client certificate, set ON */
    int     sock;
    struct sockaddr_in server_addr;
    char  *str;
    char    buf [4096];
    char    outbuf[200];

    SSL_CTX         *ctx;
    SSL            *ssl;
    SSL_METHOD      *meth;
    X509            *server_cert;
    EVP_PKEY        *pkey;

    short int       s_port = 5555;
    const char      *s_ipaddr = "127.0.0.1";

    /*----------------------------------------------------------*/
    //printf ("Message to be sent to the SSL server: ");
    //fgets (hello, 80, stdin);

    /* Load encryption & hashing algorithms for the SSL program */
    SSL_library_init();

    /* Load the error strings for SSL & CRYPTO APIs */
    SSL_load_error_strings();

    /* Create an SSL_METHOD structure (choose an SSL/TLS protocol version) */
    meth = SSLv23_method();

    /* Create an SSL_CTX structure */
    ctx = SSL_CTX_new(meth);

    RETURN_NULL(ctx);
    /*----------------------------------------------------------*/
    if(verify_client == ON)

    {

        /* Load the client certificate into the SSL_CTX structure */
        if (SSL_CTX_use_certificate_file(ctx, RSA_CLIENT_CERT,  SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            exit(1);
        }

        /* Load the private-key corresponding to the client certificate */
        if (SSL_CTX_use_PrivateKey_file(ctx, RSA_CLIENT_KEY,
                                        SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            exit(1);
        }

        /* Check if the client certificate and private-key matches */
        if (!SSL_CTX_check_private_key(ctx)) {
            fprintf(stderr,"Private key does not match the certificate public key\n");
            exit(1);
        }
    }

    /* Load the RSA CA certificate into the SSL_CTX structure */
    /* This will allow this client to verify the server's     */
    /* certificate.                                           */

    if (!SSL_CTX_load_verify_locations(ctx, RSA_CLIENT_CA_CERT, NULL)) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* Set flag in context to require peer (server) certificate */
    /* verification */

    SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);

    SSL_CTX_set_verify_depth(ctx,1);
    /* ------------------------------------------------------------- */
    /* Set up a TCP socket */

    sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);

    RETURN_ERR(sock, "socket");

    memset (&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family      = AF_INET;

    server_addr.sin_port        = htons(s_port);       /* Server Port number */

    server_addr.sin_addr.s_addr = inet_addr(s_ipaddr); /* Server IP */

    /* Establish a TCP/IP connection to the SSL client */

    err = connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr));

    RETURN_ERR(err, "connect");
    /* ----------------------------------------------- */
    /* An SSL structure is created */

    ssl = SSL_new (ctx);

    RETURN_NULL(ssl);

    /* Assign the socket into the SSL structure (SSL and socket without BIO) */
    SSL_set_fd(ssl, sock);

    /* Perform SSL Handshake on the SSL client */
    err = SSL_connect(ssl);

    RETURN_SSL(err);

    /* Informational output (optional) */
    //printf ("SSL connection using %s\n", SSL_get_cipher (ssl));

    /* Get the server's certificate (optional) */
    server_cert = SSL_get_peer_certificate (ssl);

    if (server_cert != NULL)
    {
        //printf ("Server certificate:\n");

        str = X509_NAME_oneline(X509_get_subject_name(server_cert),0,0);
        RETURN_NULL(str);
        //printf ("\t subject: %s\n", str);
        free (str);

        str = X509_NAME_oneline(X509_get_issuer_name(server_cert),0,0);
        RETURN_NULL(str);
        //printf ("\t issuer: %s\n", str);
        free(str);

        X509_free (server_cert);

    }
    else
        printf("The SSL server does not have certificate.\n");

    /*-------- DATA EXCHANGE - send message and receive reply. -------*/
    /* Send data to the SSL server */
    int trans=10;
    while(stop==0 )
    {
        pthread_mutex_lock(&data_mux);
        sprintf(outbuf,"cmd %d ax [%d,%d] [%d,%d] [%d] [%d] buttons %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",!stop, joy.x1>>7, joy.y1>>7,joy.x2>>7,joy.y2>>7,joy.lt>>7,joy.rt>>7,joy.buts[0],joy.buts[1],joy.buts[2],joy.buts[3],joy.buts[4],joy.buts[5],joy.buts[6],joy.buts[7],joy.buts[8],joy.buts[9],joy.buts[10]);
        pthread_mutex_unlock(&data_mux);
        //sprintf(outbuf,"cmd %d ax [%d,%d] [%d,%d] [%d] [%d] buttons %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",trans,1, 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17);
        err = SSL_write(ssl, outbuf, strlen(outbuf));

        RETURN_SSL(err);

        /* Receive data from the SSL server */
        err = SSL_read(ssl, buf, sizeof(buf)-1);

        RETURN_SSL(err);
        buf[err] = '\0';
        //printf ("Received %d chars:'%s'\n", err, buf);
        //trans--;
    }
    /*--------------- SSL closure ---------------*/
    /* Shutdown the client side of the SSL connection */

    err = SSL_shutdown(ssl);
    RETURN_SSL(err);

    /* Terminate communication on a socket */
    err = close(sock);

    RETURN_ERR(err, "close");

    /* Free the SSL structure */
    SSL_free(ssl);

    /* Free the SSL_CTX structure */
    SSL_CTX_free(ctx);
}


