#include "Gpio.h"
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "Log.h"


int GPIO::Export(int pin)
{
	char buffer[BUFFER_MAX];  
    int len;  
    int fd;  
    fd = open("/sys/class/gpio/export", O_WRONLY);  
    if (fd < 0) {  
        //fprintf(stderr, "Failed to open export for writing!\n");  
        log_error("Failed to open export for writing! pin=%d errno=%s\n", pin,strerror(errno));
        return(-1);  
    }  
  
    len = snprintf(buffer, BUFFER_MAX, "%d", pin);  
    if (write(fd, buffer, len) < 0) {  
        //fprintf(stderr, "Fail to export gpio!");
        log_error("Fail to export gpio! pin=%d errno=%s\n", pin,strerror(errno));  
        return -1;  
    }  
     
    close(fd);  
    return 0;  
}

int GPIO::Unexport(int pin)
{
	char buffer[BUFFER_MAX];  
    int len;  
    int fd;  
  
    fd = open("/sys/class/gpio/unexport", O_WRONLY);  
    if (fd < 0) {  
        //fprintf(stderr, "Failed to open unexport for writing!\n"); 
        log_error("Failed to open unexport for writing! pin=%d errno=%s\n", pin,strerror(errno)); 
        return -1;  
    }  
  
    len = snprintf(buffer, BUFFER_MAX, "%d", pin);  
    if (write(fd, buffer, len) < 0) {  
        //fprintf(stderr, "Fail to unexport gpio!"); 
        log_error("Fail to unexport gpio! pin=%d errno=%s\n", pin,strerror(errno));  
        return -1;  
    }  
     
    close(fd);  
    return 0;  
}

int GPIO::Direction(int pin, int dir)
{
	static const char dir_str[] = "in\0out";
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        //fprintf(stderr, "failed to open gpio direction for writing!\n");
        log_error("failed to open gpio direction for writing! dir=%d pin=%d errno=%s\n", dir, pin, strerror(errno));
        return -1;
    }

    if (write(fd, &dir_str[dir == IN ? 0 : 3], dir == IN ? 2 : 3) < 0) {
        //fprintf(stderr, "failed to set direction!\n");
        log_error("failed to set direction! dir=%d pin=%d errno=%s\n", dir, pin, strerror(errno));
        return -1;
    }

    close(fd);
    return 0;
}

int GPIO::Write(int pin, int value)
{
	static const char values_str[] = "01";  
    char path[DIRECTION_MAX];  
    int fd;  
    
    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/value", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) {  
        //fprintf(stderr, "failed to open gpio value for writing!\n");  
        log_error("failed to open gpio value for writing! value=%d pin=%d errno=%s\n", value, pin, strerror(errno));
        return -1;  
    }  
  
    if (write(fd, &values_str[value == LOW ? 0 : 1], 1) < 0) {  
        //fprintf(stderr, "failed to write value!\n");  
        log_error("failed to write value! value=%d pin=%d errno=%s\n", value, pin, strerror(errno));
        return -1;  
    }  
  
    close(fd);  
    return 0;
}

int GPIO::Read(int pin)
{
    char path[DIRECTION_MAX];  
    char value_str[3] = {0};  
    int fd;  
  
    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/value", pin);  
    fd = open(path, O_RDONLY);  
    if (fd < 0) {  
        //fprintf(stderr, "failed to open gpio value for reading!\n");  
        log_error("failed to open gpio value for reading! pin=%d errno=%s\n", pin, strerror(errno));
        return -1;  
    }  
  
    if (read(fd, value_str, 3) < 0) {  
        //fprintf(stderr, "failed to read value!\n");  
        log_error("failed to read value! pin=%d errno=%s\n", pin, strerror(errno));
        return -1;  
    }  
  
    close(fd);  
    return (atoi(value_str));
}

int GPIO::Edge(int pin, char * mode)
{
	char path[DIRECTION_MAX];
    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/edge", pin);
	int edgeFd = open(path, O_WRONLY);  
    if (edgeFd < 0) 
	{  
        printf("failed to open edge errno=%s\n", strerror(errno));
        return -1;  
    }

	if (write(edgeFd, mode, strlen(mode)) < 0) 
	{  
        printf("Fail to write gpio edge errno=%s\n", strerror(errno));  
        return -1;  
    }
	close(edgeFd);
    return 0;
}

