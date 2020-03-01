# Socket and thread

UDP Socket and thread with conditional mutex.

Producer receives data from UDP socket and put into circular buffer.

Consumer waits for mutex to pop the data from the circular buffer.

This implementation avoids a busy-wait loop.

Reference: https://linux.die.net/man/3/pthread_cond_init
