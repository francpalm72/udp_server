/*
    Simple udp server
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//per la I2C
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

//per la SPI
#include <getopt.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))






#include "udp_server.h"


#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

	
	
// I2C Linux device handle
int g_i2cFile;

// open the Linux device
void i2cOpen()
{
	g_i2cFile = open("/dev/i2c-2", O_RDWR);
	if (g_i2cFile < 0) {
		perror("i2c2 Open");
		exit(1);
	}
}

// close the Linux device
void i2cClose()
{
	close(g_i2cFile);
}

// set the I2C slave address for all subsequent I2C device transfers
void i2cSetAddress(int address)
{
	if (ioctl(g_i2cFile, I2C_SLAVE, address) < 0) {
		perror("i2cSetAddress");
		exit(1);
	}
}

// write a 16 bit value to a register pair
// write low byte of value to register reg,
// and high byte of value to register reg+1
void i2cWriteRegisterPair(uint8_t reg, uint16_t value)
{
	uint8_t data[3];
	data[0] = reg;
	data[1] = (value >> 8) & 0xff;
	data[2] = value & 0xff;
	
	if (write(g_i2cFile, data, 3) != 3) {
		perror("SetRegisterPair");
	}
}

// read a 16 bit value from a register pair
uint16_t i2cReadRegisterPair(uint8_t reg)
{
	uint8_t data[3];
	data[0] = reg;
	if (write(g_i2cFile, data, 1) != 1) {
		perror("ReadRegisterPair set register");
	}
	if (read(g_i2cFile, data, 2) != 2) {
		perror("ReadRegisterPair read value");
	}
	return data[1] | (data[0] << 8);
}

uint16_t regalert;

uint16_t ch0val;
uint16_t ch1val;
uint16_t ch2val;
uint16_t ch3val;

void i2c_read_chn(int chn){
uint16_t regval;



	// open Linux I2C device
	i2cOpen();

	// set address of the PCA9555
	i2cSetAddress(0x23);


	regval = i2cReadRegisterPair(0x02);
	i2cWriteRegisterPair(0x02, 0x04C1);


	regalert = i2cReadRegisterPair(0x03);
	//printf("reg3 alert: %d\n", regval);


/*
	regval = i2cReadRegisterPair(0x04);
	printf("reg4 ch0low: %d\n", regval);

	regval = i2cReadRegisterPair(0x05);
	printf("reg5 ch0high: %d\n", regval);
	
	regval = i2cReadRegisterPair(0x06);
	printf("reg6 ch0hyst: %d\n", regval);



	regval = i2cReadRegisterPair(0x07);
	printf("reg7 ch1low: %d\n", regval);

	regval = i2cReadRegisterPair(0x08);
	printf("reg8 ch1high: %d\n", regval);
	
	regval = i2cReadRegisterPair(0x09);
	printf("reg9 ch1hyst: %d\n", regval);



	regval = i2cReadRegisterPair(0x0A);
	printf("regA ch2low: %d\n", regval);

	regval = i2cReadRegisterPair(0x0B);
	printf("regB ch2high: %d\n", regval);
	
	regval = i2cReadRegisterPair(0x0C);
	printf("regC ch2hyst: %d\n", regval);



	regval = i2cReadRegisterPair(0x0D);
	printf("regD ch3low: %d\n", regval);

	regval = i2cReadRegisterPair(0x0E);
	printf("regE ch3high: %d\n", regval);
	
	regval = i2cReadRegisterPair(0x0F);
	printf("regF ch3hyst: %d\n", regval);
*/


	//channel read
	if(chn==0){
		i2cWriteRegisterPair(0x01, 0x0101);
		ch0val = i2cReadRegisterPair(0x00);
	}
	else if(chn==1){
		i2cWriteRegisterPair(0x01, 0x0202);
		ch1val = i2cReadRegisterPair(0x00);
	}
	else if(chn==2){
		i2cWriteRegisterPair(0x01, 0x0404);
		ch2val = i2cReadRegisterPair(0x00);
	}
	else if(chn==3){
		i2cWriteRegisterPair(0x01, 0x0808);
		ch3val = i2cReadRegisterPair(0x00);
	}
/*
	printf("chn0: %d alert:%d\n", ch0val & 0x0FFF, (int)((ch0val & 0x1000)!=0));
	printf("chn1: %d alert:%d\n", ch1val & 0x0FFF, (int)((ch1val & 0x1000)!=0));
	printf("chn2: %d alert:%d\n", ch2val & 0x0FFF, (int)((ch2val & 0x1000)!=0));
	printf("chn3: %d alert:%d\n", ch3val & 0x0FFF, (int)((ch3val & 0x1000)!=0));
*/

	// close Linux I2C device
	i2cClose();

	return ;

}



void die(char *s)
{
    perror(s);
    exit(1);
}
int read_gpio(int idport)
{
	char filename[256];
	char out_str[256];
	int ret;
	int out_value = -1;
	
	sprintf(filename, "/dev/gpio%d_irq", idport);
	
	int fd = open(filename, O_RDONLY);
	
	if(fd < 0){
		printf("err opening gpio file %d\n",idport);
	}else{
		ret = read(fd, out_str, sizeof out_str);
		if(ret <= 0){
			printf("err reading gpio file %d\n",idport);	
		} else {				
			close(fd);
			sscanf(out_str, "%d", &out_value);
		}				
	}
	
	if (fd >= 0) {
		close(fd);
	}
	
	return out_value;
}
int read_in(int idport)
{
	char filename[256];
	char out_str[256];
	int ret;
	int out_value = -1;
	
	sprintf(filename, "/sys/class/gpio/gpio%d/value", idport);
	
	int fd = open(filename, O_RDONLY);
	
	if(fd < 0){
		printf("err opening gpio file %d\n",idport);
	}else{
		ret = read(fd, out_str, sizeof out_str);
		if(ret <= 0){
			printf("err reading gpio file %d\n",idport);	
		} else {				
			close(fd);
			sscanf(out_str, "%d", &out_value);
		}				
	}
	
	if (fd >= 0) {
		close(fd);
	}
	
	return out_value;
}
int read_pwm(int idport)
{
	char filename[256];
	char out_str[256];
	int ret;
	int out_value = -1;
	
	sprintf(filename, "/sys/class/backlight/backlight_lcd/brightness");
	
	int fd = open(filename, O_RDONLY);
	
	if(fd < 0){
		printf("err opening brightness file\n");
	}else{
		ret = read(fd, out_str, sizeof out_str);
		if(ret <= 0){
			printf("err reading brightness file\n");	
		} else {				
			close(fd);
			sscanf(out_str, "%d", &out_value);
		}				
	}
	
	if (fd >= 0) {
		close(fd);
	}
	
	if(out_value==50){
		out_value=0;
	}
	else if(out_value==130){
		out_value=1;
	}

	return out_value;
}
int write_out(int idport, int val)
{
	char filename[256];
	int out_value = -1;
	
	int ret;
	sprintf(filename, "/sys/class/gpio/gpio%d/value", idport);
		
	int fd = open(filename, O_WRONLY, O_APPEND);
	if(fd < 0){
		printf("err opening gpio file %d\n",idport);
		out_value=0;
	}else{
		if(val == 0){
			ret = write(fd, "0", 1);
		}else{
			ret = write(fd, "1", 1);
		}
		out_value=1;
		
		if(ret < 0){
			printf("err writing gpio file %d\n",idport);
			out_value=0;
		}
		close(fd);
	}

	if (fd >= 0) {
		close(fd);
	}
	
	return out_value;
}
int write_pwm(int idport, int val)
{
	char filename[256];
	int out_value = -1;
	
	int ret;
	sprintf(filename, "/sys/class/backlight/backlight_lcd/brightness");
		
	int fd = open(filename, O_WRONLY, O_APPEND);
	if(fd < 0){
		printf("err opening brightness file\n");
		out_value=0;
	}else{
		if(val == 0){
			ret = write(fd, "50", 2);
		}else{
			ret = write(fd, "130", 3);
		}
		out_value=1;
		
		if(ret < 0){
			printf("err writing brightness file\n");
			out_value=0;
		}
		close(fd);
	}

	if (fd >= 0) {
		close(fd);
	}
	
	return out_value;
}


int read_eth_speed()
{
	char filename[256];
	char out_str[256];
	int ret;
	int out_value = -1;
	
	sprintf(filename, "/sys/class/net/eth0/speed");
	
	int fd = open(filename, O_RDONLY);
	
	if(fd < 0){
		printf("err opening /sys/class/net/eth0/speed\n");
	}else{
		ret = read(fd, out_str, sizeof out_str);
		if(ret <= 0){
			printf("err reading /sys/class/net/eth0/speed\n");	
		} else {				
			close(fd);
			sscanf(out_str, "%d", &out_value);
		}				
	}
	
	if (fd >= 0) {
		close(fd);
	}
	
	return out_value;
}



void pabort(const char *s)
{
	perror(s);
	abort();
}


unsigned char tmp_rx_buf[300];
unsigned char tmp_tx_buf[300];


static const char *device0 = "/dev/spidev0.0";
static const char *device2 = "/dev/spidev2.0";
static const char *device3 = "/dev/spidev3.0";
static const char *device4 = "/dev/spidev4.0";

static uint8_t mode = 3;	//le SPI FPGA richiedono mode3
static uint8_t mode_old = 0; //la SPI del uP richiede mode0
static uint8_t bits = 8;
static uint32_t speed = 100000;
static uint16_t delay=0;
static uint16_t pausaus = 100;


int spi_send_receive(unsigned char reqchn, unsigned char reqspeed, unsigned char reqdelayms, unsigned char lentx, unsigned char lenrx)
{
	int ret = 0;
	int fd;
	//unsigned int csid;
	unsigned char  cmd_buf[300];
	unsigned char  data_buf[300];
	struct         spi_ioc_transfer xfer;
 

	if(reqchn==0){
		fd = open(device0, O_RDWR);
		//csid = OUT_ECSPI1_SS0;
	}
	else if(reqchn==2){
		fd = open(device2, O_RDWR);
		//csid = OUT_ECSPI3_SS0;
	}
	else if(reqchn==3){
		fd = open(device3, O_RDWR);
		//csid = OUT_ECSPI4_SS0;
	}
	else if(reqchn==4){
		fd = open(device4, O_RDWR);
		//csid = OUT_ECSPI5_SS0;
	}
		
	if (fd < 0)
		pabort("can't open device");

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");
		
		
	switch (reqspeed){
		case 0x00:
			speed = 100000;
			break;
		case 0x10:
			speed = 200000;
			break;
		case 0x20:
			speed = 400000;
			break;
		case 0x30:
			speed = 600000;
			break;
		case 0x40:
			speed = 800000;
			break;
		case 0x50:
			speed = 1000000;
			break;
		case 0x60:
			speed = 1200000;
			break;
		case 0x70:
			speed = 1400000;
			break;
		case 0x80:
			speed = 2000000;
			break;
		case 0x90:
			speed = 2500000;
			break;
		case 0xA0:
			speed = 5000000;
			break;
		case 0xB0:
			speed = 7500000;
			break;
		case 0xC0:
			speed = 10000000;
			break;
		case 0xD0:
			speed = 20000000;
			break;
		case 0xE0:
			speed = 25000000;
			break;
		case 0xF0:
			speed = 30000000;
			break;
			
		default:
			speed = 100000;
			break;
	}
	
	//per la SPI1 non gestisco manualmente il CS
	//if(reqchn!=0){
	//	write_out(csid,0);
	//}
	
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	
	for(int i=0; i<lentx; i++){
		cmd_buf[i] = tmp_tx_buf[i];
	}
	for(int i=lentx; i<lentx+lenrx; i++){
		cmd_buf[i] = 0xFF;
	}	
		
	memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (unsigned long)cmd_buf;
	xfer.rx_buf = (unsigned long)data_buf;
    xfer.len = lentx+lenrx;
	xfer.delay_usecs = delay;
	xfer.speed_hz = speed;
	xfer.bits_per_word = bits;
   	xfer.cs_change = 0; 


    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (ret < 1)
		pabort("can't send spi message");
		
		
	//per la SPI1 non gestisco manualmente il CS
	//if(reqchn!=0){
	//	write_out(csid,1);
	//}
	
	for(int i=0; i<lenrx; i++){
		tmp_rx_buf[i] = data_buf[i+lentx];
	}
	
	close(fd);

	return ret;	
}


int spi_send_receive_old(unsigned char reqchn, unsigned char reqspeed, unsigned char reqdelayms, unsigned char lentx, unsigned char lenrx)
{
	int ret = 0;
	int fd;
	unsigned int csid;
	unsigned char  cmd_buf[300];
	unsigned char  data_buf[300];
	struct         spi_ioc_transfer xfer;
 

	if(reqchn==0){
		fd = open(device0, O_RDWR);
		//csid = OUT_ECSPI1_SS0;
	}
	else if(reqchn==2){
		fd = open(device2, O_RDWR);
		csid = OUT_ECSPI3_SS0;
	}
	else if(reqchn==3){
		fd = open(device3, O_RDWR);
		csid = OUT_ECSPI4_SS0;
	}
	else if(reqchn==4){
		fd = open(device4, O_RDWR);
		csid = OUT_ECSPI5_SS0;
	}
		
	if (fd < 0)
		pabort("can't open device");

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode_old);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode_old);
	if (ret == -1)
		pabort("can't get spi mode");

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	switch (reqspeed){
		case 0x00:
			speed = 100000;
			break;
		case 0x10:
			speed = 200000;
			break;
		case 0x20:
			speed = 400000;
			break;
		case 0x30:
			speed = 600000;
			break;
		case 0x40:
			speed = 800000;
			break;
		case 0x50:
			speed = 1000000;
			break;
		case 0x60:
			speed = 1200000;
			break;
		case 0x70:
			speed = 1400000;
			break;
		case 0x80:
			speed = 2000000;
			break;
		case 0x90:
			speed = 2500000;
			break;
		case 0xA0:
			speed = 5000000;
			break;
		case 0xB0:
			speed = 7500000;
			break;
		case 0xC0:
			speed = 10000000;
			break;
		case 0xD0:
			speed = 20000000;
			break;
		case 0xE0:
			speed = 25000000;
			break;
		case 0xF0:
			speed = 30000000;
			break;
			
		default:
			speed = 100000;
			break;
	}
	
	//per la SPI1 non gestisco manualmente il CS
	if(reqchn!=0){
		write_out(csid,0);
	}
	
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	//printf("spi mode: %d\n", mode);
	//printf("bits per word: %d\n", bits);
	//printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	
	if(reqdelayms>0){
		pausaus = reqdelayms * 10000;
	}
	
	for(int i=0; i<lentx; i++){
		cmd_buf[i] = tmp_tx_buf[i];
	}

	memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (unsigned long)cmd_buf;
	xfer.rx_buf = (unsigned long)data_buf;
    xfer.len = lentx;
	xfer.delay_usecs = delay;
	xfer.speed_hz = speed;
	xfer.bits_per_word = bits;
   	xfer.cs_change = 0; 


    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (ret < 1)
		pabort("can't send spi message");

	
	usleep(pausaus);
	
	for(int i=0; i<lenrx; i++){
		cmd_buf[i] = 0xFF;
		data_buf[i] = 0xFF;
	}
	
	xfer.tx_buf = (unsigned long)cmd_buf;
	xfer.rx_buf = (unsigned long)data_buf;
    xfer.len = lenrx; 
	xfer.delay_usecs = delay;
	xfer.speed_hz = speed;
	xfer.bits_per_word = bits;
    xfer.cs_change = 0; 


    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (ret < 1)
		pabort("can't send spi message");
	
	//per la SPI1 non gestisco manualmente il CS
	if(reqchn!=0){
		write_out(csid,1);
	}
	
	for(int i=0; i<lenrx; i++){
		tmp_rx_buf[i] = data_buf[i];
	}
	
	close(fd);

	return ret;
}


int main(void)
{
    struct sockaddr_in si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , rx_len, tx_len;
    unsigned char rx_buf[BUFLEN];
    unsigned char tx_buf[BUFLEN];
    unsigned char crc_buf[BUFLEN];
    int tmp;
    
    
    
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
    while(1)
    {
		
		
		
        //printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((rx_len = recvfrom(s, rx_buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
		
		
		if(rx_len == 5){
			//printf("TX: ");
			//for(int i=2; i<rx_len; i++){
			//	printf("%02X ", rx_buf[i]);
			//}
			//printf("\n");
		
			if(rx_buf[0]==0x0C){
				tmp_tx_buf[0] = rx_buf[2];
				tmp_tx_buf[1] = rx_buf[3];
				tmp_tx_buf[2] = rx_buf[4];
				
				//spidev2.0
				i = spi_send_receive(0x02, (rx_buf[1] & 0xF0),(rx_buf[1] & 0x0F), 3, tmp_tx_buf[2] + 2);
				
			
				tx_buf[0] = 0xCC;
				tx_buf[1] = rx_buf[1];

				for(int i=0; i<(tmp_tx_buf[2] + 2); i++){
					tx_buf[i+2] = tmp_rx_buf[i];
				}
				tx_len = tmp_tx_buf[2] + 4;
				
				//printf("RX: ",rx_len);
				//for(int i=2; i<tx_len; i++){
				//	printf("%02X ", tx_buf[i]);
				//}
				//printf("\n");
				
			}
			else{
				continue;
			}
			if (sendto(s, tx_buf, tx_len, 0, (struct sockaddr*) &si_other, slen) == -1)
			{
				die("sendto()");
			}
		}
		else if(rx_len == 7){
			//printf("RX: ");
			//for(i=0;i<7;i++){
			//	printf("%02X ",rx_buf[i]);
			//}
			//printf("\n");
		
			if(rx_buf[0]==0x00){
				tx_buf[0] = 0x00;
				tx_buf[1] = 0x55;
				tx_buf[2] = 0xAA;
				tx_buf[3] = 0x55;
				tx_buf[4] = 0xAA;
				tx_buf[5] = 0x55;
				tx_buf[6] = 0xAA;
				tx_len = 7;
			}
			else if(rx_buf[0]==0x01){
				tmp_tx_buf[0] = rx_buf[2];
				tmp_tx_buf[1] = rx_buf[3];
				tmp_tx_buf[2] = rx_buf[4];
				tmp_tx_buf[3] = rx_buf[5];
				tmp_tx_buf[4] = rx_buf[6];
			
				//spidev0.0
				i = spi_send_receive_old(0x00, (rx_buf[1] & 0xF0),(rx_buf[1] & 0x0F), 5, 5);
				//printf("rx_len=%d\n",i);
			
				tx_buf[0] = 0x11;
				tx_buf[1] = rx_buf[1];
				tx_buf[2] = tmp_rx_buf[0];
				tx_buf[3] = tmp_rx_buf[1];
				tx_buf[4] = tmp_rx_buf[2];
				tx_buf[5] = tmp_rx_buf[3];
				tx_buf[6] = tmp_rx_buf[4];
				tx_len = 7;
			}
			else if(rx_buf[0]==0x02){
				tmp_tx_buf[0] = rx_buf[2];
				tmp_tx_buf[1] = rx_buf[3];
				tmp_tx_buf[2] = rx_buf[4];
				tmp_tx_buf[3] = rx_buf[5];
				tmp_tx_buf[4] = rx_buf[6];
			
				//spidev2.0
				i = spi_send_receive(0x02, (rx_buf[1] & 0xF0),(rx_buf[1] & 0x0F), 5, 5);
				//printf("rx_len=%d\n",i);
			
				tx_buf[0] = 0x22;
				tx_buf[1] = rx_buf[1];
				tx_buf[2] = tmp_rx_buf[0];
				tx_buf[3] = tmp_rx_buf[1];
				tx_buf[4] = tmp_rx_buf[2];
				tx_buf[5] = tmp_rx_buf[3];
				tx_buf[6] = tmp_rx_buf[4];
				tx_len = 7;
			}
			else if(rx_buf[0]==0x03){
				tmp_tx_buf[0] = rx_buf[2];
				tmp_tx_buf[1] = rx_buf[3];
				tmp_tx_buf[2] = rx_buf[4];
				tmp_tx_buf[3] = rx_buf[5];
				tmp_tx_buf[4] = rx_buf[6];
			
				//spidev3.0
				i = spi_send_receive(0x03, (rx_buf[1] & 0xF0),(rx_buf[1] & 0x0F), 5, 5);
				//printf("rx_len=%d\n",i);
			
				tx_buf[0] = 0x33;
				tx_buf[1] = rx_buf[1];
				tx_buf[2] = tmp_rx_buf[0];
				tx_buf[3] = tmp_rx_buf[1];
				tx_buf[4] = tmp_rx_buf[2];
				tx_buf[5] = tmp_rx_buf[3];
				tx_buf[6] = tmp_rx_buf[4];
				tx_len = 7;
			}
			else if(rx_buf[0]==0x04){
				tmp_tx_buf[0] = rx_buf[2];
				tmp_tx_buf[1] = rx_buf[3];
				tmp_tx_buf[2] = rx_buf[4];
				tmp_tx_buf[3] = rx_buf[5];
				tmp_tx_buf[4] = rx_buf[6];
			
				//spidev4.0
				i = spi_send_receive(0x04, (rx_buf[1] & 0xF0),(rx_buf[1] & 0x0F), 5, 5);
				//printf("rx_len=%d\n",i);
			
				tx_buf[0] = 0x44;
				tx_buf[1] = rx_buf[1];
				tx_buf[2] = tmp_rx_buf[0];
				tx_buf[3] = tmp_rx_buf[1];
				tx_buf[4] = tmp_rx_buf[2];
				tx_buf[5] = tmp_rx_buf[3];
				tx_buf[6] = tmp_rx_buf[4];
				tx_len = 7;
			}
			else if(rx_buf[0]==0x05){
			
				if(rx_buf[1]==0x00){
					i2c_read_chn(0x00);
				
					tx_buf[0] = 0x55;
					tx_buf[1] = 0x00;
					tx_buf[2] = 0x00;
					tx_buf[3] = 0x00;
					tx_buf[4] = 0x00;
				
					tx_buf[5] = (unsigned char)((ch0val & 0x1F00) >> 8);
					tx_buf[6] = (unsigned char)((ch0val & 0x00FF) >> 0);
					tx_len = 7;
				}
				else if(rx_buf[1]==0x01){
					i2c_read_chn(0x01);
				
					tx_buf[0] = 0x55;
					tx_buf[1] = 0x01;
					tx_buf[2] = 0x00;
					tx_buf[3] = 0x00;
					tx_buf[4] = 0x00;
				
					tx_buf[5] = (unsigned char)((ch1val & 0x1F00) >> 8);
					tx_buf[6] = (unsigned char)((ch1val & 0x00FF) >> 0);
					tx_len = 7;
				}
				else if(rx_buf[1]==0x02){
					i2c_read_chn(0x02);
				
					tx_buf[0] = 0x55;
					tx_buf[1] = 0x02;
					tx_buf[2] = 0x00;
					tx_buf[3] = 0x00;
					tx_buf[4] = 0x00;
				
					tx_buf[5] = (unsigned char)((ch2val & 0x1F00) >> 8);
					tx_buf[6] = (unsigned char)((ch2val & 0x00FF) >> 0);
					tx_len = 7;
				}
				else if(rx_buf[1]==0x03){
					i2c_read_chn(0x03);
				
					tx_buf[0] = 0x55;
					tx_buf[1] = 0x03;
					tx_buf[2] = 0x00;
					tx_buf[3] = 0x00;
					tx_buf[4] = 0x00;
				
					tx_buf[5] = (unsigned char)((ch3val & 0x1F00) >> 8);
					tx_buf[6] = (unsigned char)((ch3val & 0x00FF) >> 0);
					tx_len = 7;
				}
			}
			//COMANDO LETTURA LINEE INGRESSO (A)
			else if(rx_buf[0]==0x06){
				tx_buf[0] = 0x66;
				tx_buf[1] = 0x00;
			
				tmp = 0;
				tmp += read_in(IN_SPARE_1_IN_CPU) 	<< 0;
				tmp += read_in(IN_SPARE_2_IN_CPU) 	<< 1;
				tmp += read_in(IN_SPARE_3_IN_CPU) 	<< 2;
				tmp += read_in(IN_OK_TH_CPU) 		<< 3;
				tmp += read_in(IN_CK_PLUG_0_CPU) 	<< 4;
				tmp += read_in(IN_CK_PLUG_1_CPU) 	<< 5;
				tmp += read_in(IN_LOOPBACK_SIGNAL_CPU) << 6;
				tmp += read_in(IN_OK_SPOLETTA_CPU) 	<< 7;
				tx_buf[2] = tmp;
			
				tmp = 0;
				tmp += read_in(IN_OK_DL_CPU)		<< 0;
				tmp += read_in(IN_OK_PS_TH_CPU) 	<< 1;
				tmp += read_in(IN_OK_GPS_CPU)		<< 2;
				tmp += read_in(IN_STATUS_ANT_CPU) 	<< 3;
				tmp += read_in(IN_OK_PS_SWRF_CPU) 	<< 4;
				tmp += read_in(IN_FUS_BT1_CPU)		<< 5;
				tmp += read_in(IN_FUS_BT2_CPU)		<< 6;
				tmp += read_in(IN_FAILSAFE_ENABLE_CPU) 	<< 7;
				tx_buf[3] = tmp;
			
				tmp = 0;
				tmp += read_in(IN_MAINT_SEL_CPU)	<< 0;
				tmp += read_in(IN_OK_BT_SK_PSB_CPU)	<< 1;
				tmp += read_in(IN_OK_PS_WCU_CPU)	<< 2;
				tmp += read_in(IN_FUS_SK_CPU)		<< 3;
				tmp += read_in(IN_PB_SX_CPU)		<< 4;
				tmp += read_in(IN_TELLBACKBT1_CPU)	<< 5;
				tmp += read_in(IN_CTR_TELLBACKBT2_CPU) 	<< 6;
				tmp += read_in(IN_CK_BT2_CPU)		<< 7;
				tx_buf[4] = tmp;

				tmp = 0;
				tmp += read_in(IN_TB2_CPU)			<< 0;
				tmp += read_in(IN_TELLBACKBT2_CPU)	<< 1;
				tmp += read_in(IN_OK_TIMER_CPU)		<< 2;
				tmp += read_in(IN_TERMINATE_CMD_1_CPU) 	<< 3;
				tmp += read_in(IN_TERMINATE_CMD_2_CPU) 	<< 4;
				tmp += read_in(IN_GO_SC_CPU)		<< 5;
				tmp += read_in(IN_CK_PRES_BT2_CPU)	<< 6;
				tmp += read_in(IN_T0_MSL_CPU)	<< 7;
				tx_buf[5] = tmp;

				tmp = 0;
				tmp += read_in(IN_STATO_D1_CPU)		<< 0;
				tmp += read_in(IN_STATO_D2_CPU)		<< 1;
				tmp += read_in(IN_PB_DX_CPU)		<< 2;
				tmp += read_in(IN_GO_HW_CPU)		<< 3;
				tmp += read_in(IN_GO_NOGO_MSL_CPU) 	<< 4;
				tmp += read_in(IN_GO_NOGO_UA_CPU) 	<< 5;
				tmp += read_in(IN_UART3_RTS_B_CPU) 	<< 6;
				tx_buf[6] = tmp;
				tx_len = 7;

			
				//printf("TX: ");
				//for(i=0;i<7;i++){
				//	printf("%02X ",tx_buf[i]);
				//}
				//printf("\n");
		
			}
			//COMANDO LETTURA LINEE INGRESSO (B)
			else if(rx_buf[0]==0x07){
				tx_buf[0] = 0x77;
				tx_buf[1] = 0x00;
			
				tmp = 0;
				tmp += read_in(IN_ARM_TRACE_08) << 0;
				tmp += read_in(IN_ARM_TRACE_09) << 1;
				tmp += read_in(IN_ARM_TRACE_10) << 2;
				tmp += read_in(IN_ARM_TRACE_11) << 3;
				tmp += read_in(IN_ARM_TRACE_12) << 4;
				tmp += read_in(IN_ARM_TRACE_13) << 5;
				tmp += read_in(IN_ARM_TRACE_14) << 6;
				tmp += read_in(IN_ARM_TRACE_15) << 7;
				tx_buf[2] = tmp;
			
				tmp = 0;
				tmp += read_in(IN_ARM_EVENTI)		<< 0;
				tmp += read_in(IN_ARM_TRACE_CTL) 	<< 1;
				tmp += read_gpio(IN_IT_SL_1) << 2;
				tmp += read_gpio(IN_IT_SL_2) << 3;
				tmp += read_gpio(IN_IT_SL_3) << 4;
				tmp += read_gpio(IN_IT_SL_4) << 5;
				tmp += read_gpio(IN_IT_SL_5) << 6;
				tmp += read_gpio(IN_IT_SL_6) << 7;
				tx_buf[3] = tmp;
			
				tmp = 0;
				tmp += read_gpio(IN_IT_SL_7) << 0;
				tmp += read_gpio(IN_IT_SL_8) << 1;
				tmp += read_gpio(IN_IT_SL_10) << 2;
				tmp += read_in(IN_SPARE1_ARTIX7) << 3;
				tmp += read_in(IN_SPARE2_ARTIX7) << 4;
				tmp += read_in(IN_GPIO4_IO06_UNUSED) << 5;
				tmp += read_in(IN_GPIO7_IO12_UNUSED) << 6;
				tmp += read_in(IN_T0_CONN_CPU) << 7;
				tx_buf[4] = tmp;
			
				tmp = 0;
				tmp += read_in(IN_USB_OTG_OC) << 0;
				tmp += read_in(IN_USB_OTG_ID) << 1;
				tx_buf[5] = tmp;
			
				tx_buf[6] = 0x00;
				tx_len = 7;
				
			}
			else if(rx_buf[0]==0x08){
		
				write_out(OUT_SPARE_1_OUT_CPU, ((rx_buf[2] & 	0x01)!=0));
				write_out(OUT_SPARE_2_OUT_CPU, ((rx_buf[2] & 	0x02)!=0));
				write_out(OUT_SPARE_3_OUT_CPU, ((rx_buf[2] & 	0x04)!=0));
				write_out(OUT_EASAU_CPU, ((rx_buf[2] & 			0x08)!=0));
				write_out(OUT_EEO_CPU, ((rx_buf[2] & 			0x10)!=0));
				write_out(OUT_ESS_CPU, ((rx_buf[2] & 			0x20)!=0));
				write_out(OUT_EPA_CPU, ((rx_buf[2] & 			0x40)!=0));
				write_out(OUT_EDB_CPU, ((rx_buf[2] & 			0x80)!=0));
			
				write_out(OUT_EFB_CPU, ((rx_buf[3] & 			0x01)!=0));
				write_out(OUT_EAB_CPU, ((rx_buf[3] & 			0x02)!=0));
				write_out(OUT_INH_SORVOLO_CPU, ((rx_buf[3] & 	0x04)!=0));
				write_out(OUT_EBT_CPU, ((rx_buf[3] & 			0x08)!=0));
				write_out(OUT_INH_G_SWITCH_CPU, ((rx_buf[3] & 	0x10)!=0));
				write_out(OUT_BIT_L_CPU, ((rx_buf[3] & 			0x20)!=0));
				write_out(OUT_EBT_SK_CPU, ((rx_buf[3] & 			0x40)!=0));
				write_out(OUT_SKR_PWR_CTR_CPU, ((rx_buf[3] & 	0x80)!=0));
			
				write_pwm(OUT_CMD_CONSENSOFUOCO_CPU, ((rx_buf[4] & 0x01)!=0));				
				write_out(OUT_CMD_FMP_INT_CPU, ((rx_buf[4] & 	0x02)!=0));
				write_out(OUT_SENS_D00_CPU, ((rx_buf[4] & 		0x04)!=0));
				write_out(OUT_SENS_D01_CPU, ((rx_buf[4] & 		0x08)!=0));
				write_out(OUT_ID_00_CPU, ((rx_buf[4] & 			0x10)!=0));
				write_out(OUT_ID_01_CPU, ((rx_buf[4] & 			0x20)!=0));
				write_out(OUT_TFUEL_RANGE_SLC, ((rx_buf[4] & 	0x40)!=0));
				write_out(OUT_MAINT_SK_CPU, ((rx_buf[4] & 		0x80)!=0));
			
				write_out(OUT_SEL_ANT_CPU, ((rx_buf[5] & 		0x01)!=0));
				write_out(OUT_RESET_L_CPU, ((rx_buf[5] & 		0x02)!=0));
				write_out(OUT_CMD_SPIRA_CPU, ((rx_buf[5] & 		0x04)!=0));
				write_out(OUT_IMX6_SPARE_LED, ((rx_buf[5] & 		0x08)!=0));
				write_out(OUT_SAFE_SPOLETTA_CPU, ((rx_buf[5] & 	0x10)!=0));
				write_out(OUT_ENABLE_DL_CPU, ((rx_buf[5] & 		0x20)!=0));
				write_out(OUT_ESA_CPU, ((rx_buf[5] & 			0x40)!=0));
				write_out(OUT_ESF_CPU, ((rx_buf[5] & 			0x80)!=0));
			
				write_out(OUT_OK_CPU, ((rx_buf[6] & 				0x01)!=0));
				write_out(OUT_GO_SW_CPU, ((rx_buf[6] & 				0x02)!=0));
				write_out(OUT_UART3_CTS_B_CPU, ((rx_buf[6] & 		0x04)!=0));
				write_out(OUT_IT_UC, ((rx_buf[6] & 					0x08)!=0));
				write_out(OUT_OTG_PWR_EN, ((rx_buf[6] & 			0x10)!=0));
				write_out(OUT_RESET_ADC_CPU, ((rx_buf[6] & 0x20)!=0));
				write_out(OUT_GPIO6_IO06_UNUSED, ((rx_buf[6] &  	0x40)!=0));
				write_out(OUT_GPIO2_IO28_UNUSED, ((rx_buf[6] &  	0x80)!=0));
			
				tx_buf[0] = 0x88;
				tx_buf[1] = 0x00;
			
				tmp = 0;
				tmp += read_in(OUT_SPARE_1_OUT_CPU)			<< 0;
				tmp += read_in(OUT_SPARE_2_OUT_CPU)			<< 1;
				tmp += read_in(OUT_SPARE_3_OUT_CPU)			<< 2;
				tmp += read_in(OUT_EASAU_CPU)				<< 3;
				tmp += read_in(OUT_EEO_CPU)					<< 4;
				tmp += read_in(OUT_ESS_CPU)					<< 5;
				tmp += read_in(OUT_EPA_CPU)					<< 6;
				tmp += read_in(OUT_EDB_CPU)					<< 7;
				tx_buf[2] = tmp;
			
				tmp = 0;
				tmp += read_in(OUT_EFB_CPU)					<< 0;
				tmp += read_in(OUT_EAB_CPU)					<< 1;
				tmp += read_in(OUT_INH_SORVOLO_CPU)			<< 2;
				tmp += read_in(OUT_EBT_CPU)					<< 3;
				tmp += read_in(OUT_INH_G_SWITCH_CPU)		<< 4;
				tmp += read_in(OUT_BIT_L_CPU)				<< 5;
				tmp += read_in(OUT_EBT_SK_CPU)				<< 6;
				tmp += read_in(OUT_SKR_PWR_CTR_CPU)			<< 7;
				tx_buf[3] = tmp;
			
				tmp = 0;
				tmp += read_pwm(OUT_CMD_CONSENSOFUOCO_CPU)	<< 0;
				tmp += read_in(OUT_CMD_FMP_INT_CPU)			<< 1;
				tmp += read_in(OUT_SENS_D00_CPU)			<< 2;
				tmp += read_in(OUT_SENS_D01_CPU)			<< 3;
				tmp += read_in(OUT_ID_00_CPU)				<< 4;
				tmp += read_in(OUT_ID_01_CPU)				<< 5;
				tmp += read_in(OUT_TFUEL_RANGE_SLC)			<< 6;
				tmp += read_in(OUT_MAINT_SK_CPU)			<< 7;
				tx_buf[4] = tmp;
			
				tmp = 0;
				tmp += read_in(OUT_SEL_ANT_CPU)				<< 0;
				tmp += read_in(OUT_RESET_L_CPU)				<< 1;
				tmp += read_in(OUT_CMD_SPIRA_CPU)			<< 2;
				tmp += read_in(OUT_IMX6_SPARE_LED)			<< 3;
				tmp += read_in(OUT_SAFE_SPOLETTA_CPU)		<< 4;
				tmp += read_in(OUT_ENABLE_DL_CPU)			<< 5;
				tmp += read_in(OUT_ESA_CPU)					<< 6;
				tmp += read_in(OUT_ESF_CPU)					<< 7;
				tx_buf[5] = tmp;
			
				tmp = 0;
				tmp += read_in(OUT_OK_CPU)					<< 0;
				tmp += read_in(OUT_GO_SW_CPU)				<< 1;
				tmp += read_in(OUT_UART3_CTS_B_CPU)			<< 2;
				tmp += read_in(OUT_IT_UC)					<< 3;
				tmp += read_in(OUT_OTG_PWR_EN)				<< 4;
				tmp += read_in(OUT_RESET_ADC_CPU)			<< 5;
				tmp += read_in(OUT_GPIO6_IO06_UNUSED)		<< 6;
				tmp += read_in(OUT_GPIO2_IO28_UNUSED)		<< 7;
				tx_buf[6] = tmp;
				tx_len = 7;
			
		
			}
			else if(rx_buf[0]==0x09){
			
				write_out(OUT_ARM_TRACE_00, ((rx_buf[2] & 	0x01)!=0));
				write_out(OUT_ARM_TRACE_01, ((rx_buf[2] & 	0x02)!=0));
				write_out(OUT_ARM_TRACE_02, ((rx_buf[2] & 	0x04)!=0));
				write_out(OUT_ARM_TRACE_03, ((rx_buf[2] & 	0x08)!=0));
				write_out(OUT_ARM_TRACE_04, ((rx_buf[2] & 	0x10)!=0));
				write_out(OUT_ARM_TRACE_05, ((rx_buf[2] & 	0x20)!=0));
				write_out(OUT_ARM_TRACE_06, ((rx_buf[2] & 	0x40)!=0));
				write_out(OUT_ARM_TRACE_07, ((rx_buf[2] & 	0x80)!=0));
			
				write_out(OUT_ARM_EVENTO, ((rx_buf[3] & 	0x01)!=0));
				write_out(OUT_ARM_TRACE_CLK, ((rx_buf[3] & 	0x02)!=0));
				write_out(OUT_ECSPI3_SS1, ((rx_buf[3] & 	0x04)!=0));
				write_out(OUT_ECSPI3_SS2, ((rx_buf[3] & 	0x08)!=0));
				write_out(OUT_ECSPI3_SS3, ((rx_buf[3] & 	0x10)!=0));
				write_out(OUT_ECSPI5_SS1, ((rx_buf[3] & 	0x20)!=0));
				write_out(OUT_ECSPI5_SS2, ((rx_buf[3] & 	0x40)!=0));
				write_out(OUT_ECSPI5_SS3, ((rx_buf[3] & 	0x80)!=0));
						
			
			
			
				tx_buf[0] = 0x99;
				tx_buf[1] = 0x00;
			
				tmp = 0;
				tmp += read_in(OUT_ARM_TRACE_00)	<< 0;
				tmp += read_in(OUT_ARM_TRACE_01)	<< 1;
				tmp += read_in(OUT_ARM_TRACE_02)	<< 2;
				tmp += read_in(OUT_ARM_TRACE_03)	<< 3;
				tmp += read_in(OUT_ARM_TRACE_04)	<< 4;
				tmp += read_in(OUT_ARM_TRACE_05)	<< 5;
				tmp += read_in(OUT_ARM_TRACE_06)	<< 6;
				tmp += read_in(OUT_ARM_TRACE_07)	<< 7;
				tx_buf[2] = tmp;
			
				tmp = 0;
				tmp += read_in(OUT_ARM_EVENTO)		<< 0;
				tmp += read_in(OUT_ARM_TRACE_CLK)	<< 1;
				tmp += read_in(OUT_ECSPI3_SS1)		<< 2;
				tmp += read_in(OUT_ECSPI3_SS2)		<< 3;
				tmp += read_in(OUT_ECSPI3_SS3)		<< 4;
				tmp += read_in(OUT_ECSPI5_SS1)		<< 5;
				tmp += read_in(OUT_ECSPI5_SS2)		<< 6;
				tmp += read_in(OUT_ECSPI5_SS3)		<< 7;
				tx_buf[3] = tmp;
			
				tx_buf[4] = 0x00;
				tx_buf[5] = 0x00;
				tx_buf[6] = 0x00;
				tx_len = 7;
			}
			else if(rx_buf[0]==0x0A){
				tx_buf[0] = 0xAA;
				tx_buf[1] = 0x00;
				tx_buf[2] = 0x01;
				tx_buf[3] = 0x00;
				tx_buf[4] = 0x00;
				tx_buf[5] = 0x00;
				tx_buf[6] = 0x00;
				tx_len = 7;
			}
			else if(rx_buf[0]==0x0B){
				tmp = read_eth_speed();
			
				tx_buf[0] = 0xBB;
				tx_buf[1] = 0x00;
				tx_buf[2] = 0x00;
				tx_buf[3] = 0x00;
				tx_buf[4] = 0x00;
				tx_buf[5] = (unsigned char)((tmp & 0xFF00)>>8);
				tx_buf[6] = (unsigned char)(tmp & 0xFF);
				tx_len = 7;
			}
			else{
				continue;
			}
		
			if (sendto(s, tx_buf, tx_len, 0, (struct sockaddr*) &si_other, slen) == -1)
			{
				die("sendto()");
			}
		}
		else{
			//printf("TX: ");
			//for(int i=2; i<rx_len; i++){
			//	printf("%02X ", rx_buf[i]);
			//}
			//printf("\n");
			
			if(rx_buf[0]==0x0D){
				
				for(int i=0; i<(rx_len-2); i++){
					tmp_tx_buf[i] = rx_buf[i+2];
				}
				
				//spidev2.0
				i = spi_send_receive(0x02, (rx_buf[1] & 0xF0),(rx_buf[1] & 0x0F), (rx_len-2), 1);
				
				
				
				tx_buf[0] = 0xDD;
				tx_buf[1] = rx_buf[1];
				tx_buf[2] = tmp_rx_buf[0];
				tx_len = 3;
				
				//printf("RX: ",rx_len);
				//for(int i=2; i<tx_len; i++){
				//	printf("%02X ", tx_buf[i]);
				//}
				//printf("\n");
				
				
			}
			else{
				continue;
			}
			if (sendto(s, tx_buf, tx_len, 0, (struct sockaddr*) &si_other, slen) == -1)
			{
				die("sendto()");
			}	
		}
	}
 
	close(s);
	return 0;
}



