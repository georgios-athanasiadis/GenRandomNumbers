#ifndef DFROBOT_GNSS_H_
#define DFROBOT_GNSS_H_

#include "stm32h7xx_hal.h"


#define GNSS_DEVICE_ADDR  0x20U

#define I2C_YEAR_H        0U
#define I2C_YEAR_L        1U
#define I2C_MONTH         2U
#define I2C_DATE          3U
#define I2C_HOUR          4U
#define I2C_MINUTE        5U
#define I2C_SECOND        6U

#define I2C_LAT_1         7U
#define I2C_LAT_2         8U
#define I2C_LAT_X_24      9U
#define I2C_LAT_X_16      10U
#define I2C_LAT_X_8       11U
#define I2C_LON_DIS       12U

#define I2C_LON_1         13U
#define I2C_LON_2         14U
#define I2C_LON_X_24      15U
#define I2C_LON_X_16      16U
#define I2C_LON_X_8       17U
#define I2C_LAT_DIS       18U

#define I2C_ID            30U
#define I2C_GNSS_MODE     34U
#define I2C_SLEEP_MODE    35U
#define I2C_RGB_MODE      36U

#define TIME_OUT          500U

#define ENABLE_POWER      0U
#define RGB_ON            0x05U


typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t date;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} sTim_t;

typedef struct
{
    uint8_t lonDDD;
    uint8_t lonMM;
    uint32_t lonMMMMM;
    char lonDirection;

    uint8_t latDD;
    uint8_t latMM;
    uint32_t latMMMMM;
    char latDirection;

    double latitude;
    double latitudeDegree;
    double lonitude;
    double lonitudeDegree;
} sLonLat_t;

typedef enum
{
    eGPS = 1,
    eBeiDou,
    eGPS_BeiDou,
    eGLONASS,
    eGPS_GLONASS,
    eBeiDou_GLONASS,
    eGPS_BeiDou_GLONASS
} eGnssMode_t;




HAL_StatusTypeDef GNSS_Begin(
    UART_HandleTypeDef *huart);

HAL_StatusTypeDef GNSS_EnablePower(void);

HAL_StatusTypeDef GNSS_SetGnss(
    eGnssMode_t mode);

HAL_StatusTypeDef GNSS_SetRgbOn(void);

HAL_StatusTypeDef GNSS_GetUTC(
    sTim_t *utc);

HAL_StatusTypeDef GNSS_GetDate(
    sTim_t *date);

HAL_StatusTypeDef GNSS_GetLat(
    sLonLat_t *latitude);

HAL_StatusTypeDef GNSS_GetLon(
    sLonLat_t *longitude);

#endif
