#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_TIMINGS 85

static int PHTPIN = 11;
static int dht22_dat[5] = {0};

static unit8_t sizecvt(const int read){
    if (read > 255 || read < 0){
        fprintf(stderr, "Invalid data from wingPi library\n");
        exit(EXIT_FAILURE);
    }

    return (unit8_t)read;
}

static int read_dht22_dat(){
    unit8_t laststate = HIGH;
    unit8_t counter = 0;
    unit8_t j = 0, i;

    for (i = 0; i < 5; i++)
        dht22_dat[i] = 0;

    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, HIGH);
    delay(10);
    digialWrite(DHTPIN, LOW);
    delay(10);

    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);

    pinMode(DHTPIN, INPUT);

    for (i = 0; i < MAXTIMINGS; i++){
        counter = 0;
        while (sizecvt(digitalRead(DHTPIN)) == laststate){
            counter++;
            delayMicroseconds(1);
            if (counter == 255)
                break;
        }

        laststate = sizecvt(digitalRead(DHTPIN));

        if (counter == 255)
            break;

        if ((i >= 4) && !(i%2)){
            dht22_dat[j/8] <<= 1;
            if (counter > 16)
                dht22_dat[j/8] |= 1;

            j++;
        }
    }

    if ((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dat22_dat[3]) & 0xFF))){
        float t, h;
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10.0;
        t = (float)(dht22_dat[2] & 0x7F) * 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)
            t *= -1;

        printf("Humidity = %.2f %% Temperature =%.2f *C \n", h, t);
        return 1;
    }

    else{
        printf("Data not good, skip\n");
        return 0;
    }
}

int main(void){
    if (wiringPiSetup() == -1)
        exit(EXIT_FAILURE);

    if (setuid(getuid()) < 0)
        perror("Dropping priviliges failed\n");

    while (1){
        while (read_dht22_dat() == 0)
            delay(1000);

        delay(3000);
    }

    return 0;
}
