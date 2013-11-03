/*
 * serial.c
 *
 *  Created on: 02/nov/2013
 *      Author: cristina
 */

#include <serial/serial.h>

//------ SERIAL DEVICE ------
struct termios actual_conf;
/*
 *
 */
int serial_conf(int force_init, serial_configuration_t* serial_port) {
	if(!serial_port -> is_init || force_init){

		char tty_file[30];
		int br;

		// auto detect TTY device
		// system("dmesg | grep tty | grep 0x3f8");
		// ...parse output into a pipe
		printf("Initializing TTY device on\n");
		if ( serial_port -> file != NULL) {
			strcpy(tty_file, serial_port -> file);
		} else {
			strcpy(tty_file, DEFAULT_SERIAL_FILE);
		}
		printf("%s... ", tty_file);

		serial_port -> file_descriptor = open(tty_file, O_RDWR | O_NOCTTY );	// O_RDWR | O_NOCTTY | O_NDELAY
		if (serial_port -> file_descriptor < 0) {
			fprintf(stderr, "Cannot open %s\n", tty_file);
			return(EXIT_FAILURE);
		}

		if (!isatty(serial_port -> file_descriptor)) {
			fprintf(stderr,"Non TTY device error\n");
			return(EXIT_FAILURE);
		}

		/* GET CURRENT PORT SETTINGS */
		if (tcgetattr(serial_port -> file_descriptor, &actual_conf ) < 0) {
			fprintf(stderr,"Cannot retrieve actual TTY configuration\n");
			return(EXIT_FAILURE);
		}

		// SET IO BAUD RATE
		switch ( serial_port -> baudrate ) {
			case 9600:  	br = B9600;  	break;
			case 19200: 	br = B19200; 	break;
			case 38400: 	br = B38400; 	break;
			case 57600: 	br = B57600; 	break;
			case 115200:	br = B115200;	break;
			default:    	br = B9600;
		}

		cfsetispeed(&actual_conf, br); 	/* Set input speed at br value */
		cfsetospeed(&actual_conf, br);	/* Set output speed at br value */

		// ENABLE RECEIVER AND SET LOCAL MODE
		actual_conf.c_cflag |= (CLOCAL | CREAD);

		// SET 8N1 Parity Check (NO PARITY)
		actual_conf.c_cflag &= ~PARENB;
		actual_conf.c_cflag &= ~CSTOPB;
		actual_conf.c_cflag &= ~CSIZE;
		actual_conf.c_cflag |= CS8;

		// NO HW FLOW CONTROL
		actual_conf.c_cflag &= ~CRTSCTS;

		// RAW MODE
		actual_conf.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

		// NO SW FLOW CONTROL
		actual_conf.c_iflag &= ~(IXON | IXOFF | IXANY);

		// RAW OUTPUT
		actual_conf.c_oflag &= ~OPOST;

		// TIMEOUT 1 sec
		actual_conf.c_cc[VMIN]  = 1;
		actual_conf.c_cc[VTIME] = 1;

		// Finally, apply the configuration (TCSFLUSH)
		if (tcsetattr(serial_port -> file_descriptor, TCSANOW, &actual_conf) < 0) {
			fprintf(stderr,"Cannot apply the configuration to TTY device\n");
			return(EXIT_FAILURE);
		};

		printf("done %d \n", serial_port -> file_descriptor);


		serial_port -> is_init = true;
	}
	return(EXIT_SUCCESS);
}

int serial_write(char *buff, int buff_length, serial_configuration_t* serial_port) {
	serial_conf(0, serial_port);
	write(serial_port -> file_descriptor, buff, buff_length);
	return(EXIT_SUCCESS);
}

int serial_read(char* buff, int buff_length, serial_configuration_t* serial_port) {
	serial_conf(0, serial_port);
	ssize_t res = 0;
	res = read(serial_port -> file_descriptor, buff, buff_length);
	return res;
}

int serial_close(serial_configuration_t* serial_port) {
	close(serial_port -> file_descriptor);
	return(EXIT_SUCCESS);
}
