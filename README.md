# Socket and thread

UDP Socket and thread with conditional mutex.

Producer receives data from UDP socket and put into circular buffer.

Consumer waits for mutex to pop the data from the circular buffer.

This implementation avoids a busy-wait loop.


Reference: https://linux.die.net/man/3/pthread_cond_init

## Producer
1. message_received_thread_handler() will block on udp_receive
2. When a message is received, it will be inserted into a circular buffer
3. Will also call buffer_set_ready(1) to sinalize data is available in buffer

## Consumer
1. message_handler_thread_handler() will wait on buffer_wait_ready()
2. buffer_wait_ready() will lock a mutex
3. buffer_wait_ready() will wait on contidion to unlock mutex
	3.1 the condition is that the buffer is not empty
	3.2 this wait does not consume resources as a while(1) loop
4. lastely, message_handler_thread_handler() will call buffer_set_ready(0)

### Hint
To check CPU utilization, run `watch -n 0.1  ps -C socket_and_thread -o %cpu,%mem`

The CPU usage of a naive implementation (using poll-loops) was about 12%.
The CPU usage using the conditional mutex is about 1%.

