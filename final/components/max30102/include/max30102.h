/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include "esp_err.h"
#include "driver/i2c.h"

typedef void *max30102_handle_t;

typedef struct {
    bool hand_detected;
    float heart_rate;
    float spo2;
} max30102_data_t;

/**
 * @brief Create a new MAX30102 sensor
 *
 * This function creates a new MAX30102 sensor and initializes the I2C bus.
 *
 * @param port The I2C port number
 * @param handle A pointer to the handle for the MAX30102 sensor
 * @return ESP_OK if successful, an error code otherwise
 */
esp_err_t max30102_create(i2c_port_t port, max30102_handle_t *handle);

/**
 * @brief Configure the MAX30102 sensor
 *
 * This function configures the MAX30102 sensor with the default settings.
 *
 * @param sensor The handle to the MAX30102 sensor
 * @return ESP_OK if successful, an error code otherwise
 */
esp_err_t max30102_config(max30102_handle_t sensor);

/**
 * @brief Deinitialize the MAX30102 sensor
 *
 * This function deinitializes the MAX30102 sensor and frees the memory allocated for the sensor.
 *
 * @param sensor The handle to the MAX30102 sensor
 * @return ESP_OK if successful, an error code otherwise
 */
esp_err_t max30102_deinit(max30102_handle_t sensor);

/**
 * @brief Get data from the MAX30102 sensor
 *
 * This function reads data from the MAX30102 sensor, processes the data, and stores the results in the provided data structure.
 * The processing includes reading the FIFO write and read pointers, calculating the number of samples, reading the FIFO data register,
 * and applying a FIR filter to the data.
 *
 * @param sensor The handle to the MAX30102 sensor
 * @param data A pointer to the data structure where the results will be stored
 * @return ESP_OK if successful, an error code otherwise
 */
esp_err_t max30102_get_data(max30102_handle_t sensor, max30102_data_t *data);

#ifdef __cplusplus
} /* end of extern "C" */
#endif