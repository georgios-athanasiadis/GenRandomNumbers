#include "dfrobot_gnss.h"


#define GNSS_COMMAND_DELAY_MS  500U

static UART_HandleTypeDef *gnss_uart;


static HAL_StatusTypeDef GNSS_ReadRegister(
    uint8_t reg,
    uint8_t *data,
    uint8_t len)
{
    uint8_t request[2] = {
        (uint8_t)(reg & 0x7FU),
        len
    };
    HAL_StatusTypeDef status;

    if (gnss_uart == 0)
    {
        return HAL_ERROR;
    }

    __HAL_UART_CLEAR_OREFLAG(gnss_uart);
    __HAL_UART_SEND_REQ(
        gnss_uart,
        UART_RXDATA_FLUSH_REQUEST);

    status = HAL_UART_Transmit(
        gnss_uart,
        request,
        (uint16_t)sizeof(request),
        TIME_OUT);

    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_UART_Receive(
        gnss_uart,
        data,
        len,
        TIME_OUT);

    if (status == HAL_OK)
    {
        HAL_Delay(GNSS_COMMAND_DELAY_MS);
    }

    return status;
}


static HAL_StatusTypeDef GNSS_WriteByte(
    uint8_t reg,
    uint8_t value)
{
    uint8_t request[2] = {
        (uint8_t)(reg | 0x80U),
        value
    };

    if (gnss_uart == 0)
    {
        return HAL_ERROR;
    }

    return HAL_UART_Transmit(
        gnss_uart,
        request,
        (uint16_t)sizeof(request),
        TIME_OUT);
}


HAL_StatusTypeDef GNSS_Begin(UART_HandleTypeDef *huart)
{
    uint8_t id = 0U;
    HAL_StatusTypeDef status;

    gnss_uart = huart;

    status = GNSS_ReadRegister(
        I2C_ID,
        &id,
        1U);

    if (status != HAL_OK)
    {
        gnss_uart = 0;
        return status;
    }

    if (id != GNSS_DEVICE_ADDR)
    {
        gnss_uart = 0;
        return HAL_ERROR;
    }

    return HAL_OK;
}


HAL_StatusTypeDef GNSS_EnablePower(void)
{
    HAL_StatusTypeDef status;

    status = GNSS_WriteByte(
        I2C_SLEEP_MODE,
        ENABLE_POWER);

    if (status == HAL_OK)
    {
        HAL_Delay(GNSS_COMMAND_DELAY_MS);
    }

    return status;
}


HAL_StatusTypeDef GNSS_SetGnss(eGnssMode_t mode)
{
    HAL_StatusTypeDef status;

    status = GNSS_WriteByte(
        I2C_GNSS_MODE,
        (uint8_t)mode);

    if (status == HAL_OK)
    {
        HAL_Delay(GNSS_COMMAND_DELAY_MS);
    }

    return status;
}


HAL_StatusTypeDef GNSS_SetRgbOn(void)
{
    HAL_StatusTypeDef status;

    status = GNSS_WriteByte(
        I2C_RGB_MODE,
        RGB_ON);

    if (status == HAL_OK)
    {
        HAL_Delay(GNSS_COMMAND_DELAY_MS);
    }

    return status;
}


HAL_StatusTypeDef GNSS_GetUTC(sTim_t *utc)
{
    uint8_t raw[3];
    HAL_StatusTypeDef status;

    if (utc == 0)
    {
        return HAL_ERROR;
    }

    status = GNSS_ReadRegister(
        I2C_HOUR,
        raw,
        (uint8_t)sizeof(raw));

    if (status != HAL_OK)
    {
        return status;
    }

    utc->hour = raw[0];
    utc->minute = raw[1];
    utc->second = raw[2];

    return HAL_OK;
}


HAL_StatusTypeDef GNSS_GetDate(sTim_t *date)
{
    uint8_t raw[4];
    HAL_StatusTypeDef status;

    if (date == 0)
    {
        return HAL_ERROR;
    }

    status = GNSS_ReadRegister(
        I2C_YEAR_H,
        raw,
        (uint8_t)sizeof(raw));

    if (status != HAL_OK)
    {
        return status;
    }

    date->year =
        ((uint16_t)raw[0] << 8U) |
        (uint16_t)raw[1];

    date->month = raw[2];
    date->date = raw[3];

    return HAL_OK;
}


HAL_StatusTypeDef GNSS_GetLat(sLonLat_t *latitude)
{
    uint8_t raw[6];
    HAL_StatusTypeDef status;

    if (latitude == 0)
    {
        return HAL_ERROR;
    }

    status = GNSS_ReadRegister(
        I2C_LAT_1,
        raw,
        (uint8_t)sizeof(raw));

    if (status != HAL_OK)
    {
        return status;
    }

    latitude->latDD = raw[0];
    latitude->latMM = raw[1];

    latitude->latMMMMM =
        ((uint32_t)raw[2] << 16U) |
        ((uint32_t)raw[3] << 8U) |
        (uint32_t)raw[4];

    /*
     * Регистры 13–18:
     * пять байтов долготы и направление широты.
     */
    status = GNSS_ReadRegister(
        I2C_LON_1,
        raw,
        (uint8_t)sizeof(raw));

    if (status != HAL_OK)
    {
        return status;
    }

    latitude->latDirection = (char)raw[5];

    latitude->latitude =
        ((double)latitude->latDD * 100.0) +
        (double)latitude->latMM +
        ((double)latitude->latMMMMM / 100000.0);

    latitude->latitudeDegree =
        (double)latitude->latDD +
        ((double)latitude->latMM / 60.0) +
        ((double)latitude->latMMMMM / 100000.0 / 60.0);

    return HAL_OK;
}


HAL_StatusTypeDef GNSS_GetLon(sLonLat_t *longitude)
{
    uint8_t raw[6];
    HAL_StatusTypeDef status;

    if (longitude == 0)
    {
        return HAL_ERROR;
    }

    status = GNSS_ReadRegister(
        I2C_LON_1,
        raw,
        (uint8_t)sizeof(raw));

    if (status != HAL_OK)
    {
        return status;
    }

    longitude->lonDDD = raw[0];
    longitude->lonMM = raw[1];

    longitude->lonMMMMM =
        ((uint32_t)raw[2] << 16U) |
        ((uint32_t)raw[3] << 8U) |
        (uint32_t)raw[4];

    /*
     * Регистры 7–12:
     * пять байтов широты и направление долготы.
     */
    status = GNSS_ReadRegister(
        I2C_LAT_1,
        raw,
        (uint8_t)sizeof(raw));

    if (status != HAL_OK)
    {
        return status;
    }

    longitude->lonDirection = (char)raw[5];

    longitude->lonitude =
        ((double)longitude->lonDDD * 100.0) +
        (double)longitude->lonMM +
        ((double)longitude->lonMMMMM / 100000.0);

    longitude->lonitudeDegree =
        (double)longitude->lonDDD +
        ((double)longitude->lonMM / 60.0) +
        ((double)longitude->lonMMMMM / 100000.0 / 60.0);

    return HAL_OK;
}
