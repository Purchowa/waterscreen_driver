/*
 * spi_transfer.h
 *
 *  Created on: 20 maj 2024
 *      Author: purch
 */

#ifndef SPI_TRANSFER_H_
#define SPI_TRANSFER_H_
#include <stdint.h>

typedef int32_t status_t;

status_t sendDataToValves(const uint64_t* data);

#endif /* SPI_TRANSFER_H_ */
