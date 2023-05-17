#include <thread.h>
#include <mutex.h>
#include <xtimer.h>

#define LOW_PRIO  14
#define MED_PRIO  6
#define HIGH_PRIO 1

static mutex_t mutex;

static char low_prio_thread_stack[THREAD_STACKSIZE_DEFAULT];

void* low_prio_thread(void* arg) {
    (void)arg;
    while (1) {
        printf("Low priority thread\n");
        mutex_lock(&mutex);
        printf("Low priority thread inside critical section\n");
		for (int i = 0; i < 5; i++) {
			xtimer_sleep(1);
			printf("Low priority thread work %d\n", i);
		}
		printf("Low priority thread finished critical section\n");
        mutex_unlock(&mutex);
		printf("Low priority thread yield\n");
        thread_yield();
    }
}

static char med_prio_thread_stack[THREAD_STACKSIZE_DEFAULT];

void* med_prio_thread(void* arg) {
    (void)arg;
    while (1) {
        printf("Medium priority thread\n");
        thread_yield();
    }
}

static char high_prio_thread_stack[THREAD_STACKSIZE_DEFAULT];

void* high_prio_thread(void* arg) {
    (void)arg;
    while (1) {
        printf("High priority thread\n");
        mutex_lock(&mutex);
        printf("High priority thread inside critical section\n");
		for (int i = 0; i < 5; i++) {
			xtimer_sleep(1);
			printf("High priority thread work %d\n", i);
		}
        printf("High priority thread finished critical section\n");
        mutex_unlock(&mutex);
		printf("High priority thread yield\n");
        thread_yield();
    }
}

int main(void) {
    mutex_init(&mutex);
    thread_create(low_prio_thread_stack, sizeof(low_prio_thread_stack), LOW_PRIO, 0, low_prio_thread, NULL, "Low");
    thread_create(med_prio_thread_stack, sizeof(med_prio_thread_stack), MED_PRIO, 0, med_prio_thread, NULL, "Medium");
    thread_create(high_prio_thread_stack, sizeof(high_prio_thread_stack), HIGH_PRIO, 0, high_prio_thread, NULL, "High");
	
	thread_yield();
	return 0;
	//return thread_yield();
}