#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include <string.h> // memset

#include "mcp3008.h"

/* Pin definitions, taken from example, any GPIO can be used
 * AFAIK */
#define PIN_NUM_MISO   12
#define PIN_NUM_MOSI   13
#define PIN_NUM_CLK    14
#define PIN_NUM_CS0    15

static spi_device_handle_t m_deviceHandle;
static const char* TAG = "MCP3008";

static void init_spi_bus(void);
static void init_mcp3008_dev(void);

void mcp3008_init()
{
    init_spi_bus();

    init_mcp3008_dev();
}

static void init_spi_bus(void)
{
    spi_bus_config_t busConfig;    

    busConfig.miso_io_num = PIN_NUM_MISO;
    busConfig.mosi_io_num = PIN_NUM_MOSI;
    busConfig.sclk_io_num = PIN_NUM_CLK;

    /* Unused pins */
    busConfig.quadhd_io_num = -1;
    busConfig.quadwp_io_num = -1;

    /* Max transfer size defaults to 4096, see Docs */
    busConfig.max_transfer_sz = 0;

    /* Do not use DMA */
    ESP_ERROR_CHECK(spi_bus_initialize(HSPI_HOST, &busConfig, 0));

}

static void init_mcp3008_dev(void)
{
    spi_device_interface_config_t devConfig = {
          .command_bits       = 5 
          , .address_bits     = 3
          , .dummy_bits       = 0
          , .mode             = 0
          , .duty_cycle_pos   = 0
          , .cs_ena_pretrans  = 0
          , .cs_ena_posttrans = 0
          , .clock_speed_hz   = SPI_MASTER_FREQ_8M
          , .spics_io_num     = PIN_NUM_CS0
          , .flags            = 0
          , .queue_size       = 8
    };

    ESP_ERROR_CHECK(spi_bus_add_device(HSPI_HOST, &devConfig, &m_deviceHandle));
}

uint16_t mcp3008_get_raw(uint8_t channel)
{
    spi_transaction_t trans;
    uint8_t bufferTx[2] = {0,};
    uint8_t bufferRx[2] = {0,};
    uint16_t ret;

    memset(&trans, 0, sizeof(trans));
    
    trans.cmd = 0x03; 
    trans.addr = channel;
    trans.length = 16;
    trans.tx_buffer = (void*)&bufferTx;
    trans.rx_buffer = (void*)&bufferRx;
    
    ESP_LOGI(TAG, "Getting raw data for channel %u", channel);

    ESP_ERROR_CHECK(spi_device_transmit(m_deviceHandle, &trans));

    /* First bit is sample hold, next bit is NULL bit, the B9-B0 */
    ret = (bufferRx[0] & 0x3F) << 4  | ((bufferRx[1] & 0xF0) >> 4);

    ESP_LOGI(TAG, "Got [%02X] [%02X]", bufferRx[0], bufferRx[1]);
   
    return ret;
}
