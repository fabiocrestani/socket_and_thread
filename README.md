# Visual CAN Send Receive

This simple application sends and receives CAN messages and has a graphical representation of the  received messages.

* DUT 1 sends messages with CAN ID 0x01
* DUT 2 sends messages with CAN ID 0x02


When CAN is not available, the can.c will call udp.c to send the messages over UDP. This is also useful for local debugging.


## Compilation
`make`

## Usage
~~~bash
./can_visual_send_receive [dut] [period] [l]
~~~

* [dut] is the current DUT (1 or 2)
* [period] is the period in milliseconds to send messages
* [l] is an optional parameter to set the direction of the graph to the left side of the terminal

### Example

On DUT 1 run:

~~~bash
./can_visual_send_receive 1 100
~~~

On DUT 2 run:

```bash
./can_visual_send_receive 2 100 l
```

## Hint

For real CAN communication, replace the following interfaces with your CAN
send/write library:

~~~c
uint8_t can_send(uint32_t can_id, unsigned char data[], uint32_t len);
uint8_t can_receive(uint32_t *can_id, unsigned char data[], uint32_t *len);
~~~

