#include "mcp_can.h"

#include "app_error.h"
#include "nrf_gpio.h"

#include "cstring"

#include "SEGGER_RTT.h"

/*********************************************************************************************************
** Function name:           MCP_CAN
** Descriptions:            Constructor
*********************************************************************************************************/
MCP_CAN::MCP_CAN(byte _CS)
{
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
    nrf_gpio_cfg_output(SPICS); // set chip select
}

void MCP_CAN::setSPI(nrf_drv_spi_t *_pSPI)
{
    pSPI = *_pSPI; // define SPI port to use before begin()
}

/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    spi_xfer_done = true;
    // TODO
}

// Initialize nrf spi instance
void MCP_CAN::initSPI()
{
    this->spiInitialized = false;

    pSPI = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */;

    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = SPI_SS_PIN;
    spi_config.miso_pin = SPI_MISO_PIN;
    spi_config.mosi_pin = SPI_MOSI_PIN;
    spi_config.sck_pin  = SPI_SCK_PIN;
    spi_config.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;

    APP_ERROR_CHECK(nrf_drv_spi_init(&pSPI, &spi_config, spi_event_handler, NULL));

    SEGGER_RTT_printf(0, "Initialized SPI ... \n");

    this->spiInitialized = true;

}

// Set nrf spi configuration
void MCP_CAN::setSPIConfig(nrf_drv_spi_config_t * _pConfig)
{
  APP_ERROR_CHECK(nrf_drv_spi_init(&pSPI, _pConfig, spi_event_handler, NULL));
}

// SPI read write
byte MCP_CAN::spi_readwrite(const byte buf)
{
  uint8_t p_tx_data;
  uint8_t p_rx_data;
  const uint16_t len = 0;
  p_tx_data = buf;

  p_rx_data = nrf_drv_spi_transfer(&pSPI, &p_tx_data, sizeof(p_tx_data), &p_rx_data, len);
  return p_rx_data; //NOTE: Returning just the first byte in RX buf

//  spi_m_length = 0;
//  memset(&spi_m_tx_buf, buf, sizeof(buf));
//  nrf_drv_spi_transfer(&pSPI, spi_m_tx_buf, sizeof(spi_m_tx_buf), spi_m_rx_buf, spi_m_length);
//  return spi_m_rx_buf[0]; //NOTE: Returning just the first byte in RX buf
}

byte MCP_CAN::spi_read()
{
  return spi_readwrite(0x00);
}

byte MCP_CAN::spi_write(const byte buf)
{
  return spi_readwrite(buf);
}