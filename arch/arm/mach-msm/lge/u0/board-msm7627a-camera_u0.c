/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/i2c.h>
#include <linux/i2c/sx150x.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <asm/mach-types.h>
#include <mach/msm_iomap.h>
#include <mach/board.h>
#include <mach/irqs-7xxx.h>
/*                                                    */
#if defined (CONFIG_MACH_LGE)
#include "../../devices-msm7x2xa.h"
#include "../../board-msm7627a.h"
#else /*qct original*/
#include "devices-msm7x2xa.h"
#include "board-msm7627a.h"
#endif /*               */
/*                                                    */
#include <mach/vreg.h>
#ifdef CONFIG_MACH_LGE
#include CONFIG_LGE_BOARD_HEADER_FILE
#endif
/*                                                       */
#ifdef CONFIG_LEDS_AS364X
#include <linux/leds-as364x.h>
#endif
/*                                                       */

#define GPIO_SKU1_CAM_VGA_SHDN    18
#define GPIO_SKU1_CAM_VGA_RESET_N 29
#define GPIO_SKU3_CAM_5MP_SHDN_N   5         /* PWDN */
#define GPIO_SKU3_CAM_5MP_CAMIF_RESET   6    /* (board_is(EVT))?123:121 RESET */
#define GPIO_SKU3_CAM_5MP_CAM_DRIVER_PWDN 30
#define GPIO_SKU7_CAM_VGA_SHDN    91
#define GPIO_SKU7_CAM_5MP_SHDN_N   93         /* PWDN */
#define GPIO_SKU7_CAM_5MP_CAMIF_RESET   23   /* (board_is(EVT))?123:121 RESET */
/*                                                         */
#ifdef CONFIG_MSM_CAMERA_V4L2
#ifndef CONFIG_MACH_LGE
static uint32_t camera_off_gpio_table[] = {
	GPIO_CFG(15, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};

static uint32_t camera_on_gpio_table[] = {
	GPIO_CFG(15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};

#ifdef CONFIG_S5K4E1
static struct gpio s5k4e1_cam_req_gpio[] = {
	{GPIO_CAM_GP_CAMIF_RESET_N, GPIOF_DIR_OUT, "CAM_RESET"},
};

static struct msm_gpio_set_tbl s5k4e1_cam_gpio_set_tbl[] = {
	{GPIO_CAM_GP_CAMIF_RESET_N, GPIOF_OUT_INIT_LOW, 1000},
	{GPIO_CAM_GP_CAMIF_RESET_N, GPIOF_OUT_INIT_HIGH, 4000},
};

static struct msm_camera_gpio_conf gpio_conf_s5k4e1 = {
	.camera_off_table = camera_off_gpio_table,
	.camera_off_table_size = ARRAY_SIZE(camera_off_gpio_table),
	.camera_on_table = camera_on_gpio_table,
	.camera_on_table_size = ARRAY_SIZE(camera_on_gpio_table),
	.cam_gpio_req_tbl = s5k4e1_cam_req_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(s5k4e1_cam_req_gpio),
	.cam_gpio_set_tbl = s5k4e1_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(s5k4e1_cam_gpio_set_tbl),
	.gpio_no_mux = 1,
};
#endif

#ifdef CONFIG_MT9E013
static struct msm_camera_gpio_conf gpio_conf_mt9e013 = {
	.camera_off_table = camera_off_gpio_table,
	.camera_on_table = camera_on_gpio_table,
	.gpio_no_mux = 1,
};
#endif

#ifdef CONFIG_WEBCAM_OV9726
static struct msm_camera_gpio_conf gpio_conf_ov9726 = {
	.camera_off_table = camera_off_gpio_table,
	.camera_on_table = camera_on_gpio_table,
	.gpio_no_mux = 1,
};
#endif

#ifdef CONFIG_OV7692
static struct gpio ov7692_cam_req_gpio[] = {
	{GPIO_SKU1_CAM_VGA_SHDN, GPIOF_DIR_OUT, "CAM_VGA_SHDN"},
	{GPIO_SKU1_CAM_VGA_RESET_N, GPIOF_DIR_OUT, "CAM_VGA_RESET"},
};

static struct msm_gpio_set_tbl ov7692_cam_gpio_set_tbl[] = {
	{GPIO_SKU1_CAM_VGA_SHDN, GPIOF_OUT_INIT_HIGH, 5000},
	{GPIO_SKU1_CAM_VGA_SHDN, GPIOF_OUT_INIT_LOW, 5000},
	{GPIO_SKU1_CAM_VGA_RESET_N, GPIOF_OUT_INIT_HIGH, 5000},
	{GPIO_SKU1_CAM_VGA_RESET_N, GPIOF_OUT_INIT_LOW, 5000},
};

static struct msm_camera_gpio_conf gpio_conf_ov7692 = {
	.cam_gpio_req_tbl = ov7692_cam_req_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(ov7692_cam_req_gpio),
	.cam_gpio_set_tbl = ov7692_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(ov7692_cam_gpio_set_tbl),
	.gpio_no_mux = 1,
};
#endif

#ifdef CONFIG_OV5647
static struct msm_camera_gpio_conf gpio_conf_ov5647 = {
	.camera_off_table = camera_off_gpio_table,
	.camera_on_table = camera_on_gpio_table,
	.gpio_no_mux = 1,
};
#endif

#if 0 
// TODO  enable this as following settings.
//#ifdef CONFIG_MSM_CAMERA_FLASH
static struct msm_camera_sensor_flash_src msm_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_EXT,
	._fsrc.ext_driver_src.led_en = GPIO_CAM_GP_LED_EN1,
	._fsrc.ext_driver_src.led_flash_en = GPIO_CAM_GP_LED_EN2,
};
#endif
/*                                                                   */
static struct camera_vreg_t msm_cam_vreg[] = {
	{"msme1", REG_LDO, 1800000, 1800000, 0},
	{"ldo10", REG_LDO, 2850000, 2850000, 0},
	{"usb2", REG_LDO, 1800000, 1800000, 0},
};

static struct camera_vreg_t ov5647_gpio_vreg[] = {
	{"cam_ov5647_avdd", REG_GPIO, 0, 0, 0},
	{"cam_ov5647_vdd", REG_GPIO, 0, 0, 0},
};

static struct camera_vreg_t ov8825_gpio_vreg[] = {
	{"cam_ov8825_avdd", REG_GPIO, 0, 0, 0},
	{"cam_ov8825_vdd", REG_GPIO, 0, 0, 0},
};

static struct camera_vreg_t ov7692_gpio_vreg[] = {
	{"cam_ov7692_avdd", REG_GPIO, 0, 0, 0},
	{"cam_ov7692_vdd", REG_GPIO, 0, 0, 0},
};
#endif
/*                                                         */
/*                                                         */
#ifdef CONFIG_HI542
static uint32_t hi542_cam_off_gpio_table[] = {
	GPIO_CFG(15, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), // mclk	
};

static uint32_t hi542_cam_on_gpio_table[] = {
	GPIO_CFG(15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), // mclk
};

static struct gpio hi542_cam_req_gpio[] = {
	{128, GPIOF_DIR_OUT, "CAM_IOVDD"},
	{48, GPIOF_DIR_OUT, "CAM_AVDD"},
	{49, GPIOF_DIR_OUT, "CAM_DVDD"},
	{23, GPIOF_DIR_OUT, "CAM_VCM"},
};

static struct msm_gpio_set_tbl hi542_cam_gpio_set_tbl[] = {
	{128, GPIOF_OUT_INIT_HIGH, 1000}, // iovdd18
	{48, GPIOF_OUT_INIT_HIGH, 1000}, // avdd28
	{49, GPIOF_OUT_INIT_HIGH, 1000}, // dvdd18
	{23, GPIOF_OUT_INIT_HIGH, 1000}, // vcm28
};

static struct msm_camera_gpio_conf gpio_conf_hi542 = {
	.camera_off_table =  hi542_cam_off_gpio_table,
	.camera_off_table_size = ARRAY_SIZE(hi542_cam_off_gpio_table),
	.camera_on_table =  hi542_cam_on_gpio_table,
	.camera_on_table_size = ARRAY_SIZE(hi542_cam_on_gpio_table),
	.cam_gpio_req_tbl = hi542_cam_req_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(hi542_cam_req_gpio),
	.cam_gpio_set_tbl = hi542_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(hi542_cam_gpio_set_tbl),
	.gpio_no_mux = 1,
};
#endif

#ifdef CONFIG_MT9E013_LGIT
static uint32_t mt9e013_lgit_cam_off_gpio_table[] = {
	GPIO_CFG(15/*GPIO_CAM_RESET*/, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};

static uint32_t mt9e013_lgit_cam_on_gpio_table[] = {
	GPIO_CFG(15/*GPIO_CAM_RESET*/, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};

static struct gpio mt9e013_lgit_cam_req_gpio[] = {
	{49, GPIOF_DIR_OUT, "CAM_DVDD"},	
	{128, GPIOF_DIR_OUT, "CAM_IOVDD"},
	{48, GPIOF_DIR_OUT, "CAM_AVDD"},
	{23, GPIOF_DIR_OUT, "CAM_VCM"},
};

static struct msm_gpio_set_tbl mt9e013_lgit_cam_gpio_set_tbl[] = {
	{49, GPIOF_OUT_INIT_HIGH, 1000}, // dvdd18	
	{128, GPIOF_OUT_INIT_HIGH, 1000}, // iovdd18
	{48, GPIOF_OUT_INIT_HIGH, 1000}, // avdd28
	{23, GPIOF_OUT_INIT_HIGH, 1000}, // vcm28
};

static struct msm_camera_gpio_conf gpio_conf_mt9e013_lgit = {
	.camera_off_table =  mt9e013_lgit_cam_off_gpio_table,
	.camera_off_table_size = ARRAY_SIZE(mt9e013_lgit_cam_off_gpio_table),
	.camera_on_table =  mt9e013_lgit_cam_on_gpio_table,
	.camera_on_table_size = ARRAY_SIZE(mt9e013_lgit_cam_on_gpio_table),
	.cam_gpio_req_tbl = mt9e013_lgit_cam_req_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(mt9e013_lgit_cam_req_gpio),
	.cam_gpio_set_tbl = mt9e013_lgit_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(mt9e013_lgit_cam_gpio_set_tbl),
	.gpio_no_mux = 1,
};
#endif

#ifdef CONFIG_MT9V113
static uint32_t mt9v113_cam_off_gpio_table[] = {
	GPIO_CFG(15, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), // mclk	
};

static uint32_t mt9v113_cam_on_gpio_table[] = {
	GPIO_CFG(15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), // mclk
};

static struct gpio mt9v113_cam_req_gpio[] = {
	{128, GPIOF_DIR_OUT, "CAM_IOVDD"},
	{48, GPIOF_DIR_OUT, "CAM_AVDD"},
	{49, GPIOF_DIR_OUT, "CAM_DVDD"},
};

static struct msm_gpio_set_tbl mt9v113_cam_gpio_set_tbl[] = {
	{128, GPIOF_OUT_INIT_HIGH, 2000}, // iovdd18
	{48, GPIOF_OUT_INIT_HIGH, 2000}, // avdd28
	{49, GPIOF_OUT_INIT_HIGH, 1000}, // dvdd18
};

static struct msm_camera_gpio_conf gpio_conf_mt9v113 = {
	.camera_off_table = mt9v113_cam_off_gpio_table,
	.camera_off_table_size = ARRAY_SIZE(mt9v113_cam_off_gpio_table),
	.camera_on_table = mt9v113_cam_on_gpio_table,
	.camera_on_table_size = ARRAY_SIZE(mt9v113_cam_on_gpio_table),
	.cam_gpio_req_tbl = mt9v113_cam_req_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(mt9v113_cam_req_gpio),
	.cam_gpio_set_tbl = mt9v113_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(mt9v113_cam_gpio_set_tbl),
	.gpio_no_mux = 1,
};
#endif
/*                                                         */

struct msm_camera_device_platform_data msm_camera_device_data_csi1[] = {
	{
		.csid_core = 1,
		.ioclk = {
			.vfe_clk_rate = 192000000,
		},
	},
	{
		.csid_core = 1,
		.ioclk = {
			.vfe_clk_rate = 266667000,
		},
	},
};

struct msm_camera_device_platform_data msm_camera_device_data_csi0[] = {
	{
		.csid_core = 0,
		.ioclk = {
			.vfe_clk_rate = 192000000,
		},
	},
	{
		.csid_core = 0,
		.ioclk = {
			.vfe_clk_rate = 266667000,
		},
	},
};

/*                                                         */
#ifdef CONFIG_HI542
static struct i2c_board_info msm_act_main_cam_i2c_info = {
/*                                                               */
#if defined(CONFIG_HI542_ACT) 
	I2C_BOARD_INFO("msm_actuator", 0x18),
#else
	I2C_BOARD_INFO("msm_actuator", 0x11),
#endif
/*                                                               */
};

#ifdef CONFIG_HI542_ACT
static struct msm_actuator_info msm_act_main_cam_3_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_3,
	.bus_id         = MSM_GSBI0_QUP_I2C_BUS_ID,
	.vcm_pwd        = 31, // GPIO_31
	.vcm_enable     = 1,
};
#endif

#ifdef CONFIG_LEDS_AS364X
static struct msm_camera_sensor_flash_src led_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_CURRENT_DRIVER,
};

static struct msm_camera_sensor_flash_data flash_hi542 = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &led_flash_src,
};
#else
static struct msm_camera_sensor_flash_data flash_hi542 = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	//.flash_src              = &msm_flash_src
};
#endif

static struct msm_camera_sensor_platform_info sensor_board_info_hi542 = {
	.mount_angle	= 90,
	//.cam_vreg = msm_cam_vreg,
	//.num_vreg = ARRAY_SIZE(msm_cam_vreg),
	.gpio_conf = &gpio_conf_hi542,
};

static struct msm_camera_sensor_info msm_camera_sensor_hi542_data = {
	.sensor_name    = "hi542",
	.sensor_reset_enable = 1,
	.sensor_reset           = 34, // GPIO_34
	.pdata                  = &msm_camera_device_data_csi1[0],
	.flash_data             = &flash_hi542,
	.sensor_platform_info   = &sensor_board_info_hi542,
	.csi_if                 = 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
#ifdef CONFIG_HI542_ACT
	.actuator_info = &msm_act_main_cam_3_info,
#endif
};
#endif

#ifdef CONFIG_MT9E013_LGIT
#ifdef CONFIG_MT9E013_LGIT_ACT
static struct i2c_board_info msm_act_main_cam_i2c_info = {
		I2C_BOARD_INFO("msm_actuator", 0x11),
};

static struct msm_actuator_info msm_act_main_cam_6_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_6,
	.bus_id         = MSM_GSBI0_QUP_I2C_BUS_ID,
	.vcm_pwd        = 1, 
	.vcm_enable     = 1,
};
#endif//CONFIG_MT9E013_LGIT_ACT

#ifdef CONFIG_LEDS_AS364X
static struct msm_camera_sensor_flash_src led_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_CURRENT_DRIVER,
};

static struct msm_camera_sensor_flash_data flash_mt9e013_lgit = {
	.flash_type = MSM_CAMERA_FLASH_LED,
    .flash_src              = &led_flash_src,
};
#else
static struct msm_camera_sensor_flash_data flash_mt9e013_lgit = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	//.flash_src              = &msm_flash_src
};
#endif//CONFIG_LEDS_AS364X

static struct msm_camera_sensor_platform_info mt9e013_lgit_sensor_info = {
	.mount_angle = 90,
	.gpio_conf = &gpio_conf_mt9e013_lgit,		
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9e013_lgit_data = {
        .sensor_name          = "mt9e013_lgit",
		.sensor_reset_enable = 1,
        .sensor_reset         = 34, // GPIO_34
        .pdata                = &msm_camera_device_data_csi1[0],
        .flash_data           = &flash_mt9e013_lgit,
        .sensor_platform_info  = &mt9e013_lgit_sensor_info,
        .csi_if               = 1,
	    .camera_type = BACK_CAMERA_2D,
	    .sensor_type = BAYER_SENSOR,           
#ifdef CONFIG_MT9E013_LGIT_ACT
	    .actuator_info = &msm_act_main_cam_6_info,
#endif	    
};
#endif

#ifdef CONFIG_MT9V113
static struct msm_camera_sensor_flash_data flash_mt9v113 = {
	.flash_type             = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_platform_info sensor_board_info_mt9v113 = {
	.mount_angle	= 270,
	.gpio_conf = &gpio_conf_mt9v113,
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9v113_data = {
	.sensor_name    = "mt9v113",
	.sensor_reset_enable = 1,
	.sensor_reset           = 7, // GPIO_7
	.sensor_pwd = 8, // GPIO_8	
	.pdata                  = &msm_camera_device_data_csi0[0],
	.flash_data             = &flash_mt9v113,
	.sensor_platform_info   = &sensor_board_info_mt9v113,
	.csi_if                 = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,};
#endif
/*                                                         */
/*                                                         */
#ifndef CONFIG_MACH_LGE
static struct msm_actuator_info msm_act_main_cam_4_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_4,
	.bus_id         = MSM_GSBI0_QUP_I2C_BUS_ID,
	.vcm_pwd        = GPIO_CAM_GP_CAM_PWDN,
	.vcm_enable     = 1,
};

static struct msm_camera_sensor_flash_data flash_s5k4e1 = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	.flash_src              = &msm_flash_src
};

static struct msm_camera_sensor_platform_info sensor_board_info_s5k4e1 = {
/*                                                                                   */
#if defined(CONFIG_MACH_MSM7X27A_U0)
	.mount_angle 	= 270   
#else
	.mount_angle	= 90,
#endif
	.cam_vreg = msm_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_cam_vreg),
	.gpio_conf = &gpio_conf_s5k4e1,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k4e1_data;
static struct msm_camera_sensor_info msm_camera_sensor_s5k4e1_data = {
	.sensor_name    = "s5k4e1",
	.sensor_reset_enable = 1,
	.pdata                  = &msm_camera_device_data_csi1[0],
	.flash_data             = &flash_s5k4e1,
	.sensor_platform_info   = &sensor_board_info_s5k4e1,
	.csi_if                 = 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
	.actuator_info = &msm_act_main_cam_4_info,
};
#endif

#ifdef CONFIG_OV7692
static struct msm_camera_sensor_platform_info sensor_board_info_ov7692 = {
	.mount_angle = 90,
	.cam_vreg = msm_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_cam_vreg),
	.gpio_conf = &gpio_conf_ov7692,
};

static struct msm_camera_sensor_flash_data flash_ov7692 = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov7692_data = {
	.sensor_name	    = "ov7692",
	.sensor_reset_enable    = 0,
	.sensor_reset	   = GPIO_SKU1_CAM_VGA_RESET_N,
	.sensor_pwd	     = GPIO_SKU1_CAM_VGA_SHDN,
	.pdata			= &msm_camera_device_data_csi0[0],
	.flash_data	     = &flash_ov7692,
	.sensor_platform_info   = &sensor_board_info_ov7692,
	.csi_if		 = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif

#ifdef CONFIG_OV5647
static struct msm_actuator_info msm_act_main_cam_5_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_5,
	.bus_id         = MSM_GSBI0_QUP_I2C_BUS_ID,
	.vcm_pwd        = GPIO_SKU3_CAM_5MP_CAM_DRIVER_PWDN,
	.vcm_enable     = 1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_ov5647 = {
	.mount_angle = 90,
	.cam_vreg = msm_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_cam_vreg),
	.gpio_conf = &gpio_conf_ov5647,
};

static struct msm_camera_sensor_flash_src msm_flash_src_ov5647 = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_LED1,
	._fsrc.ext_driver_src.led_en = 13,
	._fsrc.ext_driver_src.led_flash_en = 32,
};

static struct msm_camera_sensor_flash_data flash_ov5647 = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	.flash_src              = &msm_flash_src_ov5647,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov5647_data = {
	.sensor_name    = "ov5647",
	.sensor_reset_enable = 1,
	.sensor_reset   = GPIO_SKU3_CAM_5MP_CAMIF_RESET,
	.sensor_pwd     = GPIO_SKU3_CAM_5MP_SHDN_N,
	.pdata          = &msm_camera_device_data_csi1[0],
	.flash_data     = &flash_ov5647,
	.sensor_platform_info   = &sensor_board_info_ov5647,
	.csi_if                 = 1,
	.camera_type	= BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
	.actuator_info = &msm_act_main_cam_5_info,
};

#endif
#ifndef CONFIG_MACH_LGE
static struct msm_camera_gpio_conf gpio_conf_ov8825 = {
	.camera_off_table = camera_off_gpio_table,
	.camera_on_table = camera_on_gpio_table,
	.gpio_no_mux = 1,
};

static struct msm_camera_sensor_flash_src msm_flash_src_ov8825 = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_LED1,
	._fsrc.ext_driver_src.led_en = 13,
	._fsrc.ext_driver_src.led_flash_en = 32,
};

static struct msm_camera_sensor_flash_data flash_ov8825 = {
	.flash_type     = MSM_CAMERA_FLASH_LED,
	.flash_src      = &msm_flash_src_ov8825,
};

static struct msm_camera_sensor_platform_info sensor_board_info_ov8825 = {
	.mount_angle  = 90,
	.cam_vreg = msm_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_cam_vreg),
	.gpio_conf = &gpio_conf_ov8825,
};

static struct msm_actuator_info msm_act_main_cam_3_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_3,
	.bus_id         = MSM_GSBI0_QUP_I2C_BUS_ID,
	.vcm_pwd        = GPIO_SKU3_CAM_5MP_CAM_DRIVER_PWDN,
	.vcm_enable     = 0,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov8825_data = {
	.sensor_name    = "ov8825",
	.sensor_reset_enable    = 1,
	.pmic_gpio_enable = 1,
	.sensor_reset           = GPIO_SKU3_CAM_5MP_CAMIF_RESET,
	.sensor_pwd     = GPIO_SKU3_CAM_5MP_SHDN_N,
	.pdata  = &msm_camera_device_data_csi1[1],
	.flash_data     = &flash_ov8825,
	.sensor_platform_info = &sensor_board_info_ov8825,
	.csi_if = 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
	.actuator_info = &msm_act_main_cam_3_info,
};

#ifdef CONFIG_MT9E013
static struct msm_camera_sensor_flash_data flash_mt9e013 = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	.flash_src              = &msm_flash_src
};

static struct msm_camera_sensor_platform_info sensor_board_info_mt9e013 = {
	.mount_angle	= 90,
	.cam_vreg = msm_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_cam_vreg),
	.gpio_conf = &gpio_conf_mt9e013,
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9e013_data = {
	.sensor_name    = "mt9e013",
	.sensor_reset_enable = 1,
	.pdata                  = &msm_camera_device_data_csi1[1],
	.flash_data             = &flash_mt9e013,
	.sensor_platform_info   = &sensor_board_info_mt9e013,
	.csi_if                 = 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
};
#endif

#ifdef CONFIG_WEBCAM_OV9726
static struct msm_camera_sensor_flash_data flash_ov9726 = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	.flash_src              = &msm_flash_src
};

static struct msm_camera_sensor_platform_info sensor_board_info_ov9726 = {
	.mount_angle	= 90,
	.cam_vreg = msm_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_cam_vreg),
	.gpio_conf = &gpio_conf_ov9726,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov9726_data = {
	.sensor_name    = "ov9726",
	.sensor_reset_enable = 0,
	.pdata                  = &msm_camera_device_data_csi0[0],
	.flash_data             = &flash_ov9726,
	.sensor_platform_info   = &sensor_board_info_ov9726,
	.csi_if                 = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
};
#endif
#endif
/*                                                         */

static struct platform_device msm_camera_server = {
	.name = "msm_cam_server",
	.id = 0,
};

static void __init msm7x27a_init_cam(void)
{
	if (!(machine_is_msm7x27a_ffa() || machine_is_msm7625a_ffa()
				|| machine_is_msm7627a_qrd1()
				|| machine_is_msm8625_ffa())) {
/*                                                         */
#ifdef CONFIG_MACH_LGE
#ifdef CONFIG_HI542
				sensor_board_info_hi542.cam_vreg = NULL;
				sensor_board_info_hi542.num_vreg = 0;
#endif
#ifdef CONFIG_MT9E013_LGIT
				mt9e013_lgit_sensor_info.cam_vreg = NULL;
				mt9e013_lgit_sensor_info.num_vreg = 0;
#endif
#ifdef CONFIG_MT9V113
		sensor_board_info_mt9v113.cam_vreg = NULL;
		sensor_board_info_mt9v113.num_vreg = 0;
#endif
#else
#ifdef CONFIG_S5K4E1
		sensor_board_info_s5k4e1.cam_vreg = NULL;
		sensor_board_info_s5k4e1.num_vreg = 0;
#endif
#ifdef CONFIG_MT9E013
		sensor_board_info_mt9e013.cam_vreg = NULL;
		sensor_board_info_mt9e013.num_vreg = 0;
#endif
#ifdef CONFIG_WEBCAM_OV9726
		sensor_board_info_ov9726.cam_vreg = NULL;
		sensor_board_info_ov9726.num_vreg = 0;
#endif
#ifdef CONFIG_OV7692
		sensor_board_info_ov7692.cam_vreg = NULL;
		sensor_board_info_ov7692.num_vreg = 0;
#endif
#ifdef CONFIG_OV5647
		sensor_board_info_ov5647.cam_vreg = NULL;
		sensor_board_info_ov5647.num_vreg = 0;
#endif
		sensor_board_info_ov8825.cam_vreg = NULL;
		sensor_board_info_ov8825.num_vreg = 0;
#endif
/*                                                         */
	}
	if (machine_is_msm8625_evb()
			|| machine_is_msm8625_evt()) {
#ifdef CONFIG_OV7692
		sensor_board_info_ov7692.cam_vreg =
			ov7692_gpio_vreg;
		sensor_board_info_ov7692.num_vreg =
			ARRAY_SIZE(ov7692_gpio_vreg);
#endif
#ifdef CONFIG_OV5647
		sensor_board_info_ov5647.cam_vreg =
			ov5647_gpio_vreg;
		sensor_board_info_ov5647.num_vreg =
			ARRAY_SIZE(ov5647_gpio_vreg);
#endif
/*                                                         */
#ifndef CONFIG_MACH_LGE
		sensor_board_info_ov8825.cam_vreg =
			ov8825_gpio_vreg;
		sensor_board_info_ov8825.num_vreg =
			ARRAY_SIZE(ov8825_gpio_vreg);
#endif
/*                                                         */
	}
	platform_device_register(&msm_camera_server);
/*                                                         */
#ifndef CONFIG_MACH_LGE
	if (machine_is_msm8625_surf() || machine_is_msm8625_evb()
			|| machine_is_msm8625_evt()
			|| machine_is_msm8625_qrd7()
			|| machine_is_msm7x27a_u0()) {
		platform_device_register(&msm8625_device_csic0);
		platform_device_register(&msm8625_device_csic1);
	} else {
		platform_device_register(&msm7x27a_device_csic0);
		platform_device_register(&msm7x27a_device_csic1);
	}
#endif
	platform_device_register(&msm7x27a_device_csic0);
	platform_device_register(&msm7x27a_device_csic1);
/*                                                         */
	if (machine_is_msm8625_evb()
			|| machine_is_msm8625_evt()
			|| machine_is_msm8625_qrd7())
		*(int *) msm7x27a_device_clkctl.dev.platform_data = 1;
	platform_device_register(&msm7x27a_device_clkctl);
	platform_device_register(&msm7x27a_device_vfe);
}

static struct i2c_board_info i2c_camera_devices[] = {
/*                                                         */
#ifdef CONFIG_MACH_LGE
#ifdef CONFIG_HI542
	{
		I2C_BOARD_INFO("hi542", 0x40),
		.platform_data = &msm_camera_sensor_hi542_data,
	},
#endif
#ifdef CONFIG_MT9E013_LGIT
	{
		I2C_BOARD_INFO("mt9e013_lgit", 0x6C),
        .platform_data = &msm_camera_sensor_mt9e013_lgit_data,			
	},
#endif
#ifdef CONFIG_MT9V113
	{
		I2C_BOARD_INFO("mt9v113", 0x7A),
		.platform_data = &msm_camera_sensor_mt9v113_data,
	},
#endif
#else
#ifdef CONFIG_S5K4E1
	{
		I2C_BOARD_INFO("s5k4e1", 0x36),
		.platform_data = &msm_camera_sensor_s5k4e1_data,
	},
#endif
#ifdef CONFIG_WEBCAM_OV9726
	{
		I2C_BOARD_INFO("ov9726", 0x10),
		.platform_data = &msm_camera_sensor_ov9726_data,
	},
#endif
#ifdef CONFIG_MT9E013
	{
		I2C_BOARD_INFO("mt9e013", 0x6C >> 2),
		.platform_data = &msm_camera_sensor_mt9e013_data,
	},
#endif
#ifdef CONFIG_WEBCAM_OV7692_QRD
	{
		I2C_BOARD_INFO("ov7692", 0x78),
		.platform_data = &msm_camera_sensor_ov7692_data,
	},
#endif
#ifdef CONFIG_OV5647
	{
		I2C_BOARD_INFO("ov5647", 0x36 << 1),
		.platform_data = &msm_camera_sensor_ov5647_data,
	},
#endif
#ifdef CONFIG_MSM_CAMERA_V4L2
	{
		I2C_BOARD_INFO("ov8825", 0x6C >> 3),
		.platform_data = &msm_camera_sensor_ov8825_data,
	},
#endif
	{
		I2C_BOARD_INFO("sc628a", 0x6E),
	},
#endif
/*                                                         */
};
/*                                                         */
#ifdef CONFIG_LEDS_AS364X
static struct gpio_i2c_pin flash_i2c_pin[] = {
	{
		.sda_pin	= GPIO_FLASH_I2C_SDA,
		.scl_pin	= GPIO_FLASH_I2C_SCL,
		.reset_pin	= 0,
		.irq_pin	= 0,
	},
};
static struct i2c_gpio_platform_data flash_i2c_pdata = {
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.udelay			    = 2,
};

static struct platform_device flash_i2c_device = {
	.name	= "i2c-gpio",
	.dev.platform_data = &flash_i2c_pdata,
};

static struct as364x_platform_data as364x_pdata = {
	.use_tx_mask = 1,
	.I_limit_mA = 2500,
	.txmasked_current_mA = 200,
	.vin_low_v_run_mV = 3300,
	.vin_low_v_mV = 3000,
	.strobe_type = 0, /* 0=edge */
#ifdef CONFIG_AS3648
	.max_peak_current_mA = 1000,
	.max_peak_duration_ms = 500,/* values for LUW FQ6N @1000mA */
#endif
#ifdef CONFIG_AS3647
	.max_peak_current_mA = 1600, /* LUW FQ6N can take up to 2000 */
	.max_peak_duration_ms = 100,/* value for LUW FQ6N @1600mA */
#endif
	.max_sustained_current_mA = 200,
	.min_current_mA = 50,
	.freq_switch_on = 0,
	.led_off_when_vin_low = 1, /* if 0 txmask current is used */
};

static struct i2c_board_info i2c_camera_flash_devices[] = {
	{
		I2C_BOARD_INFO(LEDS_CAMERA_FLASH_NAME, FLASH_I2C_ADDRESS),	
		.flags = I2C_CLIENT_WAKE,
		.irq = 0,
		.platform_data = &as364x_pdata,
	},
};
#endif // CONFIG_LEDS_AS364X
/*                                                         */
#else
static uint32_t camera_off_gpio_table[] = {
	GPIO_CFG(15, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};

static uint32_t camera_on_gpio_table[] = {
	GPIO_CFG(15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};

#ifdef CONFIG_MSM_CAMERA_FLASH
static struct msm_camera_sensor_flash_src msm_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_EXT,
	._fsrc.ext_driver_src.led_en = GPIO_CAM_GP_LED_EN1,
	._fsrc.ext_driver_src.led_flash_en = GPIO_CAM_GP_LED_EN2,
};
#endif

static struct regulator_bulk_data regs_camera[] = {
	{ .supply = "msme1", .min_uV = 1800000, .max_uV = 1800000 },
	{ .supply = "gp2",   .min_uV = 2850000, .max_uV = 2850000 },
	{ .supply = "usb2",  .min_uV = 1800000, .max_uV = 1800000 },
};

static void qrd1_camera_gpio_cfg(void)
{

	int rc = 0;

	rc = gpio_request(QRD_GPIO_CAM_5MP_SHDN_EN, "ov5640");
	if (rc < 0)
		pr_err("%s: gpio_request---GPIO_CAM_5MP_SHDN_EN failed!",
				__func__);


	rc = gpio_tlmm_config(GPIO_CFG(QRD_GPIO_CAM_5MP_SHDN_EN, 0,
				GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc < 0) {
		pr_err("%s: unable to enable Power Down gpio for main"
				"camera!\n", __func__);
		gpio_free(QRD_GPIO_CAM_5MP_SHDN_EN);
	}


	rc = gpio_request(QRD_GPIO_CAM_5MP_RESET, "ov5640");
	if (rc < 0) {
		pr_err("%s: gpio_request---GPIO_CAM_5MP_RESET failed!",
				__func__);
		gpio_free(QRD_GPIO_CAM_5MP_SHDN_EN);
	}


	rc = gpio_tlmm_config(GPIO_CFG(QRD_GPIO_CAM_5MP_RESET, 0,
				GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc < 0) {
		pr_err("%s: unable to enable reset gpio for main camera!\n",
				__func__);
		gpio_free(QRD_GPIO_CAM_5MP_RESET);
	}

	rc = gpio_request(QRD_GPIO_CAM_3MP_PWDN, "ov7692");
	if (rc < 0)
		pr_err("%s: gpio_request---GPIO_CAM_3MP_PWDN failed!",
				__func__);

	rc = gpio_tlmm_config(GPIO_CFG(QRD_GPIO_CAM_3MP_PWDN, 0,
				GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc < 0) {
		pr_err("%s: unable to enable Power Down gpio for front"
				"camera!\n", __func__);
		gpio_free(QRD_GPIO_CAM_3MP_PWDN);
	}

	gpio_direction_output(QRD_GPIO_CAM_5MP_SHDN_EN, 1);
	gpio_direction_output(QRD_GPIO_CAM_5MP_RESET, 1);
	gpio_direction_output(QRD_GPIO_CAM_3MP_PWDN, 1);
}
#endif

#ifdef CONFIG_OV5647
static void evb_camera_gpio_cfg(void)
{
	int rc = 0;

	rc = gpio_request(msm_camera_sensor_ov5647_data.sensor_pwd, "ov5647");
	if (rc < 0)
		pr_err("%s: gpio_request OV5647 sensor_pwd: %d failed!",
			 __func__, msm_camera_sensor_ov5647_data.sensor_pwd);

	rc = gpio_tlmm_config(GPIO_CFG(msm_camera_sensor_ov5647_data.sensor_pwd,
				0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc < 0) {
		pr_err("%s:unable to enable Powr Dwn gpio for main camera!\n",
			 __func__);
		gpio_free(msm_camera_sensor_ov5647_data.sensor_pwd);
	}

	rc = gpio_direction_output(msm_camera_sensor_ov5647_data.sensor_pwd, 1);
	if (rc < 0)
		pr_err("%s: unable to set gpio: %d direction for ov5647 camera\n",
			__func__, msm_camera_sensor_ov5647_data.sensor_pwd);

	rc = gpio_request(msm_camera_sensor_ov5647_data.sensor_reset, "ov5647");
	if (rc < 0)
		pr_err("%s: gpio_request OV5647 sensor_reset: %d failed!",
			 __func__, msm_camera_sensor_ov5647_data.sensor_reset);

	rc = gpio_tlmm_config(GPIO_CFG(
				msm_camera_sensor_ov5647_data.sensor_reset,
				0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc < 0) {
		pr_err("%s: unable to enable reset gpio for main camera!\n",
			 __func__);
		gpio_free(msm_camera_sensor_ov5647_data.sensor_reset);
	}

	rc = gpio_direction_output(
			msm_camera_sensor_ov5647_data.sensor_reset, 1);
	if (rc < 0)
		pr_err("%s: unable to set gpio: %d direction for ov5647 camera\n",
			__func__, msm_camera_sensor_ov5647_data.sensor_reset);

}
#endif

#ifndef CONFIG_MSM_CAMERA_V4L2

static void msm_camera_vreg_config(int vreg_en)
{
	int rc = vreg_en ?
		regulator_bulk_enable(ARRAY_SIZE(regs_camera), regs_camera) :
		regulator_bulk_disable(ARRAY_SIZE(regs_camera), regs_camera);

	if (rc)
		pr_err("%s: could not %sable regulators: %d\n",
				__func__, vreg_en ? "en" : "dis", rc);
}

static int config_gpio_table(uint32_t *table, int len)
{
	int rc = 0, i = 0;

	for (i = 0; i < len; i++) {
		rc = gpio_tlmm_config(table[i], GPIO_CFG_ENABLE);
		if (rc) {
			pr_err("%s not able to get gpio\n", __func__);
			for (i--; i >= 0; i--)
				gpio_tlmm_config(camera_off_gpio_table[i],
							GPIO_CFG_ENABLE);
			break;
		}
	}
	return rc;
}

static int config_camera_on_gpios_rear(void)
{
	int rc = 0;

	if (machine_is_msm7x27a_ffa() || machine_is_msm7625a_ffa()
				|| machine_is_msm7627a_qrd1()
				|| machine_is_msm8625_ffa())
		msm_camera_vreg_config(1);

	rc = config_gpio_table(camera_on_gpio_table,
			ARRAY_SIZE(camera_on_gpio_table));
	if (rc < 0) {
		pr_err("%s: CAMSENSOR gpio table request"
		"failed\n", __func__);
		return rc;
	}

	return rc;
}

static void config_camera_off_gpios_rear(void)
{
	if (machine_is_msm7x27a_ffa() || machine_is_msm7625a_ffa()
				|| machine_is_msm7627a_qrd1()
				|| machine_is_msm8625_ffa())
		msm_camera_vreg_config(0);

	config_gpio_table(camera_off_gpio_table,
			ARRAY_SIZE(camera_off_gpio_table));
}

static int config_camera_on_gpios_front(void)
{
	int rc = 0;

	if (machine_is_msm7x27a_ffa() || machine_is_msm7625a_ffa()
				|| machine_is_msm7627a_qrd1()
				|| machine_is_msm8625_ffa())
		msm_camera_vreg_config(1);

	rc = config_gpio_table(camera_on_gpio_table,
			ARRAY_SIZE(camera_on_gpio_table));
	if (rc < 0) {
		pr_err("%s: CAMSENSOR gpio table request"
			"failed\n", __func__);
		return rc;
	}

	return rc;
}

static void config_camera_off_gpios_front(void)
{
	if (machine_is_msm7x27a_ffa() || machine_is_msm7625a_ffa()
				|| machine_is_msm7627a_qrd1()
				|| machine_is_msm8625_ffa())
		msm_camera_vreg_config(0);

	config_gpio_table(camera_off_gpio_table,
			ARRAY_SIZE(camera_off_gpio_table));
}

struct msm_camera_device_platform_data msm_camera_device_data_rear = {
	.camera_gpio_on		= config_camera_on_gpios_rear,
	.camera_gpio_off	= config_camera_off_gpios_rear,
	.ioext.csiphy		= 0xA1000000,
	.ioext.csisz		= 0x00100000,
	.ioext.csiirq		= INT_CSI_IRQ_1,
	.ioclk.mclk_clk_rate	= 24000000,
	.ioclk.vfe_clk_rate	= 192000000,
	.ioext.appphy		= MSM7XXX_CLK_CTL_PHYS,
	.ioext.appsz		= MSM7XXX_CLK_CTL_SIZE,
};

struct msm_camera_device_platform_data msm_camera_device_data_front = {
	.camera_gpio_on		= config_camera_on_gpios_front,
	.camera_gpio_off	= config_camera_off_gpios_front,
	.ioext.csiphy		= 0xA0F00000,
	.ioext.csisz		= 0x00100000,
	.ioext.csiirq		= INT_CSI_IRQ_0,
	.ioclk.mclk_clk_rate	= 24000000,
	.ioclk.vfe_clk_rate	= 192000000,
	.ioext.appphy		= MSM7XXX_CLK_CTL_PHYS,
	.ioext.appsz		= MSM7XXX_CLK_CTL_SIZE,
};

#ifdef CONFIG_OV5647

static struct msm_camera_sensor_platform_info ov5647_sensor_7627a_info = {
	.mount_angle = 90
};

static struct msm_camera_sensor_flash_src msm_flash_src_ov5647 = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_LED,
	._fsrc.led_src.led_name = "flashlight",
	._fsrc.led_src.led_name_len = 10,
};

static struct msm_camera_sensor_flash_data flash_ov5647 = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	.flash_src              = &msm_flash_src_ov5647,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov5647_data = {
	.sensor_name    = "ov5647",
	.sensor_reset_enable = 1,
	.sensor_reset   = GPIO_SKU3_CAM_5MP_CAMIF_RESET,
	.pmic_gpio_enable  = 1,
	.sensor_pwd     = GPIO_SKU3_CAM_5MP_SHDN_N,
	.vcm_pwd        = GPIO_SKU3_CAM_5MP_CAM_DRIVER_PWDN,
	.vcm_enable     = 1,
	.pdata          = &msm_camera_device_data_rear,
	.flash_data     = &flash_ov5647,
	.sensor_platform_info   = &ov5647_sensor_7627a_info,
	.csi_if                 = 1
};

static struct platform_device msm_camera_sensor_ov5647 = {
	.name      = "msm_camera_ov5647",
	.dev       = {
		.platform_data = &msm_camera_sensor_ov5647_data,
	},
};
#endif

#ifdef CONFIG_S5K4E1
static struct msm_camera_sensor_platform_info s5k4e1_sensor_7627a_info = {
	.mount_angle = 90
};

static struct msm_camera_sensor_flash_data flash_s5k4e1 = {
	.flash_type	     = MSM_CAMERA_FLASH_LED,
	.flash_src	      = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k4e1_data = {
	.sensor_name		= "s5k4e1",
	.sensor_reset_enable	= 1,
	.sensor_reset		= GPIO_CAM_GP_CAMIF_RESET_N,
	.pmic_gpio_enable       = 0,
	.sensor_pwd	     = 85,
	.vcm_pwd		= GPIO_CAM_GP_CAM_PWDN,
	.vcm_enable	     = 1,
	.pdata			= &msm_camera_device_data_rear,
	.flash_data	     = &flash_s5k4e1,
	.sensor_platform_info   = &s5k4e1_sensor_7627a_info,
	.csi_if			= 1
};

static struct platform_device msm_camera_sensor_s5k4e1 = {
	.name   = "msm_camera_s5k4e1",
	.dev    = {
		.platform_data = &msm_camera_sensor_s5k4e1_data,
	},
};
#endif

#ifdef CONFIG_IMX072
static struct msm_camera_sensor_platform_info imx072_sensor_7627a_info = {
	.mount_angle = 90
};

static struct msm_camera_sensor_flash_data flash_imx072 = {
	.flash_type	     = MSM_CAMERA_FLASH_LED,
	.flash_src	      = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_imx072_data = {
	.sensor_name		= "imx072",
	.sensor_reset_enable	= 1,
	.sensor_reset		= GPIO_CAM_GP_CAMIF_RESET_N, /* TODO 106,*/
	.pmic_gpio_enable       = 0,
	.sensor_pwd	     = 85,
	.vcm_pwd		= GPIO_CAM_GP_CAM_PWDN,
	.vcm_enable	     = 1,
	.pdata			= &msm_camera_device_data_rear,
	.flash_data	     = &flash_imx072,
	.sensor_platform_info	= &imx072_sensor_7627a_info,
	.csi_if			= 1
};

static struct platform_device msm_camera_sensor_imx072 = {
	.name   = "msm_camera_imx072",
	.dev    = {
		.platform_data = &msm_camera_sensor_imx072_data,
	},
};
#endif

#ifdef CONFIG_WEBCAM_OV9726
static struct msm_camera_sensor_info msm_camera_sensor_ov9726_data;
static struct msm_camera_sensor_platform_info ov9726_sensor_7627a_info = {
	.mount_angle = 90
};

static struct msm_camera_sensor_flash_data flash_ov9726 = {
	.flash_type	     = MSM_CAMERA_FLASH_NONE,
	.flash_src	      = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_ov9726_data = {
	.sensor_name		= "ov9726",
	.sensor_reset_enable	= 0,
	.sensor_reset		= GPIO_CAM_GP_CAM1MP_XCLR,
	.pmic_gpio_enable       = 0,
	.sensor_pwd	     = 85,
	.vcm_pwd		= 1,
	.vcm_enable	     = 0,
	.pdata			= &msm_camera_device_data_front,
	.flash_data	     = &flash_ov9726,
	.sensor_platform_info   = &ov9726_sensor_7627a_info,
	.csi_if			= 1
};

static struct platform_device msm_camera_sensor_ov9726 = {
	.name   = "msm_camera_ov9726",
	.dev    = {
		.platform_data = &msm_camera_sensor_ov9726_data,
	},
};
#else
static inline void msm_camera_vreg_init(void) { }
#endif

#ifdef CONFIG_MT9E013
static struct msm_camera_sensor_platform_info mt9e013_sensor_7627a_info = {
	.mount_angle = 90
};

static struct msm_camera_sensor_flash_data flash_mt9e013 = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9e013_data = {
	.sensor_name		= "mt9e013",
	.sensor_reset		= 0,
	.sensor_reset_enable	= 1,
	.pmic_gpio_enable       = 0,
	.sensor_pwd		= 85,
	.vcm_pwd		= 1,
	.vcm_enable		= 0,
	.pdata		= &msm_camera_device_data_rear,
	.flash_data		= &flash_mt9e013,
	.sensor_platform_info   = &mt9e013_sensor_7627a_info,
	.csi_if		= 1
};

static struct platform_device msm_camera_sensor_mt9e013 = {
	.name      = "msm_camera_mt9e013",
	.dev       = {
		.platform_data = &msm_camera_sensor_mt9e013_data,
	},
};
#endif

#ifdef CONFIG_OV5640
static struct msm_camera_sensor_platform_info ov5640_sensor_info = {
	.mount_angle    = 90
};

static struct msm_camera_sensor_flash_src msm_flash_src_ov5640 = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_LED,
	._fsrc.led_src.led_name = "flashlight",
	._fsrc.led_src.led_name_len = 10,
};

static struct msm_camera_sensor_flash_data flash_ov5640 = {
	.flash_type     = MSM_CAMERA_FLASH_LED,
	.flash_src      = &msm_flash_src_ov5640,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov5640_data = {
	.sensor_name	    = "ov5640",
	.sensor_reset_enable    = 1,
	.pmic_gpio_enable  = 0,
	.sensor_reset	   = QRD_GPIO_CAM_5MP_RESET,
	.sensor_pwd	     = QRD_GPIO_CAM_5MP_SHDN_EN,
	.vcm_pwd		= 0,
	.vcm_enable	     = 0,
	.pdata			= &msm_camera_device_data_rear,
	.flash_data	     = &flash_ov5640,
	.sensor_platform_info   = &ov5640_sensor_info,
	.csi_if		 = 1,
};

static struct platform_device msm_camera_sensor_ov5640 = {
	.name   = "msm_camera_ov5640",
	.dev    = {
		.platform_data = &msm_camera_sensor_ov5640_data,
	},
};
#endif

#ifdef CONFIG_WEBCAM_OV7692_QRD
static struct msm_camera_sensor_platform_info ov7692_sensor_7627a_info = {
	.mount_angle = 90
};

static struct msm_camera_sensor_flash_data flash_ov7692 = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov7692_data = {
	.sensor_name	    = "ov7692",
	.sensor_reset_enable    = 0,
	.pmic_gpio_enable  = 1,
	.sensor_reset	   = GPIO_SKU1_CAM_VGA_RESET_N,
	.sensor_pwd	     = GPIO_SKU1_CAM_VGA_SHDN,
	.vcm_pwd		= 0,
	.vcm_enable	     = 0,
	.pdata			= &msm_camera_device_data_front,
	.flash_data	     = &flash_ov7692,
	.sensor_platform_info   = &ov7692_sensor_7627a_info,
	.csi_if		 = 1,
};

static struct platform_device msm_camera_sensor_ov7692 = {
	.name   = "msm_camera_ov7692",
	.dev    = {
		.platform_data = &msm_camera_sensor_ov7692_data,
	},
};
#endif

static struct i2c_board_info i2c_camera_devices[] = {
	#ifdef CONFIG_S5K4E1
	{
		I2C_BOARD_INFO("s5k4e1", 0x36),
	},
	{
		I2C_BOARD_INFO("s5k4e1_af", 0x8c >> 1),
	},
	#endif
	#ifdef CONFIG_WEBCAM_OV9726
	{
		I2C_BOARD_INFO("ov9726", 0x10),
	},
	#endif
	#ifdef CONFIG_IMX072
	{
		I2C_BOARD_INFO("imx072", 0x34),
	},
	#endif
	#ifdef CONFIG_MT9E013
	{
		I2C_BOARD_INFO("mt9e013", 0x6C >> 2),
	},
	#endif
	{
		I2C_BOARD_INFO("sc628a", 0x6E),
	},
};

static struct i2c_board_info i2c_camera_devices_qrd[] = {
	#ifdef CONFIG_OV5640
	{
		I2C_BOARD_INFO("ov5640", 0x78 >> 1),
	},
	#endif
	#ifdef CONFIG_WEBCAM_OV7692_QRD
	{
		I2C_BOARD_INFO("ov7692", 0x78),
	},
	#endif
};

static struct i2c_board_info i2c_camera_devices_evb[] = {
	#ifdef CONFIG_OV5647
	{
		I2C_BOARD_INFO("ov5647", 0x36 << 1),
	},
	{
		I2C_BOARD_INFO("ov5647_af", 0x18 >> 1),
	},
	#endif
	#ifdef CONFIG_WEBCAM_OV7692_QRD
	{
		I2C_BOARD_INFO("ov7692", 0x78),
	},
	#endif
};

static struct platform_device *camera_devices_msm[] __initdata = {
#ifdef CONFIG_S5K4E1
	&msm_camera_sensor_s5k4e1,
#endif
#ifdef CONFIG_IMX072
	&msm_camera_sensor_imx072,
#endif
#ifdef CONFIG_WEBCAM_OV9726
	&msm_camera_sensor_ov9726,
#endif
#ifdef CONFIG_MT9E013
	&msm_camera_sensor_mt9e013,
#endif
};

static struct platform_device *camera_devices_qrd[] __initdata = {
#ifdef CONFIG_OV5640
	&msm_camera_sensor_ov5640_data,
#endif
#ifdef CONFIG_WEBCAM_OV7692_QRD
	&msm_camera_sensor_ov7692_data,
#endif
};

static struct platform_device *camera_devices_evb[] __initdata = {
#ifdef CONFIG_OV5647
	&msm_camera_sensor_ov5647_data,
#endif
#ifdef CONFIG_WEBCAM_OV7692_QRD
	&msm_camera_sensor_ov7692_data,
#endif
#ifdef CONFIG_MSM_CAMERA_V4L2
	&msm_camera_sensor_ov8825_data,
#endif
};
#endif
/*                                                         */
#ifndef CONFIG_MACH_LGE
enum {
	SX150X_CAM,
};

static struct sx150x_platform_data sx150x_data[] __initdata = {
	[SX150X_CAM]    = {
		.gpio_base	      = GPIO_CAM_EXPANDER_BASE,
		.oscio_is_gpo	   = false,
		.io_pullup_ena	  = 0,
		.io_pulldn_ena	  = 0,
		.io_open_drain_ena      = 0x23,
		.irq_summary	    = -1,
	},
};

static struct i2c_board_info cam_exp_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO("sx1508q", 0x22),
		.platform_data  = &sx150x_data[SX150X_CAM],
	},
};

static void __init register_i2c_devices(void)
{
	i2c_register_board_info(MSM_GSBI0_QUP_I2C_BUS_ID,
				cam_exp_i2c_info,
				ARRAY_SIZE(cam_exp_i2c_info));
}
#endif
/*                                                         */

#ifndef CONFIG_MSM_CAMERA_V4L2
#define LCD_CAMERA_LDO_2V8 35 /* SKU1&SKU3 2.8V LDO */
#define SKU3_LCD_CAMERA_LDO_1V8 40 /* SKU3 1.8V LDO */
#define SKU7_LCD_CAMERA_LDO_1V8 58 /* SKU7 1.8V LDO */

static int lcd_camera_ldo_1v8 = SKU3_LCD_CAMERA_LDO_1V8;

static void lcd_camera_power_init(void)
{
	int rc = 0;

	pr_debug("lcd_camera_power_init\n");

	if (machine_is_msm7627a_qrd3() || machine_is_msm8625_qrd7())
		lcd_camera_ldo_1v8 = SKU7_LCD_CAMERA_LDO_1V8;
	else
		lcd_camera_ldo_1v8 = SKU3_LCD_CAMERA_LDO_1V8;

	/* LDO_EXT2V8 */
	if (gpio_request(LCD_CAMERA_LDO_2V8, "lcd_camera_ldo_2v8")) {
		pr_err("failed to request gpio lcd_camera_ldo_2v8\n");
		return;
	}

	rc = gpio_tlmm_config(GPIO_CFG(LCD_CAMERA_LDO_2V8, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN,
		GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc < 0) {
		pr_err("%s: unable to enable lcd_camera_ldo_2v8!\n", __func__);
		goto fail_gpio2;
	}

	/* LDO_EVT1V8 */
	if (gpio_request(lcd_camera_ldo_1v8, "lcd_camera_ldo_1v8")) {
		pr_err("failed to request gpio lcd_camera_ldo_1v8\n");
		goto fail_gpio2;
	}

	rc = gpio_tlmm_config(GPIO_CFG(lcd_camera_ldo_1v8, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN,
		GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc < 0) {
		pr_err("%s: unable to enable lcd_camera_ldo_1v8!\n", __func__);
		goto fail_gpio1;
	}

	return;

fail_gpio1:
	gpio_free(lcd_camera_ldo_1v8);
fail_gpio2:
	gpio_free(LCD_CAMERA_LDO_2V8);

	return;
}

static int lcd_camera_power_on_sku3(void)
{
	int rc = 0;

	pr_debug("turn on sku3 lcd_camera_ldo_1v8\n");
	gpio_set_value_cansleep(lcd_camera_ldo_1v8, 1);

	pr_debug("turn on sku3 lcd_camera_ldo\n");
	gpio_set_value_cansleep(LCD_CAMERA_LDO_2V8, 1);

	return rc;
}

static int lcd_camera_power_off_sku3(void)
{
	int rc = 0;

	pr_debug("turn off sku3 lcd_camera_ldo_1v8\n");
	gpio_set_value_cansleep(lcd_camera_ldo_1v8, 0);

	pr_debug("turn off sku3 lcd_camera_ldo\n");
	gpio_set_value_cansleep(LCD_CAMERA_LDO_2V8, 0);

	gpio_free(lcd_camera_ldo_1v8);
	gpio_free(LCD_CAMERA_LDO_2V8);

	return rc;
}

int lcd_camera_power_onoff(int on)
{
	int rc = 0;

	pr_debug("lcd_camera_power_onoff on = %d,\n", on);

	if (on)
		rc = lcd_camera_power_on_sku3();
	else
		rc = lcd_camera_power_off_sku3();

	return rc;
}
EXPORT_SYMBOL(lcd_camera_power_onoff);
#endif

/*                                                       */
#ifdef CONFIG_LEDS_AS364X
static void __init lge_init_i2c_camera(int bus_num)
{
	flash_i2c_device.id = bus_num;

	lge_init_gpio_i2c_pin(&flash_i2c_pdata, flash_i2c_pin[0],
		&i2c_camera_flash_devices[0]);
	i2c_register_board_info(bus_num, i2c_camera_flash_devices,
		ARRAY_SIZE(i2c_camera_flash_devices));
	platform_device_register(&flash_i2c_device);
}
#endif
/*                                                       */
void __init msm7627a_camera_init(void)
{

#ifndef CONFIG_MSM_CAMERA_V4L2
	int rc;
#endif

	pr_debug("msm7627a_camera_init Entered\n");

	if (machine_is_msm7627a_qrd3() || machine_is_msm8625_qrd7()) {
#ifdef CONFIG_OV7692
		ov7692_cam_req_gpio[0].gpio =
			GPIO_SKU7_CAM_VGA_SHDN;
		ov7692_cam_gpio_set_tbl[0].gpio = GPIO_SKU7_CAM_VGA_SHDN;
		ov7692_cam_gpio_set_tbl[1].gpio = GPIO_SKU7_CAM_VGA_SHDN;

		msm_camera_sensor_ov5647_data.sensor_pwd =
			GPIO_SKU7_CAM_5MP_SHDN_N;
		msm_camera_sensor_ov5647_data.sensor_reset =
			GPIO_SKU7_CAM_5MP_CAMIF_RESET;
#endif
	}

	/* LCD and camera power (VREG & LDO) init */
	if (machine_is_msm7627a_evb() || machine_is_msm8625_evb()
			|| machine_is_msm8625_evt()
			|| machine_is_msm7627a_qrd3()
			|| machine_is_msm8625_qrd7()) {
#ifndef CONFIG_MSM_CAMERA_V4L2
		lcd_camera_power_init();
#endif
#ifdef CONFIG_OV5647
		evb_camera_gpio_cfg();
#endif
	}

#ifndef CONFIG_MSM_CAMERA_V4L2
	if (machine_is_msm7627a_qrd1()) {
		qrd1_camera_gpio_cfg();
		platform_add_devices(camera_devices_qrd,
				ARRAY_SIZE(camera_devices_qrd));
	} else if (machine_is_msm7627a_evb()
			|| machine_is_msm8625_evb()
			|| machine_is_msm8625_evt()
			|| machine_is_msm7627a_qrd3()
			|| machine_is_msm8625_qrd7()) {
		platform_add_devices(camera_devices_evb,
				ARRAY_SIZE(camera_devices_evb));
	} else if (machine_is_msm7627a_qrd3())
		return;
	else
		platform_add_devices(camera_devices_msm,
				ARRAY_SIZE(camera_devices_msm));
#endif
/*                                                         */
#ifndef CONFIG_MACH_LGE
	if (!machine_is_msm7627a_qrd1() || !machine_is_msm7627a_evb()
					|| !machine_is_msm8625_evb()
					|| !machine_is_msm8625_evt()
					|| !machine_is_msm7627a_qrd3()
					|| !machine_is_msm8625_qrd7())
		register_i2c_devices();
#endif
/*                                                         */
#ifndef CONFIG_MSM_CAMERA_V4L2
	rc = regulator_bulk_get(NULL, ARRAY_SIZE(regs_camera), regs_camera);

	if (rc) {
		pr_err("%s: could not get regulators: %d\n", __func__, rc);
		return;
	}

	rc = regulator_bulk_set_voltage(ARRAY_SIZE(regs_camera), regs_camera);

	if (rc) {
		pr_err("%s: could not set voltages: %d\n", __func__, rc);
		return;
	}
#endif

#if defined(CONFIG_MSM_CAMERA_V4L2)
	msm7x27a_init_cam();
#endif
#ifndef CONFIG_MSM_CAMERA_V4L2
	if (machine_is_msm7627a_qrd1()) {
		i2c_register_board_info(MSM_GSBI0_QUP_I2C_BUS_ID,
				i2c_camera_devices_qrd,
				ARRAY_SIZE(i2c_camera_devices_qrd));
	} else if (machine_is_msm7627a_evb()
			|| machine_is_msm8625_evb()
			|| machine_is_msm8625_evt()
			|| machine_is_msm7627a_qrd3()
			|| machine_is_msm8625_qrd7()) {
		pr_debug("machine_is_msm7627a_evb i2c_register_board_info\n");
		i2c_register_board_info(MSM_GSBI0_QUP_I2C_BUS_ID,
				i2c_camera_devices_evb,
				ARRAY_SIZE(i2c_camera_devices_evb));
	} else
#endif
		pr_debug("i2c_register_board_info\n");
		i2c_register_board_info(MSM_GSBI0_QUP_I2C_BUS_ID,
				i2c_camera_devices,
				ARRAY_SIZE(i2c_camera_devices));
/*                                                         */
#ifdef CONFIG_LEDS_AS364X
	lge_add_gpio_i2c_device(lge_init_i2c_camera);
#endif
/*                                                         */
}
