# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# tinyusb
set(PICO_TINYUSB_PATH ${CMAKE_SOURCE_DIR}/lib/tinyusb)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_usb_i2s_speaker C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico_usb_i2s_speaker
        main.c
        usb_descriptors.c
        )

pico_set_program_name(pico_usb_i2s_speaker "pico_usb_i2s_speaker")
pico_set_program_version(pico_usb_i2s_speaker "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pico_usb_i2s_speaker 0)
pico_enable_stdio_usb(pico_usb_i2s_speaker 0)

# Add the standard library to the build
target_link_libraries(pico_usb_i2s_speaker
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(pico_usb_i2s_speaker PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

set(I2S_MAX_FREQ_KHZ 96)
set(I2S_QUEUE_LEN 10)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/pico-i2s-pio)

# Add any user requested libraries
target_link_libraries(pico_usb_i2s_speaker
        hardware_clocks
        hardware_dma
        pico_multicore
        tinyusb_device
        tinyusb_board
        pico-i2s-pio
        )

# Hardware workarounds
pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(pico_usb_i2s_speaker PRIVATE
        # Startup delay to avoid issue with QSPI flash
        PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64

        # Performance, avoid calls to ____wrap___aeabi_lmul_veneer when doing 64bit multiplies
        PICO_INT64_OPS_IN_RAM=1
)
target_compile_definitions(slower_boot2 PRIVATE
        #MG - 7/30/2024
        #System clock set to 295MHz by i2s lib: PICO_FLASH_SPI_CLKDIV=2 by default -> 147.5MHz clk is above 133MHz flash memory spec
        #Drop QSPI clock to avoid errors when loading program from flash (295MHz / 4 = 73.75MHz)
        # PICO_FLASH_SPI_CLKDIV=2 can still be used if voltage is raised to >=1.25V
        PICO_FLASH_SPI_CLKDIV=4
)
pico_set_boot_stage2(pico_usb_i2s_speaker slower_boot2)

pico_add_extra_outputs(pico_usb_i2s_speaker)

