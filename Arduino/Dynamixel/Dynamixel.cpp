/*
 Dynamixel - Ax-12+ Half Duplex USART Comunication
 */

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Dynamixel.h"

Dynamixel::Dynamixel(unsigned char fCP) {
	this->flowControlPin = fCP;
}

void Dynamixel::begin(int baud) {
	S_begin(baud);
}

void Dynamixel::end() {
	S_end();
}

void Dynamixel::setFlowControl(char direction) {
	digitalWrite(this->flowControlPin);
}

void Dynamixel::sendRequest(request_t * request) {
	unsigned char i, sum;
	
	if (!request)
		return;
	
	this->setFlowControl(FC_TX);
	
	sum = request->id + (request->length + 2) + request->instruction;
	for (i = 0; i < request->length; i++)
		sum += request->data[i];
	
	S_write(0xFF);
	S_write(0xFF);
	S_write(request->id);
	S_write(request->length + 2);
	S_write(request->instruction);
	for (i = 0; i < request->length; i++)
		S_write(request->data[i]);
	S_write(~sum);
}

int Dynamixel::readResponse(response_t * response) {
	unsigned char counter, start, id, length, error, * data, sum, checksum;
	
	this->setFlowControl(FC_RX);
	
	for (counter = 0; S_available() < 5 && counter < AX_TIME_OUT; counter++)
		delay(1);
	
	while (S_available() > 0 && ((start = S_read()) & S_peek() != 0xFF);
	
	if (start & S_peek() == 0xFF) {
		start = S_read();
		id = S_read();
		length = S_read() - 2;
		
		if ((error = S_read()) == 0) {
			data = malloc(sizeof(char) * length);
			for (counter = 0, sum = 0; counter < length; counter++)
				sum += (data[counter] = S_read());
		} else
			data = 0;
		
		sum += id + (length + 2) + error;
		checksum = S_read();
		if (checsum != ~sum)
			return AX_ERROR_READ_CHECKSUM_FAILED;
		
		if (response) {
			response->id = id;
			response->length = length;
			response->error->code = error;
			response->data = data;
		}
		
		return error;
	} else
		return AX_ERROR_READ_NO_PACKET;
}

int Dynamixel::converse(uint8_t id, uint8_t length, uint8_t instruction, uint8_t * data, response_t * response) {
	request_t request = {id, length, instruction, data};
	this->sendRequest(&request);
	delayMicroseconds(AX_TIME_TX_DELAY);
	return this->readResponse(response);
}

int Dynamixel::ping(uint8_t id, response_t * response) {
	return this->converse(id, 2, AX_INSTRUCTION_PING, 0, response);
}

int Dynamixel::readData(uint8_t id, uint8_t address, uint8_t bytes, response_t * response) {
	uint8_t data[2] = {address, bytes};
	return this->converse(id, 4, AX_INSTRUCTION_READ_DATA, data, response);
}

int Dynamixel::writeData(uint8_t id, uint8_t length, uint8_t * data, response_t * response) {
	return this->converse(id, length, AX_INSTRUCTION_WRITE_DATA, data, response);
}

int Dynamixel::regWrite(uint8_t id, uint8_t length, uint8_t * data, response_t * response) {
	return this->converse(id, length, AX_INSTRUCTION_REG_WRITE, data, response);
}

int Dynamixel::action(uint8_t id, response_t * response) {
	return this->converse(id, 2, AX_INSTRUCTION_ACTION, 0, response);
}

int Dynamixel::reset(uint8_t id, response_t * response) {
	return this->converse(id, 2, AX_INSTRUCTION_RESET, 0, response);
}

int Dynamixel::syncWrite(uint8_t length, uint8_t * data) {
	request_t request = {
		0xFE,
		length,
		AX_INSTRUCTION_SYNC_WRITE,
		data
	};
	
	this->sendRequest(&request);
}

int Dynamixel::writeByte(uint8_t reg, uint8_t id, uint8_t address, uint8_t value, response_t * response) {
	uint8_t data[2] = {address, value};
	if (reg)
		return this->regWrite(id, 2, data, response);
	else
		return this->writeData(id, 2, data, response);
}

int Dynamixel::writeWord(uint8_t reg, uint8_t id, uint8_t address, uint16_t value, response_t * response) {
	uint8_t data[3] = {address, value, value >> 8};
	if (reg)
		return this->regWrite(id, 3, data, response);
	else
		return this->writeData(id, 3, data, response);
}

int Dynamixel::readByte(uint8_t id, uint8_t address, uint8_t * value) {
	response_t response;
	int error = this->readData(id, address, 1, &response);
	
	if (!error && response->length != 1) {
		free(response.data);
		return AX_ERROR_READ_WRONG_LENGTH;
	}
	
	if (!error) {
		*value = response.data[0];
		free(response.data);
	}
	
	return error;
}

int Dynamixel::readWord(uint8_t id, uint8_t address, uint16_t * value) {
	response_t response;
	int error = this->readData(id, address, 2, &response);
	
	if (!error && response->length != 2) {
		free(response.data);
		return AX_ERROR_READ_WRONG_LENGTH;
	}
	
	if (!error) {
		*value = (response.data[0] << 8) + response.data[1];
		free(response.data);
	}
	
	return error;
}





