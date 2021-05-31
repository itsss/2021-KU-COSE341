#include "message_buffer_semaphore.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    MessageBuffer *buffer;
    Message *msg = malloc(sizeof(Message));

    init_buffer(&buffer);

    while (1) {
        if (consume(&buffer, &msg) == 0) {
            printf("%d : %s\n", msg->sender_id, msg->data);
        }
    }
    return 0;
}
