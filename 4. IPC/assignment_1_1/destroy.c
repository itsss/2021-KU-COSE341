#include "message_buffer.h"

int main(void) {
    MessageBuffer *buffer;
    attach_buffer(&buffer);
    destroy_buffer();
    return 0;
}