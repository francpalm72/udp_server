#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

//===========================================
//   PROTOTYPE
//===========================================
int read_in(int idport);
int write_out(int idport, int val);

//===========================================
//   GPIO INPUT
//===========================================
	#define IN_SPARE_1_IN_CPU 90 
	#define IN_SPARE_2_IN_CPU 83 
	#define IN_SPARE_3_IN_CPU 91 
	#define IN_OK_TH_CPU 203
	#define IN_CK_PLUG_0_CPU 93 
	#define IN_CK_PLUG_1_CPU 94 
	#define IN_LOOPBACK_SIGNAL_CPU 140 
	#define IN_OK_SPOLETTA_CPU 114 

	#define IN_OK_DL_CPU 115 
	#define IN_OK_PS_TH_CPU 116 
	#define IN_OK_GPS_CPU 113 
	#define IN_STATUS_ANT_CPU 124 
	#define IN_OK_PS_SWRF_CPU 127 
	#define IN_FUS_BT1_CPU 30 
	#define IN_FUS_BT2_CPU 29
	#define IN_FAILSAFE_ENABLE_CPU 25 

	#define IN_MAINT_SEL_CPU 0
	#define IN_OK_BT_SK_PSB_CPU 141 
	#define IN_OK_PS_WCU_CPU 142 
	#define IN_FUS_SK_CPU 107 
	#define IN_PB_SX_CPU 103
	#define IN_TELLBACKBT1_CPU 26 
	#define IN_CTR_TELLBACKBT2_CPU 169 
	#define IN_CK_BT2_CPU 24

	#define IN_TB2_CPU 28 
	#define IN_TELLBACKBT2_CPU 205 
//#define IN_T0_OMB_CPU 101 //NON ESISTE PIU
	#define IN_OK_TIMER_CPU 27 //NUOVO
	#define IN_TERMINATE_CMD_1_CPU 174 
	#define IN_TERMINATE_CMD_2_CPU 175 
//#define IN_SENSOR_CPU 176  //NON ESISTE PIU
	#define IN_GO_SC_CPU 106 //NUOVO
	#define IN_CK_PRES_BT2_CPU 112
//#define IN_TIMER_SWITCH_CPU 32  //NON ESISTE PIU
	#define IN_T0_MSL_CPU 109 //NUOVO
	
	#define IN_STATO_D1_CPU 176 
	#define IN_STATO_D2_CPU 171
	#define IN_PB_DX_CPU 104 
	#define IN_GO_HW_CPU 143
	#define IN_GO_NOGO_MSL_CPU 144 
	#define IN_GO_NOGO_UA_CPU 145
	
	#define IN_UART3_RTS_B_CPU 95
	
	#define IN_ARM_TRACE_08 157 
	#define IN_ARM_TRACE_09 158 
	#define IN_ARM_TRACE_10 159 
	#define IN_ARM_TRACE_11 160 
	#define IN_ARM_TRACE_12 161 
	#define IN_ARM_TRACE_13 162 
	#define IN_ARM_TRACE_14 163 
	#define IN_ARM_TRACE_15 164 
	#define IN_ARM_EVENTI 5 
	#define IN_ARM_TRACE_CTL 147 

	#define IN_IT_SL_1 4 
	#define IN_IT_SL_2 7 
	#define IN_IT_SL_3 8 
	#define IN_IT_SL_4 9 
	#define IN_IT_SL_5 11 //nuovo era OUTPUT
	#define IN_IT_SL_6 10 //nuovo era OUTPUT
	#define IN_IT_SL_7 15  //nuovo era OUTPUT
	#define IN_IT_SL_8 14  //nuovo era OUTPUT
	#define IN_IT_SL_10 13  //nuovo era OUTPUT
	
	#define IN_SPARE1_ARTIX7 108 //nuovo era OUTPUT
	#define IN_SPARE2_ARTIX7 165 //nuovo era OUTPUT
	
	#define IN_GPIO4_IO06_UNUSED 102 //NUOVO
	#define IN_GPIO7_IO12_UNUSED 204 //NUOVO
	#define IN_USB_OTG_OC 110 //NUOVO
	#define IN_USB_OTG_ID 1 //NUOVO
	#define IN_T0_CONN_CPU 170  //NUOVO
	
	
	
	
	
//===========================================
//   GPIO OUTPUT
//===========================================
	#define OUT_SPARE_1_OUT_CPU 59 
	#define OUT_SPARE_2_OUT_CPU 58 
	#define OUT_SPARE_3_OUT_CPU 57 
	#define OUT_EASAU_CPU 54 
	#define OUT_EEO_CPU 53 
	#define OUT_ESS_CPU 52 
	#define OUT_EPA_CPU 51 
	#define OUT_EDB_CPU 50 
	
	#define OUT_EFB_CPU 49 
	#define OUT_EAB_CPU 48 
	#define OUT_INH_SORVOLO_CPU 132 
	#define OUT_EBT_CPU 130 
	#define OUT_INH_G_SWITCH_CPU 63 
	#define OUT_BIT_L_CPU 128 
	#define OUT_EBT_SK_CPU 191 
	#define OUT_SKR_PWR_CTR_CPU 56 
	
	#define OUT_CMD_CONSENSOFUOCO_CPU 125
	#define OUT_CMD_FMP_INT_CPU 126 
	#define OUT_SENS_D00_CPU 133 
	#define OUT_SENS_D01_CPU 134 
	#define OUT_ID_00_CPU 34 
	#define OUT_ID_01_CPU 35 
	#define OUT_TFUEL_RANGE_SLC 32 
	#define OUT_MAINT_SK_CPU 33 
	
	#define OUT_SEL_ANT_CPU 138 
	#define OUT_RESET_L_CPU 139 
	#define OUT_CMD_SPIRA_CPU 62 
	#define OUT_IMX6_SPARE_LED 101 
	#define OUT_SAFE_SPOLETTA_CPU 36 
	#define OUT_ENABLE_DL_CPU 37 
	#define OUT_ESA_CPU 38 
	#define OUT_ESF_CPU 39
	
	#define OUT_OK_CPU 135 
	#define OUT_GO_SW_CPU 136 

	#define OUT_UART3_CTS_B_CPU 87


	
	//#define OUT_IT_SL_5 11  //eliminato diventa INPUT
	//#define OUT_IT_SL_6 10  //eliminato diventa INPUT
	//#define OUT_IT_SL_7 15  //eliminato diventa INPUT
	//#define OUT_IT_SL_8 14  //eliminato diventa INPUT
	//#define OUT_IT_SL_10 13 //eliminato diventa INPUT

	#define OUT_ARM_EVENTO 146 
	#define OUT_ARM_TRACE_CLK 148 
	#define OUT_ARM_TRACE_00 149
	#define OUT_ARM_TRACE_01 150 
	#define OUT_ARM_TRACE_02 151 
	#define OUT_ARM_TRACE_03 152 
	#define OUT_ARM_TRACE_04 153 
	#define OUT_ARM_TRACE_05 154 
	#define OUT_ARM_TRACE_06 155 
	#define OUT_ARM_TRACE_07 156 

	//#define OUT_SPARE1_ARTIX7 108 //eliminato diventa INPUT
	//#define OUT_SPARE2_ARTIX7 171 //eliminato diventa INPUT

	#define OUT_OTG_PWR_EN 111 //NUOVO

	
	#define OUT_IT_UC 2 
	#define OUT_RESET_ADC_CPU 55 //NUOVO
	#define OUT_GPIO6_IO06_UNUSED 166 //NUOVO
	#define OUT_GPIO2_IO28_UNUSED 60 //NUOVO
	
	//#define OUT_ECSPI1_SS0 105
	
	#define OUT_ECSPI3_SS0 120
	#define OUT_ECSPI3_SS1 121
	#define OUT_ECSPI3_SS2 122
	#define OUT_ECSPI3_SS3 123
	
	#define OUT_ECSPI4_SS0 84
	
	#define OUT_ECSPI5_SS0 17
	#define OUT_ECSPI5_SS1 19
	#define OUT_ECSPI5_SS2 21
	#define OUT_ECSPI5_SS3 12




/*
//                                16  12  5
// The CCITT CRC 16 polynomial is X + X + X + 1.
// In binary, this is the bit pattern 1 0001 0000 0010 0001, and in hex it
//  is 0x11021.
// A 17 bit register is simulated by testing the MSB before shifting
//  the data, which affords us the luxury of specifiy the polynomial as a
//  16 bit value, 0x1021.
// Due to the way in which we process the CRC, the bits of the polynomial
//  are stored in reverse order. This makes the polynomial 0x8408.
*/

unsigned short crc16(unsigned char* data_p, unsigned char length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    
    crc ^= 0xFFFF;
    
    return crc;
}


#endif
