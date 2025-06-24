#include "systemConfig.h"
#include "thirdParty/thirdPartyInclude.h"
#include "thirdParty/AlgorithmLib/crc16.h"
systemCfg_t g_system_cfg = {0};

#define CRC_CBVALUE 	0XFEFE
void global_cfg_reset(void)
{

	strcpy(g_system_cfg.device_name, "123123");


  	strcpy(g_system_cfg.update_url, "teeechina.xicp.net");
  	g_system_cfg.update_port = 13103;

  	strcpy(g_system_cfg.server_url, "teeechina.xicp.net");
  	g_system_cfg.server_port = 13101; 

	g_system_cfg.mqtt_connect_timeout = 5000;
	g_system_cfg.mqtt_keep_alive = 30;

	strcpy(g_system_cfg.mqtt_key, "DefaultProductKey");
  	strcpy(g_system_cfg.mqtt_secret, "0TQ9rsH17Ij2f9Pc4Ft25U1LH3j320r0");
}

void global_cfg_read(void)
{
	systemCfg_t cfg_data = {0};
	uint8_t *cfg_ptr = NULL;
	uint16_t pre_crc16 = 0, cur_crc16 = 0;
	struct fal_partition falpart, *part1 = NULL;
	part1 = &falpart;
	cfg_ptr = (uint8_t *)&cfg_data;
	part1 = (struct fal_partition *)fal_partition_find(PARTITION_NAME_ONCHIP_RFCFG);
	if(part1 == NULL)
	{
		printf("fal partition error\r\n");
		return;
	}
	fal_partition_read(part1,0,cfg_ptr,sizeof(systemCfg_t));
	Delay_ms(50);
	pre_crc16 = cfg_data.crc;
	cfg_data.crc = CRC_CBVALUE;
	cur_crc16 = DL_CRC16(&cfg_data, sizeof(systemCfg_t));
	printf("pre_crc16 = 0x%04x  cur_crc16 = 0x%04x\r\n",pre_crc16,cur_crc16);
	if(pre_crc16 != cur_crc16)
	{	
		printf("crc not match, g_system_cfg reset\r\n");
		global_cfg_reset();
	}
	else
	{
		printf("read g_system_cfg successfully\r\n");
		memcpy(&g_system_cfg, &cfg_data, sizeof(systemCfg_t));
	}
}

void global_cfg_write(void)
{
	uint16_t crc16 = 0;
	struct fal_partition falpart, *p;
	p = &falpart;
	g_system_cfg.crc = CRC_CBVALUE;
	crc16 = DL_CRC16(&g_system_cfg, sizeof(systemCfg_t));
	g_system_cfg.crc = crc16;
	p = (struct fal_partition *)fal_partition_find(PARTITION_NAME_ONCHIP_RFCFG);
	if(p == NULL)
	{
		printf("fal partition error\r\n");
		return;
	}
	fal_partition_erase(p, 0, sizeof(systemCfg_t));
	Delay_ms(20);
	fal_partition_write(p, 0, (uint8_t *)&g_system_cfg, sizeof(systemCfg_t));
	Delay_ms(20);
}



