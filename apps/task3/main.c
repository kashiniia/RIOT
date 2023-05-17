#include "xtimer.h"
#include "shell.h"
#include "bmx280.h"
#include "dht.h"
#include "dht_params.h"
#include "string.h"

#define DHT_PIN     GPIO_PIN(PORT_A, 0)
#define BME280_DEV  I2C_DEV(0)

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
static dht_t dht_dev;
int16_t dht_temp, dht_hum;
float dht_temperature, dht_humidity;

static int manage_command_line_arguments(int argc, char** argv) {
	if( argc == 2 ) {
		if (strcmp(argv[1], "bme") == 0) {
			return 0;
		} else if (strcmp(argv[1], "dht") == 0) {
			return 1;
		} else if (strcmp(argv[1], "both") == 0) {
			return 2;
		}
	} else if( argc > 2 ) {
		printf("Too many arguments.\n");
		return 3;
	} else {
		//no arguments
		return 4;
	}
	return 5;
}

static int cmd_get_temperature(int argc, char** argv) {
    float temperature_bme280;
    
    int argument = manage_command_line_arguments(argc, argv);
    
    if (argument == 0 || argument == 2) {
		temperature_bme280 = bmx280_read_temperature(&bmx280) / 100;
    	printf("BME280 temperature: %0.1f C\n", temperature_bme280);
    }
    
    if (argument == 1 || argument == 2) {
        //dht_read_temperature(&dev_dht, &temperature_dht);
		dht_read(&dht_dev, &dht_temp, &dht_hum);
		dht_temperature = dht_temp / 10;
        printf("DHT temperature: %0.1f C\n", dht_temperature);
    }
    
    if (argument == 4) {
		printf("temperature [dht|bme|both]\n");
    }

    return 0;
}

static int cmd_get_humidity(int argc, char** argv) {
    float humidity_bme280;
    
    int argument = manage_command_line_arguments(argc, argv);
    
    if (argument == 0 || argument == 2) {
		humidity_bme280 = bme280_read_humidity(&bmx280) / 100;
        printf("BME280 humidity: %0.1f%%\n", humidity_bme280);
    }
    
    if (argument == 1 || argument == 2) {
        dht_read(&dht_dev, &dht_temp, &dht_hum);
		dht_humidity = dht_temp / 10;
        printf("DHT humidity: %0.1f%%\n", dht_humidity);
    }
    
    if (argument == 4) {
		printf("humidity [dht|bme|both]\n");
    }
    
    return 0;
}

static int cmd_get_pressure(int argc, char** argv) {
    (void)argc;
    (void)argv;
    uint32_t pressure_bme280;
	pressure_bme280 = bmx280_read_pressure(&bmx280);
    printf("BME280 pressure: %lu Pa\n", pressure_bme280);
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "temperature", "Get temperature from sensors", cmd_get_temperature },
    { "humidity", "Get humidity from sensors", cmd_get_humidity },
    { "pressure", "Get pressure from sensors", cmd_get_pressure },
    { NULL, NULL, NULL }
};

int main(void)
{
    int8_t res;

    puts("RIOT application with shell");

    res = bmx280_init(&bmx280, &bmx280_params);
	if (res != BMX280_OK) {
        puts("Could not initialize BME280 sensor");
        return 1;
    }

	dht_params_t my_params;
	my_params.pin = GPIO_PIN(PORT_D, 4);
	my_params.in_mode = DHT_PARAM_PULL;
    res = dht_init(&dht_dev, &my_params);
    if (res != DHT_OK) {
        puts("Could not initialize DHT11 sensor");
        return 1;
    }

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}