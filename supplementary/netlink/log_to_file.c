#include "iwl_connector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<errno.h>
#include<time.h>

#define MAX_PAYLOAD 2048
#define SLOW_MSG_CNT 1

int sock_fd = -1;							
FILE* out = NULL;
typedef struct _MSG
{
    char path[64];
    int count;
}MSG;
int    shmid;
int    ret;
MSG *p = NULL;

void check_usage(int argc, char** argv);
FILE* open_file(char* filename, char* spec);
void caught_signal(int sig);
void exit_program(int code);
void exit_program_err(int code, char* func);

int main(int argc, char** argv)
{
	/* Local variables */
	struct sockaddr_nl proc_addr, kern_addr;	// addrs for recv, send, bind
	struct cn_msg *cmsg;
	char buf[4096];
	unsigned short l, l2;
	int count = 0;
    shmid = shmget(0x5120, 0, 0);
    if (shmid == -1)
    {
        perror("you must open or restart camera program firstly !!!\nipcs");
        return errno;
    }
    p = shmat(shmid, NULL, 0);
    if (p == (void *)-1 )
    {
        perror("shmget err");
        return errno;
    }

	/* Make sure usage is correct */
	check_usage(argc, argv);

	/* Open and check log file */
	out = open_file(argv[1], "w");
	strcpy(p->path, argv[1]);

	/* Setup the socket */
	sock_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (sock_fd == -1)
		exit_program_err(-1, "socket");

	/* Initialize the address structs */
	memset(&proc_addr, 0, sizeof(struct sockaddr_nl));
	proc_addr.nl_family = AF_NETLINK;
	proc_addr.nl_pid = getpid();			// this process' PID
	proc_addr.nl_groups = CN_IDX_IWLAGN;
	memset(&kern_addr, 0, sizeof(struct sockaddr_nl));
	kern_addr.nl_family = AF_NETLINK;
	kern_addr.nl_pid = 0;					// kernel
	kern_addr.nl_groups = CN_IDX_IWLAGN;

	/* Now bind the socket */
	if (bind(sock_fd, (struct sockaddr *)&proc_addr, sizeof(struct sockaddr_nl)) == -1)
		exit_program_err(-1, "bind");

	/* And subscribe to netlink group */
	{
		int on = proc_addr.nl_groups;
		ret = setsockopt(sock_fd, 270, NETLINK_ADD_MEMBERSHIP, &on, sizeof(on));
		if (ret)
			exit_program_err(-1, "setsockopt");
	}

	/* Set up the "caught_signal" function as this program's sig handler */
	signal(SIGINT, caught_signal);

	/* Poll socket forever waiting for a message */
	while (1)
	{
		/* Receive from socket with infinite timeout */
		ret = recv(sock_fd, buf, sizeof(buf), 0);
		if (ret == -1)
			exit_program_err(-1, "recv");
		/* Pull out the message portion and print some stats */
		cmsg = NLMSG_DATA(buf);
		if (count % SLOW_MSG_CNT == 0)
			printf("received %d bytes: id: %d val: %d seq: %d clen: %d\n", cmsg->len, cmsg->id.idx, cmsg->id.val, cmsg->seq, cmsg->len);
		/* Log the data to file */
		l = (unsigned short) cmsg->len;
		l2 = htons(l);
		fwrite(&l2, 1, sizeof(unsigned short), out);
		ret = fwrite(cmsg->data, 1, l, out);
		if (count % 100 == 0) printf("wrote %d bytes [msgcnt=%u]\n", ret, count);
		p->count = count;
		++count;
		if (ret != l)
			exit_program_err(1, "fwrite");
	}

	exit_program(0);
	return 0;
}

void check_usage(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
		exit_program(1);
	}
}

FILE* open_file(char* filename, char* spec)
{
	FILE* fp = fopen(filename, spec);
	if (!fp)
	{
		perror("fopen");
		exit_program(1);
	}
	return fp;
}

void caught_signal(int sig)
{
	fprintf(stderr, "Caught signal %d\n", sig);
	exit_program(0);
}

void exit_program(int code)
{
	if (out)
	{
		fclose(out);
		out = NULL;
	}
	if (sock_fd != -1)
	{
		close(sock_fd);
		sock_fd = -1;
	}
	shmdt(p);   
    printf("\nunlock thread\n");
	ret=shmctl(shmid, IPC_RMID, NULL);
    if (ret < 0)
    {
       perror("error in delete ram\n");
    }
	exit(code);
}

void exit_program_err(int code, char* func)
{
	shmdt(p);   
    printf("\nunlock thread\n");
	ret=shmctl(shmid, IPC_RMID, NULL);
    if (ret < 0)
    {
       perror("error in delete ram\n");
    }

	perror(func);
	exit_program(code);
}

