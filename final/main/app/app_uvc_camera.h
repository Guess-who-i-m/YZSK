/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#ifndef _APP_UVC_CANMERA_H_
#define _APP_UVC_CANMERA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DEMO_UVC_FRAME_WIDTH        320
#define DEMO_UVC_FRAME_HEIGHT       240
#define DEMO_UVC_XFER_BUFFER_SIZE   (30 * 1024)

typedef enum {
    PIXFORMAT_RGB565,    // 2BPP/RGB565
    PIXFORMAT_YUV422,    // 2BPP/YUV422
    PIXFORMAT_GRAYSCALE, // 1BPP/GRAYSCALE
    PIXFORMAT_JPEG,      // JPEG/COMPRESSED
    PIXFORMAT_RGB888,    // 3BPP/RGB888
    PIXFORMAT_RAW,       // RAW
    PIXFORMAT_RGB444,    // 3BP2P/RGB444
    PIXFORMAT_RGB555,    // 3BP2P/RGB555
} pixformat_t;


/**
 * @brief Data structure of camera frame buffer
 */
typedef struct {
    uint8_t *       buf;            /*!< Pointer to the pixel data */
    size_t          len;            /*!< Length of the buffer in bytes */
    size_t          width;          /*!< Width of the buffer in pixels */
    size_t          height;         /*!< Height of the buffer in pixels */
    pixformat_t     format;         /*!< Format of the pixel data */
    struct timeval  timestamp;      /*!< Timestamp since boot of the first DMA buffer of the frame */
} camera_fb_t;

/**
 * @brief Initialize the UVC (USB Video Class) camera
 *
 * This function initializes the UVC camera by allocating memory for the transfer and frame buffers, configuring the UVC streaming with the specified parameters,
 * registering a state callback to get connect/disconnect events, and starting the UVC streaming.
 */
void app_uvc_camera_init(void);

/**
 * @brief Suspend the UVC (USB Video Class) streaming
 *
 * This function sends a suspend control command to the UVC streaming. If the command fails, it logs an error message.
 */
void app_uvc_suspend(void);

/**
 * @brief Resume the UVC (USB Video Class) streaming
 *
 * This function sends a resume control command to the UVC streaming. If the command fails, it logs an error message.
 */
void app_uvc_resume(void);

/**
 * @brief Get the current frame from the UVC (USB Video Class) streaming
 *
 * This function returns a pointer to the current frame buffer.
 *
 * @return A pointer to the current frame buffer.
 */

void camera_task(void *pvParameters);
uint8_t* app_uvc_get_frame();

#ifdef __cplusplus
}
#endif

#endif
