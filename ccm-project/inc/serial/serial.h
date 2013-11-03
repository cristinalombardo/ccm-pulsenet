/*
 * serial.h
 *
 *  Created on: 02/nov/2013
 *      Author: cristina
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>

#define DEFAULT_SERIAL_FILE "/dev/ttyO1"

/**
 * Serial struct. This struct encapsulate all serial configuration info.
 */
typedef struct SERIAL_CONFIGURATION{
	const char* file;		/**< Filename eg. /dev/ttyO1 */
	int file_descriptor;	/**< identify the serial file opened */
	int is_init;			/**< 0: tty is not initialized; 1: tty is initialized */
	int baudrate;			/**< Serial baudrate eg. 9600 bps */
} serial_configuration_t;

/*! \brief Initialize serial port
 *  \param [in] force_init force initialization. If UART is already initialized reinit the port.
 *	\param [in,out] serial_port serial port to configure
 *	\return true if success
 */
int serial_conf(int force_init, serial_configuration_t* serial_port);

/*! \brief Write a buffer into a serial port
 *  \param [in] buff the buffer to write.
 *  \param [in] buff_length buffer length.
 *	\param [in] serial_port serial port in which write data
 *	\return true if success
 */
int serial_write(char *buff, int buff_length, serial_configuration_t* serial_port);

/*! \brief Read a buffer from serial port
 *  \param [out] buff the buffer in which save date.
 *  \param [in] buff_length max buffer length.
 *	\param [in] serial_port serial port in which read data
 *	\return the number of char read
 */
int serial_read(char* buff, int buff_length, serial_configuration_t* serial_port);

/*! \brief Close a serial port
 *  \param [in,out] serial_port serial port to close.
 *	\return true if success
 */
int serial_close(serial_configuration_t* serial_port);

#endif /* SERIAL_H_ */
