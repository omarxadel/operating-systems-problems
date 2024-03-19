#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int counter, random_sleep, buffer_in, buffer_out, inserted, t1, BUFF_SIZE, NUMBER_OF_THREADS, b, full, empty, count;
int *buffer;

void *Collector(void *arg)
{
    while (buffer_out != BUFF_SIZE)
    {
        random_sleep = rand() % 5;
        sleep(random_sleep);
        if (inserted == 0)
        {
            printf("Collector thread: nothing is in the buffer!\n");
        }
        else
        {
            sem_wait(&b);
            sem_wait(&full);
            printf("Collector thread: reading from buffer at position %d value of %d\n",
                   buffer_out, buffer[buffer_out]);
            buffer_out++;
            sem_post(&empty);
            sem_post(&b);
        }
    }
    exit(1);
}
void *Counter(void *arg)
{
    int thread = (int)arg;
    while (1)
    {
        random_sleep = rand() % 5;
        printf("Counter thread%d: recieved a message\n", thread);
        sem_wait(&count);
        printf("Counter thread%d: waiting to write\n", thread);
        counter++;
        printf("Counter thread%d: now adding to counter, counter value=%d\n", thread, counter);
        sem_post(&count);
        sleep(random_sleep);
    }
}
void *Monitor(void *arg)
{
    int current_counter;
    while (1)
    {
        random_sleep = rand() % 5;
        sleep(t1);
        printf("Monitor thread: waiting to read counter\n");
        sem_wait(&count);
        current_counter = counter;
        counter = 0;
        printf("Monitor thread: reading a count value of %d\n", current_counter);
        sem_post(&count);
        sem_wait(&empty);
        if (buffer_in == BUFF_SIZE)
        {
            printf("Monitor thread: Buffer full!!\n");
        }
        else
        {
            sem_wait(&b);
            printf("Monitor thread: writing to buffer at position %d\n", buffer_in);
            buffer[buffer_in] = current_counter;
            buffer_in++;
            inserted++;
            sem_post(&b);
        }
        sem_post(&full);
    }
}

void init()
{
    counter = 0;
    random_sleep = rand() % 5;
    buffer_in = 0;
    buffer_out = 0;
    inserted = 0;
    t1 = 1;
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFF_SIZE);
    sem_init(&count, 0, 1);
    sem_init(&b, 0, 1);
    buffer = (int *)malloc(BUFF_SIZE * sizeof(int));
}

void start()
{
    init();
    pthread_t mmonitor, mcollector;
    pthread_t mcounter[NUMBER_OF_THREADS];
    if (pthread_create(&mmonitor, NULL, Monitor, NULL) != 0)
    {
        puts("ERROR IN THREADS");
        exit(1);
    }
    if (pthread_create(&mcollector, NULL, Collector, NULL) != 0)
    {
        puts("ERROR IN THREADS");
        exit(1);
    }
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        if (pthread_create(&mcounter[i], NULL, Counter, i) != 0)
        {
            puts("ERROR IN THREADS");
            exit(1);
        }
    }
    pthread_join(mmonitor, NULL);
    pthread_join(mcollector, NULL);
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(mcounter[i], NULL);
    }
}

int main()
{
    NUMBER_OF_THREADS = 3;
    BUFF_SIZE = 5;
    start();
}