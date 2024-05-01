#ifndef SERIAL_PORT_HEADER
#define SERIAL_PORT_HEADER

#include <stdint.h>


struct _SerialPort;
typedef struct _SerialPort SerialPort;

extern SerialPort USART1_PORT;

enum {
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200
};

void oneshot_callback(void);

void timer_callback(void);

void finished_transmission(uint32_t bytes_sent);

void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t), void (*received_string_function)(uint8_t *, uint32_t));

void SerialOutputChar(uint8_t, SerialPort *serial_port);

void SerialOutputString(uint8_t *pt, SerialPort *serial_port);

void run_serial_test();

#endif
