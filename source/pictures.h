/*
 * pictures.h
 *
 *  Created on: 14 maj 2024
 *      Author: purch
 */

#ifndef PICTURES_H_
#define PICTURES_H_

#include <stdint.h>

typedef struct {
	const uint8_t rowCount;
	const uint64_t *dataBuffer;
} pictureData_t;

extern const pictureData_t g_pictureData;

#endif /* PICTURES_H_ */
