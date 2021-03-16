#include "mcp_can.h"

/*********************************************************************************************************
** Function name:           MCP_CAN
** Descriptions:            Constructor
*********************************************************************************************************/
MCP_CAN::MCP_CAN(byte _CS)
{
    *pSPI = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */;
    init_CS(_CS);
}

/*********************************************************************************************************
** Function name:           init_CS
** Descriptions:            init CS pin and set UNSELECTED
*********************************************************************************************************/
void MCP_CAN::init_CS(byte _CS)
{
    if (_CS == 0)
    {
        return;
    }
    SPICS = _CS;
//    pinMode(SPICS, OUTPUT);
//    digitalWrite(SPICS, HIGH);
}

void MCP_CAN::setSPI(nrf_drv_spi_t *_pSPI)
{
    pSPI = _pSPI; // define SPI port to use before begin()
}

// nrf spi event handler
void MCP_CAN::spiEventHandler()
{
}

/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    MCP_CAN::spi_xfer_done = true;
    // TODO
}

// Initialize nrf spi instance
void MCP_CAN::initSPI()
{
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = SPI_SS_PIN;
    spi_config.miso_pin = SPI_MISO_PIN;
    spi_config.mosi_pin = SPI_MOSI_PIN;
    spi_config.sck_pin  = SPI_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(pSPI, &spi_config, spi_event_handler, NULL));
}