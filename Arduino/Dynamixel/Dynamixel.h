/*
 Dynamixel - Ax-12+ Half Duplex USART Comunication
 */

#ifndef Dynamixel_h
#define Dynamixel_h

#include <inttypes.h>




// Serial Definitions & Macros

#ifndef SER_OBJ
 #define SER_OBJ	Serial
#endif

#ifndef SERIAL_MACROS
 #define SERIAL_MACROS
 #define S_begin(ags)	SER_OBJ.begin(args)
 #define S_available()	SER_OBJ.available()
 #define S_peek()		SER_OBJ.peek()
 #define S_read()		SER_OBJ.read()
 #define S_write(args)	SER_OBJ.write(args)
 #define S_end()		SER_OBJ.end()
#endif



// Timing

#define	AX_TIME_OUT			10
#define AX_TIME_TX_DELAY	400



// Flow Control

#define		FC_RX	0
#define		FC_TX	1



// Instructions

#define AX_INSTRUCTION_PING					0x01
#define AX_INSTRUCTION_READ_DATA			0x02
#define AX_INSTRUCTION_WRITE_DATA			0x03
#define AX_INSTRUCTION_REG_WRITE			0x04
#define AX_INSTRUCTION_ACTION				0x05
#define AX_INSTRUCTION_RESET				0x06
#define AX_INSTRUCTION_SYNC_WRITE			0x83



// Error Codes

#define AX_ERROR_READ_NO_PACKET			-1
#define AX_ERROR_READ_CHECKSUM_FAILED	-2
#define AX_ERROR_READ_WRONG_LENGTH		-3



// Addresses

#define AX_MEM_EEPROM_MODEL_NUMBER_L		0x00
#define AX_MEM_EEPROM_MODEL_NUMBER_H		0x01
#define AX_MEM_EEPROM_FIRMWARE_VERSION		0x02
#define AX_MEM_EEPROM_IDENTIFIER			0x03
#define AX_MEM_EEPROM_BAUD_RATE				0x04
#define AX_MEM_EEPROM_RET_DELAY_TIME		0x05
#define AX_MEM_EEPROM_CW_ANGLE_LIMIT_L		0x06
#define AX_MEM_EEPROM_CW_ANGLE_LIMIT_H		0x07
#define AX_MEM_EEPROM_CCW_ANGLE_LIMIT_L		0x08
#define AX_MEM_EEPROM_CCW_ANGLE_LIMIT_H		0x09
#define AX_MEM_EEPROM_INTERNAL_TEMP_LIMIT	0x0A
#define AX_MEM_EEPROM_LOWEST_TEMP_LIMIT		0x0B
#define AX_MEM_EEPROM_HIGHEST_TEMP_LIMIT	0x0C
#define AX_MEM_EEPROM_MAX_TORQUE_L			0x0D
#define AX_MEM_EEPROM_MAX_TORQUE_H			0x0E
#define AX_MEM_EEPROM_STATUS_RETURN_LEVEL	0x0F
#define AX_MEM_EEPROM_ALARM_LED				0x10
#define AX_MEM_EEPROM_ALARM_SHUTDOWN		0x11

#define AX_MEM_RAM_TORQUE_ENABLE			0x18
#define AX_MEM_RAM_LED						0x19
#define AX_MEM_RAM_CW_COMPLIANCE_MARGIN 	0x1A
#define AX_MEM_RAM_CCW_COMPLIANCE_MARGIN	0x1B
#define AX_MEM_RAM_CW_COMPLIANCE_SLOPE		0x1C
#define AX_MEM_RAM_CCW_COMPLIANCE_SLOPE		0x1D
#define AX_MEM_RAM_GOAL_POSITION_L			0x1E
#define AX_MEM_RAM_GOAL_POSITION_H			0x1F
#define AX_MEM_RAM_MOVING_SPEED_L			0x20
#define AX_MEM_RAM_MOVING_SPEED_H			0x21
#define AX_MEM_RAM_TORQUE_LIMIT_L			0x22
#define AX_MEM_RAM_TORQUE_LIMIT_H			0x23
#define AX_MEM_RAM_PRESENT_POSITION_L		0x24
#define AX_MEM_RAM_PRESENT_POSITION_H		0x25
#define AX_MEM_RAM_PRESENT_SPEED_L			0x26
#define AX_MEM_RAM_PRESENT_SPEED_H			0x27
#define AX_MEM_RAM_PRESENT_LOAD_L			0x28
#define AX_MEM_RAM_PRESENT_LOAD_H			0x29
#define AX_MEM_RAM_PRESENT_VOLTAGE			0x2A
#define AX_MEM_RAM_PRESENT_TEMPERATURE		0x2B
#define AX_MEM_RAM_REGISTERED				0x2C
#define AX_MEM_RAM_MOVING					0x2D
#define AX_MEM_RAM_LOCK						0x2F
#define AX_MEM_RAM_PUNCH_L					0x30
#define AX_MEM_RAM_PUNCH_H					0x31

#define AX_MEM_EEPROM_MODEL_NUMBER			0x00
#define AX_MEM_EEPROM_CW_ANGLE_LIMIT		0x06
#define AX_MEM_EEPROM_CCW_ANGLE_LIMIT		0x08
#define AX_MEM_EEPROM_MAX_TORQUE			0x0D
#define AX_MEM_RAM_GOAL_POSITION			0x1E
#define AX_MEM_RAM_MOVING_SPEED				0x20
#define AX_MEM_RAM_TORQUE_LIMIT				0x22
#define AX_MEM_RAM_PRESENT_POSITION			0x24
#define AX_MEM_RAM_PRESENT_SPEED			0x26
#define AX_MEM_RAM_PRESENT_LOAD				0x29
#define AX_MEM_RAM_PUNCH					0x30


// Types

#pragma pack(push, 1)

typedef union {
	uint8_t code;
	struct {
		uint8_t unused:1;
		uint8_t instruction:1;
		uint8_t overload:1;
		uint8_t checksum:1;
		uint8_t range:1;
		uint8_t overheat:1;
		uint8_t angle_limit:1;
		uint8_t input_voltage:1;
	} bits;
} error_t;

typedef struct {
	uint8_t id;
	uint8_t length;
	uint8_t instruction;
	uint8_t * data;
} request_t;

typedef struct {
	uint8_t id;
	uint8_t length;
	error_t error;
	uint8_t * data;
} response_t;

#pragma pack(pop)




class Dynamixel {
private:
	unsigned char flowControlPin;

	void setFlowControl(char direction);

public:
	Dynamixel(unsigned char fCP);

	void sendRequest(request_t * request);
	int readResponse(response_t * response);

};

#endif
