#include <thread.h>
#include <mutex.h>
#include <xtimer.h>

#define LOW_PRIO  14
#define MED_PRIO  6
#define HIGH_PRIO 1

static mutex_t mutex;

static char med_prio_thread_stack[THREAD_STACKSIZE_DEFAULT];

void* med_prio_thread(void* arg) {
    (void)arg;
    printf("Medium priority thread start\r\n");
    xtimer_sleep(1);
    printf("Medium priority thread sleep\r\n");
    return 0;
}

static char high_prio_thread_stack[THREAD_STACKSIZE_DEFAULT];

void* high_prio_thread(void* arg) {
    (void)arg;
    while (1) {
        printf("High priority thread start\r\n");
        mutex_lock(&mutex);
        printf("High priority thread inside critical section\r\n");
		for (int i = 0; i < 5; i++) {
			xtimer_sleep(1);
			printf("High priority thread work %d\r\n", i);
		}
        printf("High priority thread finished critical section\r\n");
        mutex_unlock(&mutex);
		printf("High priority thread sleep\r\n");
        thread_sleep();
    }
}

static char low_prio_thread_stack[THREAD_STACKSIZE_DEFAULT];

void* low_prio_thread(void* arg) {
    (void)arg;
    while (1) {
        printf("Low priority thread start\r\n");
        mutex_lock(&mutex);
        printf("Create high priority thread from low_prio_thread inside critical section\r\n");
        thread_create(high_prio_thread_stack, sizeof(high_prio_thread_stack), HIGH_PRIO, THREAD_CREATE_STACKTEST, high_prio_thread, NULL, "High");
        printf("Create medium priority thread from low_prio_thread inside critical section\r\n");
        thread_create(med_prio_thread_stack, sizeof(med_prio_thread_stack), MED_PRIO, THREAD_CREATE_STACKTEST, med_prio_thread, NULL, "Medium");
        printf("Low priority thread inside critical section\r\n");
        for (int i = 0; i < 5; i++) {
            xtimer_sleep(1);
            printf("Low priority thread work %d\r\n", i);
        }
        printf("Low priority thread finished critical section\r\n");
        mutex_unlock(&mutex);
        printf("Low priority thread sleep\r\n");
        thread_sleep();
    }
}

int main(void) {
    mutex_init(&mutex);
    thread_create(low_prio_thread_stack, sizeof(low_prio_thread_stack), LOW_PRIO, THREAD_CREATE_STACKTEST, low_prio_thread, NULL, "Low");
	
	thread_sleep();
    
	return 0;
}