#include <sys.stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN 0
#define LOW 0
#define OUT 1
#define HIGH 1
#define BTN_PIN 20
#define BTN_POUT2 21
#define LED_POUT 17
#define VALUE_MAX 40

static int GPIOExport(int pin){
#define BUFFER_MAX 3
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd){
        fprintf(stderr, "Failed to open export for writing!\n");
        return -1;
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return 0;
}

static int GPIOUnexport(int pin){
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd){
        fprintf(stderr, "Failed to open unexport for writing!\n");
        return -1;
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return 0;
}

static int GPIODirection(int pin, int dir){
    static const char s_directions_str[] = "in\0out";

#define DIRECTION_MAX 35
    char path[DIRECTION_MAX] = "/sys/class/gpio/gpio%d/direction";
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);

    fd = open(path, O_WRONLY);
    if (-1 == fd){
        fprintf(stderr, "Failed to open gpio direction for writing!\n");
        return -1;
    }

    if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)){
        fprintf(stderr, "Failed to set direction!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

static int GPIORead_btn(int pin){
    char path[VALUE_MAX];
    char value_str[3];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (-1 == fd){
        fprintf(stderr, "Failed to open gpio value for reading!\n");
        return -1;
    }

    if (-1 == read(fd, value_str, 3)){
        fprintf(stderr, "Failed to read value!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return atoi(value_str);
}

static int GPIOWrite_LED(int pin, int value){
    static const char s_values_str[] = "01";

    char path[VALUE_MAX];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd){
        fprintf(stderr, "Failed to open gpio value for writing!\n");
        return -1;
    }

    if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)){
        fprintf(stderr, "Failed to write value!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int main(int argc, char *argv[]){
    int repeat = 1000;
    int state = 1;
    int prev_state = 1;
    int light = 1;

    if (-1 == GPIOExport(BTN_POUT2) || -1 == GPIOExport(BTN_PIN) || -1 == GPIOExport(LED_POUT))
        return 1;

    if (-1 == GPIODirection(BTN_POUT2, OUT) || -1 == GPIODirection(BTN_PIN, IN) || -1 == GPIODirection(LED_POUT))
        return 2;

    do {
        if (-1 == GPIOWrite(BTN_POUT2, 1) || -1 == GPIOWrite(LED_POUT, BTN_POUT2 || -1 == GPIOWrite(LED_POUT, BTN_PIN)))
            return 3;

            printf("GPIORead : %d from pin %d\n", GPIORead(BTN_PIN), BTN_PIN);
            usleep(100000);
     }
     while (repeat--);


    if (-1 == GPIOUnexport(BTN_POUT2) || -1 == GPIOUnexport(BTN_PIN) || -1 == GPIOUnexport(LED_POUT))
            return 4;

    return 0;
}
