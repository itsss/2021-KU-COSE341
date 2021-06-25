#define KEY 54321
#define BUFFER_SIZE 10

typedef struct {
    int sender_id;
    char data[101];
} Message;

typedef struct {
    Message messages[BUFFER_SIZE];
    int in;
    int out;
} MessageBuffer;

int init_buffer(MessageBuffer **buffer);
int attach_buffer(MessageBuffer **buffer);
int detach_buffer();
int destroy_buffer();
int produce(MessageBuffer **buffer, int sender_id, char *data);
int consume(MessageBuffer **buffer, Message **message);
int is_empty(MessageBuffer buffer);
int is_full(MessageBuffer buffer);