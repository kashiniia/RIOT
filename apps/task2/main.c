#include <stdio.h>
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph_conf.h"
#include "bmx280.h"
#include "dht.h"
#include "dht_params.h"
#include "xtimer.h"

static bmx280_t bmx280;
static const bmx280_params_t bmx280_params = {
		.i2c_dev = I2C_DEV(0),
		.i2c_addr = 0x77,
		.t_sb = BMX280_SB_0_5,
		.filter = BMX280_FILTER_OFF,
		.run_mode = BMX280_MODE_FORCED,
		.temp_oversample = BMX280_OSRS_X1,
		.press_oversample = BMX280_OSRS_X1,
		.humid_oversample = BMX280_OSRS_X1,
};

int main(void)
{
	/* Инициализация датчика BME280 */
	int bme280_res = bmx280_init(&bmx280, &bmx280_params);
	if (bme280_res != BMX280_OK) {
		printf("Ошибка инициализации BME280: %d\n", bme280_res);
		return 1;
	}

	/* Инициализация датчика DHT11 */
	dht_params_t my_params;
	my_params.pin = GPIO_PIN(PORT_D, 4);
	my_params.in_mode = DHT_PARAM_PULL;

	//gpio_init(DHT_GPIO_PIN, GPIO_IN);
	dht_t dht_dev;
	int dht_res = dht_init(&dht_dev, &my_params);
	if (dht_res != 0) {
		printf("Ошибка инициализации DHT11: %d\n", dht_res);
		return 1;
	}

	/* Получение и вывод показаний датчиков */
	while (1) {
		/* Получение показаний BME280 */
		float bme280_temperature = bmx280_read_temperature(&bmx280) / 100; /* degrees C * 100 -> degrees C*/
		float bme280_humidity = bme280_read_humidity(&bmx280) / 100; /* percents * 100 / percents*/
		float bme280_pressure = bmx280_read_pressure(&bmx280) / 100; /* Pa -> mbar */

		/* Получение показаний DHT11 */
		int16_t temp, hum;
		dht_res = dht_read(&dht_dev, &temp, &hum);
		float dht_temperature = temp / 10;
		float dht_humidity = hum / 10;
		
		if (dht_res != 0) {
			printf("Ошибка чтения DHT11: %d\n", dht_res);
		} else {


			/* Сравнение показаний */
			printf("BME280: температура=%.1f°C, влажность=%.1f%%, давление=%.1f мбар\n",
					bme280_temperature, bme280_humidity, bme280_pressure);
			printf("DHT11: температура=%.1f°C, влажность=%.1f%%\n",
					dht_temperature, dht_humidity);
			printf("\n");
		}
		xtimer_sleep(5);
	}

	return 0;
}
