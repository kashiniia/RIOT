# name of your application
APPLICATION = task2

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f401re

USEMODULE += xtimer

USEMODULE += periph_gpio
USEMODULE += periph_i2c

USEMODULE += bme280_i2c

USEMODULE += dht
USEMODULE += fmt

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
