#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <stdio.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
using namespace std;
cv::VideoCapture *camera_ptr=NULL;
char id_str[64];
char file_str[64];
char cmd_str[64];
int save_count = 0;
int ret = 0;
int shmid;
int count_last;
int frame_count = 0;
int timeout_count=0;
int camera_id=0;
int save_interval=0;
int auto_exit=0;
bool path_initial = false;

typedef struct _MSG
{
	char path[64];
	int  count;
}MSG;

void exit_ex()
{
	camera_ptr->release();
	exit(0);
}

void sig_int(int signo)
{
	exit_ex();
}

void check_usage(int argc, char** argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s <Camera ID> <Pic_Save_Interval> <Whether Auto Exit>\n", argv[0]);
		exit(0);
	}
}

int main(int argc, char** argv)
{
	check_usage(argc, argv);
	sscanf(argv[1], "%u", &camera_id);
	sscanf(argv[2], "%u", &save_interval);
	sscanf(argv[3], "%u", &auto_exit);
	signal(SIGINT, sig_int);
	shmid = shmget(0x5120, sizeof(MSG), IPC_CREAT | 0666);
	if (shmid == -1)
	{
		perror("ipc err 1");
		return errno;
	}
	void *shm = shmat(shmid, NULL, 0);
	MSG *p = (MSG*)shm;
	if (p == (void *)-1)
	{
		perror("ipc err 2");
		return errno;
	}
	cv::VideoCapture camera(camera_id);
	camera_ptr=&camera;
	int frame_width = camera_ptr->get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = camera_ptr->get(CV_CAP_PROP_FRAME_HEIGHT);
	camera_ptr->set(CV_CAP_PROP_FRAME_WIDTH, frame_width);
	camera_ptr->set(CV_CAP_PROP_FRAME_HEIGHT, frame_height);
	if (!camera_ptr->isOpened())
	{
		cout << "Error opening video stream" << endl;
		return 1;
	}
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 1, 1, 0, 1, 4);
	while (1)
	{
		cv::Mat3b frame0;
		if (camera_ptr->read(frame0))
		{
			IplImage img = IplImage(frame0);
			if(p->count!=0) sprintf(id_str, "%s%d", "No:", p->count);
			cvPutText(&img, id_str, cvPoint(10, 30), &font, CV_RGB(255, 0, 153));
			frame_count++;
			if (p->count != 0 && frame_count > save_interval)
			{
				if (!path_initial)
				{
					sprintf(cmd_str, "%s%s%s", "mkdir ", "pic_",p->path);
					if (system(cmd_str)) exit_ex();
					path_initial = true;
				}
				sprintf(file_str, "%s%s%c%d%s", "pic_",p->path, '/', save_count++, ".jpg");
				frame_count = 0;
				if (!cvSaveImage(file_str, &img))
				{
					cout << "file path error " << endl;
					exit_ex();
				}
			    if (count_last == p->count)
			    {
					timeout_count++;
					if((timeout_count*(save_interval+1)>30)&&auto_exit) {cout <<"Timeout Auto Exit"<< endl; exit_ex();}
				}
			    count_last = p->count;
			}
			cv::imshow("IEEE 802.11n CSI Camera Synchronization Toolkit", frame0);
			int c = cvWaitKey(20);
		}
	}
	return 0;
}
