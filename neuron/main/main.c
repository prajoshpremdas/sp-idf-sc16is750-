/* The example of SC16IS750
 *
 * This code is in the public domain.
 */

#include <ctype.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "sc16is750.h"

#define TAG "MAIN-NEURON"

void echo(void *pvParameters)
{
	ESP_LOGI(TAG, "Your module is SC16IS750");
	int channels = SC16IS750_SINGLE_CHANNEL;
	long crystal_freq = 14745600UL;
	uint32_t baud_A = CONFIG_BAUDRATE1;
	uint32_t baud_B = SC16IS750_CHANNEL_NONE;

	SC16IS750_t dev;

	ESP_LOGI(TAG, "INTERFACE is SPI");
	ESP_LOGI(TAG, "CONFIG_CS_GPIO=%d",CONFIG_CS_GPIO);
	ESP_LOGI(TAG, "CONFIG_MISO_GPIO=%d",CONFIG_MISO_GPIO);
	ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d",CONFIG_MOSI_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d",CONFIG_SCLK_GPIO);

	SC16IS750_init(&dev, SC16IS750_PROTOCOL_SPI, CONFIG_CS_GPIO, channels);
	SC16IS750_spi(&dev, CONFIG_MOSI_GPIO, CONFIG_MISO_GPIO, CONFIG_SCLK_GPIO);

	ESP_LOGI(TAG, "Communication speed of channel 1 is %d", CONFIG_BAUDRATE1);

 #if 0
	if (SC16IS750_ping(&dev)!=1) {
		ESP_LOGE(TAG, "device not found");
		while(1) { vTaskDelay(1); }
	} else {
		ESP_LOGI(TAG, "device found");
	}
	ESP_LOGI(TAG, "start serial communication");
#endif

 #if 0
	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_LCR, 0x83);

	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_DLL, 0x05);
	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_DLH, 0x00);

	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_FCR, 0x06);
	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_FCR, 0x01);

	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_LCR, 0xBF);

	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_EFR, 0x10);

	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_LCR, 0x03);

	SC16IS750_WriteRegister(&dev, SC16IS750_CHANNEL_A, SC16IS750_REG_EFCR, 0x00);

	SC16IS750_write(&dev, SC16IS750_CHANNEL_A, 'P');
#endif

#if 1
	SC16IS750_begin(&dev, baud_A, baud_B, crystal_freq); //baudrate&frequency setting
	if (SC16IS750_ping(&dev)!=1) {
		ESP_LOGE(TAG, "device not found");
		while(1) { vTaskDelay(1); }
	} else {
		ESP_LOGI(TAG, "device found");
	}
	ESP_LOGI(TAG, "start serial communication");
	SC16IS750_write(&dev, SC16IS750_CHANNEL_A, 'S');
	SC16IS750_write(&dev, SC16IS750_CHANNEL_A, 'T');
	SC16IS750_write(&dev, SC16IS750_CHANNEL_A, 'A');
	SC16IS750_write(&dev, SC16IS750_CHANNEL_A, 'R');
	SC16IS750_write(&dev, SC16IS750_CHANNEL_A, 'T');
	SC16IS750_write(&dev, SC16IS750_CHANNEL_A, '\n');
	SC16IS750_write(&dev, SC16IS750_CHANNEL_A, '\r');
#endif

#if 1
	char buffer_A[64] = {0};
	int index_A = 0;
	char c;
#endif

	while(1) {
#if 1
		if (SC16IS750_available(&dev, SC16IS750_CHANNEL_A)) {
			c = SC16IS750_read(&dev, SC16IS750_CHANNEL_A);

			if (index_A < sizeof(buffer_A)-1) {
				if (c != 0) {
					buffer_A[index_A++] = (c);
					buffer_A[index_A] = 0;
				}
			}
		} else {
			if (index_A != 0) ESP_LOGI(TAG, "buffer_A=[%s]", buffer_A);
			for (int i=0;i<index_A;i++) {
				if (isupper((int)buffer_A[i])) {
					buffer_A[i] = tolower(buffer_A[i]);
				} else {
					buffer_A[i] = toupper(buffer_A[i]);
				}
				SC16IS750_write(&dev, SC16IS750_CHANNEL_A, buffer_A[i]);
			}
			buffer_A[0] = 0;
			index_A = 0;

			// Wait for transmission to complete
			while(1) {
				uint8_t state = SC16IS750_linestate(&dev, SC16IS750_CHANNEL_A);
				if ( (state & 0x40) == 0x40) break; // THR and TSR empty.
				ESP_LOGI(TAG, "Channel_A state=0x%02X", state);
				vTaskDelay(1);
			} // end while

		} // end CHENNEL_A
#endif
		vTaskDelay(1);
	}
	vTaskDelete(NULL);
}

void app_main()
{
	xTaskCreate(&echo, "ECHO", 1024*4, NULL, 2, NULL);
}
