#include "spi_transfer.h"
#include "valves_spi_cfg.h"
#include <fsl_spi.h>

status_t sendDataToValves( const uint64_t data )
{
    static uint64_t       readData       = 0;
    static spi_transfer_t valvesTransfer = { .txData      = NULL,
                                             .rxData      = (uint8_t *)&readData,
                                             .dataSize    = VALVE_BUFFER_SIZE,
                                             .configFlags = kSPI_FrameAssert };

    valvesTransfer.txData = (uint8_t *)&data;

    return SPI_MasterTransferBlocking( VALVES_SPI_MASTER, &valvesTransfer );
}
