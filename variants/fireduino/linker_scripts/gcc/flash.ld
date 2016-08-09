OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

BOOT_ROM_BASE   	=	0x00000000 ;
 
HRAM_CODE_BASE 		=	0x01000000 ;
HRAM_CODE_SIZE   	=	0x00020000 ;

HRAM_DATA_BASE   	=   0x01020000 ;
HRAM_DATA_SIZE   	=   0x00040000 ;

LOADER_DATA_BASE 	=   0x0304A000 ;
LOADER_CODE_BASE 	=   0x03088000 ;

SYS_DATA_BASE   	=	0x03050000 ;
SYS_DATA_SIZE    	=   (0x00040000) ;
SYS_DATA_LIMIT   	=   (0x03090000) ;

SYS_CODE_BASE     	=  	0x03000000 ;
SYS_CODE_SIZE     	=  	(0x00050000) ;
SYS_CODE_LIMIT   	=	(0x03050000) ;

DEV_CODE_START   	=   0x03070000 ;

PMU_DATA_BASE    	=   0x00000000 ;
PMU_DATA_SIZE     	=  	0x00001000 ;
PMU_CODE_BASE    	=   0x00001000 ;
PMU_CODE_SIZE   	=	0x0000F000 ;
SHELL_CODE_START 	=   0x0000C000 ;

PMU_DATA_LIMIT   	=	(0x00010000) ;

DLNA_CODE_BASE		=	0x03044B00 ;

/* Section Definitions */
SECTIONS 
{
	/DISCARD/ :
	 { 
		 *(.ARM.attributes) 
		 *(.comment) 
	 }
	 
	.moduleinfo (SYS_DATA_BASE - 0x1f50) : AT (BOOT_ROM_BASE)
	{
		KEEP(*(_ModuleInfo_ _ModuleInfo_.*))	
		. = ALIGN(4);		
	}
	_module_load_addr = LOADADDR(.moduleinfo);
	
	_shell_cmd_name_data_image_addr = ADDR(.moduleinfo);	
	_shell_cmd_name_data_load_addr  = LOADADDR(.moduleinfo) + SIZEOF ( .moduleinfo );	
	.shell_cmd_name_data _shell_cmd_name_data_image_addr : AT (_shell_cmd_name_data_load_addr)
	{
		KEEP(*(ShellRegisterName ShellRegisterName.*))	
	}
	_shell_cmd_name_data_len_ = SIZEOF ( .shell_cmd_name_data );

	_task_info_data_image_addr = ADDR(.moduleinfo);	
	_task_info_data_load_addr  = LOADADDR(.shell_cmd_name_data) + SIZEOF ( .shell_cmd_name_data );	
	.task_info_data _task_info_data_image_addr : AT (_task_info_data_load_addr)
	{
		KEEP(*(InitTaskItems InitTaskItems.*))	
	}
	_task_info_data_len_ = SIZEOF ( .task_info_data );

	_dmr_data_load_addr = LOADADDR(.task_info_data) + SIZEOF (.task_info_data);		
	OVERLAY SHELL_CODE_START : NOCROSSREFS  AT (_dmr_data_load_addr)
	{
		.dmr_device_data
		{
		 	_dmr_device_data_vma_ = ADDR(.dmr_device_data);
			KEEP(*(__CG_UPNPAV_DMR_DEVICE_DESCRIPTION __CG_UPNPAV_DMR_DEVICE_DESCRIPTION.*))
			. = ALIGN(4);	
		}
		.dmr_connectionmanager_service_data
		{
		 	_dmr_connectionmanager_service_data_vma_ = ADDR(.dmr_connectionmanager_service_data);
			KEEP(*(__CG_UPNPAV_DMR_CONNECTIONMANAGER_SERVICE_DESCRIPTION __CG_UPNPAV_DMR_CONNECTIONMANAGER_SERVICE_DESCRIPTION.*))
			. = ALIGN(4);		
		}
		.dmr_renderingcontrol_service_data
		{
		 	_dmr_renderingcontrol_service_data_vma_ = ADDR(.dmr_renderingcontrol_service_data);
			KEEP(*(__CG_UPNPAV_DMR_RENDERINGCONTROL_SERVICE_DESCRIPTION __CG_UPNPAV_DMR_RENDERINGCONTROL_SERVICE_DESCRIPTION.*))
			. = ALIGN(4);
		}

		.dmr_namingbitmap_data
		{
		 	_dmr_namingbitmap_data_vma_ = ADDR(.dmr_namingbitmap_data);
			KEEP(*(__NAMINGBITMAP __NAMINGBITMAP.*))
			. = ALIGN(4);		
		}
		.dmr_nmstrtpages_data
		{
		 	_dmr_nmstrtpages_data_vma_ = ADDR(.dmr_nmstrtpages_data);
			KEEP(*(__NMSTRTPAGES __NMSTRTPAGES.*))
			. = ALIGN(4);		
		}
		.dmr_namepages_data
		{
		 	_dmr_namepages_data_vma_ = ADDR(.dmr_namepages_data);
			KEEP(*(__NAMEPAGES __NAMEPAGES.*))
			. = ALIGN(4);		
		}
		.dmr_aviransport_service_data
		{
		 	_dmr_aviransport_service_data_vma_ = ADDR(.dmr_aviransport_service_data);
			KEEP(*(__CG_UPNPAV_DMR_AVTRANSPORT_SERVICE_DESCRIPTION __CG_UPNPAV_DMR_AVTRANSPORT_SERVICE_DESCRIPTION.*))
			. = ALIGN(4);	
		}	
	}	
	
	_dmr_device_data_len_							= SIZEOF (.dmr_device_data);
	_dmr_connectionmanager_service_data_len_		= SIZEOF (.dmr_connectionmanager_service_data);
	_dmr_renderingcontrol_service_data_len_			= SIZEOF (.dmr_renderingcontrol_service_data);
	_dmr_aviransport_service_data_len_				= SIZEOF (.dmr_aviransport_service_data);
	_dmr_namingbitmap_data_len_						= SIZEOF (.dmr_namingbitmap_data);
	_dmr_nmstrtpages_data_len_						= SIZEOF (.dmr_nmstrtpages_data);
	_dmr_namepages_data_len_						= SIZEOF (.dmr_namepages_data);

	_shell_code_load_addr = __load_stop_dmr_aviransport_service_data ;
	OVERLAY SYS_CODE_BASE : NOCROSSREFS  AT (_shell_code_load_addr)
	{
		.custom_shell 
		{
			 _custom_shell_vma_ = ADDR(.custom_shell);
			 *\ShellCustomCmd.o	
			 . = ALIGN(4);
		}
		.dev_shell 
		{
			 _dev_shell_vma_ = ADDR(.dev_shell);
			 *\ShellDevCmd.o
			 . = ALIGN(4);		
		}
		.task_shell 
		{
			 _task_shell_vma_ = ADDR(.task_shell);
			 *\ShellTaskCmd.o
			 . = ALIGN(4);
		}
		.system_shell
		{
			 _system_shell_vma_ = ADDR(.system_shell);
			*\ShellSysCmd.o	
			. = ALIGN(4);	
		}
		.record_shell
		{
			 _record_shell_vma_ = ADDR(.record_shell);
	        *(app_record_record_shell)
	        *(app_record_record_shell_data)
	        *\record.o
			. = ALIGN(4);				
		}
		.linein_shell
		{
			 _linein_shell_vma_ = ADDR(.linein_shell);
	        *(app_linein_linein_shell)
	        *(app_linein_linein_shell_data)
	        *\linein.o
			. = ALIGN(4);			
		}		
		.music_shell
		{
			 _music_shell_vma_ = ADDR(.music_shell);
			 *(app_audio_music_shell)
			 *(app_audio_music_shell_data)
			. = ALIGN(4);			
		}
		.wifi_shell
		{
			 _wifi_shell_vma_ = ADDR(.wifi_shell);		
			 *(ap6181wifiShell)
			 *(ap6181wifiShell_data)
			 . = ALIGN(4);		
		}	
		.dlna_shell
		{
			 _dlna_shell_vma_ = ADDR(.dlna_shell);	
			 *(DlnaShell)
			 *(DlnaShell_data)
			 . = ALIGN(4);		
		}	
		.usbotg_shell
		{
			_usbotg_shell_vma_ = ADDR(.usbotg_shell);
			*(driver_usb_usbotgdev_shell)
			*(driver_usb_usbotgdev_shell_data)
			. = ALIGN(4);
		}
		.spi_shell
		{
		 	_spi_shell_vma_ = ADDR(.spi_shell);
			*(driver_spi_spidevice_shell)
			*(driver_spi_spidevice_shell_data)
			. = ALIGN(4);		
		}
		.spiflash_shell
		{
		 	_spiflash_shell_vma_ = ADDR(.spiflash_shell);
			*(driver_spiflash_spiflashdev_shell)
			*(driver_spiflash_spiflashdev_shell_data)
			. = ALIGN(4);		
		}
		.sdc_shell
		{
		 	_sdc_shell_vma_ = ADDR(.sdc_shell);
			*(driver_sdmmc_sdmmcdevice_shell)
			*(driver_sdmmc_sdmmcdevice_shell_data)
			. = ALIGN(4);		
		}
		.sd_shell
		{
		 	_sd_shell_vma_ = ADDR(.sd_shell);
			*(driver_sd_sddevice_shell)
			*(driver_sd_sddevice_shell_data)
			. = ALIGN(4);		
		}
		.sdio_shell
		{
		 	_sdio_shell_vma_ = ADDR(.sdio_shell);
			*(driver_sdio_sdiodevice_shell)
			*(driver_sdio_sdiodevice_shell_data)
			. = ALIGN(4);		
		}
		.emmc_shell
		{
		 	_emmc_shell_vma_ = ADDR(.emmc_shell);
			*(driver_emmc_emmcdevice_shell)
			*(driver_emmc_emmcdevice_shell_data)
			. = ALIGN(4);		
		}
		.lun_shell
		{
			 _lun_shell_vma_ = ADDR(.lun_shell);
			*(driver_lun_lundevice_shell)
			*(driver_lun_lundevice_shell_data)
			. = ALIGN(4);
		}
		.par_shell
		{
			_par_shell_vma_ = ADDR(.par_shell);
			*(driver_partion_partiondevice_shell)
			*(driver_partion_partiondevice_shell_data)	
			. = ALIGN(4);
		}
		.fat_shell
		{
			_fat_shell_vma_ = ADDR(.fat_shell);
			*(driver_volume_fatdevice_shell)
			*(driver_volume_fatdevice_shell_data)	
			. = ALIGN(4);		
		}
		.file_shell
		{
			_file_shell_vma_ = ADDR(.file_shell);
			*(driver_file_filedevice_shell)
			*(driver_file_filedevice_shell_data)
			. = ALIGN(4);		
		}
		.dir_shell
		{
			_dir_shell_vma_ = ADDR(.dir_shell);
			*(driver_file_dirdevice_shell)
			*(driver_file_dirdevice_shell_data)			
			. = ALIGN(4);
		}
		.fifo_shell
		{
			_fifo_shell_vma_ = ADDR(.fifo_shell);
			*(driver_fifo_fifodevice_shell)
			*(driver_fifo_fifodevice_data)
			*(driver_fifo_fifodevice_shell_data)				
			. = ALIGN(4);
		}		
		.msg_shell
		{
			_msg_shell_vma_ = ADDR(.msg_shell);
			*(driver_msg_msgdevice_shell)
			*(driver_msg_msgdevice_shell_data)
			. = ALIGN(4);
		}
		.bcore_shell
		{
			_bcore_shell_vma_ = ADDR(.bcore_shell);
			*(driver_bcore_bcoredevice_shell)
			*(driver_bcore_bcoredevice_shell_data)
			. = ALIGN(4);
		}
		.audio_shell
		{
			_audio_shell_vma_ = ADDR(.audio_shell);		
			*(driver_audio_audiodevice_shell)
			*(driver_audio_audiodevice_data)
			. = ALIGN(4);			
		}
		.rockcodec_shell
		{
			_rockcodec_shell_vma_ = ADDR(.rockcodec_shell);		
			*(driver_rockcodec_rockcodecdevice_shell)
			*(driver_rockcodec_rockcodecdevice_shell_data)
			. = ALIGN(4);	
		}
		.i2s_shell
		{
			_i2s_shell_vma_ = ADDR(.i2s_shell);	
			*(driver_i2s_i2sdevice_shell)
			*(driver_i2s_i2sdevice_shell_data)
			. = ALIGN(4);	
		}
		.i2c_shell
		{
			_i2c_shell_vma_ = ADDR(.i2c_shell);	
			*(driver_i2c_i2cdevice_shell)
			*(driver_i2c_i2cdevice_data)
			. = ALIGN(4);				
		}
		.dma_shell
		{
			_dma_shell_vma_ = ADDR(.dma_shell);
			*(driver_dma_dmadevice_shell)
			*(driver_dma_dmadevice_data)
			. = ALIGN(4);
		}
		.adc_shell
		{
			_adc_shell_vma_ = ADDR(.adc_shell);
			*(driver_ad_adcdevice_shell)
			*(driver_ad_adcdevice_data)
			*(driver_ad_adcdevice_hlp_data)
			. = ALIGN(4);	
		}
		.gpio_shell
		{
			_gpio_shell_vma_ = ADDR(.gpio_shell);
			*(cpu_nanod_lib_gpio_shell)
			*(cpu_nanod_lib_gpio_shell_data)
			. = ALIGN(4);
		}		
		.uart_shell
		{
			 _uart_shell_vma_ = ADDR(.uart_shell);
			 *(driver_uart_uartdevie_shell)
			 *(driver_uart_uartdevice_shell_data)
			 . = ALIGN(4);
		}

	}


	_custom_shell_len_		= SIZEOF (.custom_shell);	
	_dev_shell_len_ 		= SIZEOF (.dev_shell);	
	_task_shell_len_ 		= SIZEOF (.task_shell);	
	_system_shell_len_ 		= SIZEOF (.system_shell);	
	_music_shell_len_ 		= SIZEOF (.music_shell);	
	_wifi_shell_len_ 		= SIZEOF (.wifi_shell);	
	_dlna_shell_len_ 		= SIZEOF (.dlna_shell);	
	_usbotg_shell_len_     	= SIZEOF (.usbotg_shell);
	_dma_shell_len_     	= SIZEOF (.dma_shell);
	_spi_shell_len_ 		= SIZEOF (.spi_shell);
	_spiflash_shell_len_ 	= SIZEOF (.spiflash_shell);
	_sdc_shell_len_ 		= SIZEOF (.sdc_shell);
	_sd_shell_len_ 			= SIZEOF (.sd_shell);
	_sdio_shell_len_ 		= SIZEOF (.sdio_shell);
	_emmc_shell_len_ 		= SIZEOF (.emmc_shell);
	_lun_shell_len_ 		= SIZEOF (.lun_shell);
	_par_shell_len_ 		= SIZEOF (.par_shell);
	_fat_shell_len_ 		= SIZEOF (.fat_shell);
	_file_shell_len_ 		= SIZEOF (.file_shell);
	_dir_shell_len_ 		= SIZEOF (.dir_shell);
	_fifo_shell_len_ 		= SIZEOF (.fifo_shell);
	_msg_shell_len_ 		= SIZEOF (.msg_shell);
	_bcore_shell_len_ 		= SIZEOF (.bcore_shell);
	_audio_shell_len_		= SIZEOF (.audio_shell);
	_rockcodec_shell_len_	= SIZEOF (.rockcodec_shell);
	_i2s_shell_len_			= SIZEOF (.i2s_shell);
	_i2c_shell_len_			= SIZEOF (.i2c_shell);
	_adc_shell_len_ 		= SIZEOF (.adc_shell);
	_gpio_shell_len_ 		= SIZEOF (.gpio_shell);
	_uart_shell_len_ 		= SIZEOF (.uart_shell);	
	
	_linein_shell_len_ 		= SIZEOF (.linein_shell);	
	_record_shell_len_ 		= SIZEOF (.record_shell);	


	_shell_code_max_len 	= _custom_shell_len_;
	_shell_code_max_len1 	= MAX(_shell_code_max_len,_dev_shell_len_);
	_shell_code_max_len2 	= MAX(_shell_code_max_len1,_task_shell_len_);
	_shell_code_max_len3 	= MAX(_shell_code_max_len2,_system_shell_len_);	
	_shell_code_max_len4 	= MAX(_shell_code_max_len3,_music_shell_len_);
	_shell_code_max_len5 	= MAX(_shell_code_max_len4,_wifi_shell_len_);
	_shell_code_max_len6 	= MAX(_shell_code_max_len5,_dlna_shell_len_);
	_shell_code_max_len7 	= MAX(_shell_code_max_len6,_usbotg_shell_len_);
	_shell_code_max_len8 	= MAX(_shell_code_max_len7,_dma_shell_len_);
	_shell_code_max_len9 	= MAX(_shell_code_max_len8,_spi_shell_len_);
	_shell_code_max_len10 	= MAX(_shell_code_max_len9,_spiflash_shell_len_);
	_shell_code_max_len11 	= MAX(_shell_code_max_len10,_sdc_shell_len_);
	_shell_code_max_len12 	= MAX(_shell_code_max_len11,_sd_shell_len_);
	_shell_code_max_len13 	= MAX(_shell_code_max_len12,_sdio_shell_len_);
	_shell_code_max_len14 	= MAX(_shell_code_max_len13,_emmc_shell_len_);
	_shell_code_max_len15 	= MAX(_shell_code_max_len14,_lun_shell_len_);
	_shell_code_max_len16 	= MAX(_shell_code_max_len15,_par_shell_len_);
	_shell_code_max_len17 	= MAX(_shell_code_max_len16,_fat_shell_len_);
	_shell_code_max_len18 	= MAX(_shell_code_max_len17,_file_shell_len_);
	_shell_code_max_len19 	= MAX(_shell_code_max_len18,_dir_shell_len_);
	_shell_code_max_len20 	= MAX(_shell_code_max_len19,_fifo_shell_len_);
	_shell_code_max_len21 	= MAX(_shell_code_max_len20,_msg_shell_len_);
	_shell_code_max_len22 	= MAX(_shell_code_max_len21,_bcore_shell_len_);
	_shell_code_max_len23 	= MAX(_shell_code_max_len22,_audio_shell_len_);
	_shell_code_max_len24 	= MAX(_shell_code_max_len23,_rockcodec_shell_len_);
	_shell_code_max_len25 	= MAX(_shell_code_max_len24,_i2s_shell_len_);
	_shell_code_max_len26 	= MAX(_shell_code_max_len25,_i2c_shell_len_);
	_shell_code_max_len27 	= MAX(_shell_code_max_len26,_adc_shell_len_);
	_shell_code_max_len28 	= MAX(_shell_code_max_len27,_gpio_shell_len_);
	_shell_code_max_len29 	= MAX(_shell_code_max_len28,_uart_shell_len_);
	
	_shell_code_max_len30 	= MAX(_shell_code_max_len29,_linein_shell_len_);
	_shell_code_max_len31 	= MAX(_shell_code_max_len30,_record_shell_len_);



/*	__init_code_load_addr = LOADADDR(.uart_shell) + SIZEOF (.uart_shell);	*/
 	__init_code_load_addr = __load_stop_uart_shell == SYS_CODE_BASE ? _shell_code_load_addr : __load_stop_uart_shell  ;	 
	__init_code_image_addr = ADDR(.uart_shell) +  _shell_code_max_len31;
	OVERLAY __init_code_image_addr : NOCROSSREFS  AT (__init_code_load_addr)
	{	

		.audio_init
		{
			_audio_init_vma_ = ADDR(.audio_init);
			*(driver_audio_audiodevice_init)
			. = ALIGN(4);			
		}
		.rockcodec_init
		{
			_rockcodec_init_vma_ = ADDR(.rockcodec_init);
			*(driver_rockcodec_rockcodecdevice_init)
			. = ALIGN(4);				
		}
		.dlna_task_init
		{
			_dlna_task_init_vma_ = ADDR(.dlna_task_init);
			*(dlna_task_init)
			. = ALIGN(4);				
		}
		.i2s_init
		{
			_i2s_init_vma_ = ADDR(.i2s_init);
			*(driver_i2s_i2sdevice_init)
			. = ALIGN(4);			
		}
		.i2c_init
		{
			_i2c_init_vma_ = ADDR(.i2c_init);
			*(driver_i2c_i2cdevice_init)
			. = ALIGN(4);			
		}
		.stream_control_init
		{
			_stream_control_init_vma_ = ADDR(.stream_control_init);
			 *(app_filestream_streamcontroltask_init)
			. = ALIGN(4);			
		}
		.audio_control_init
		{
			_audio_control_init_vma_ = ADDR(.audio_control_init);
			 *(app_audio_audiocontroltask_init)
			. = ALIGN(4);		
		}
		.record_control_init
		{
			_record_control_init_vma_ = ADDR(.record_control_init);
			 *(app_record_recordcontroltask_init)
			. = ALIGN(4);		
		}
		.linein_control_init
		{
			_linein_control_init_vma_ = ADDR(.linein_control_init);
			 *(app_line_in_lineincontroltask_init)
			. = ALIGN(4);		
		}	
		.linein_init
		{
			_linein_init_vma_ = ADDR(.linein_init);
			 *(app_line_in_lineintask_init)
			. = ALIGN(4);		
		}		
		.msg_init
		{
			_msg_init_vma_ = ADDR(.msg_init);
			 *(driver_msg_msgdevice_init)
			. = ALIGN(4);
		}
		.bcore_init
		{
			_bcore_init_vma_ = ADDR(.bcore_init);
			 *(driver_bcore_bcoredevice_init)
			. = ALIGN(4);
		}
		.fifo_init
		{
			_fifo_init_vma_ = ADDR(.fifo_init);
			 *(driver_fifo_fifodevice_init)
			. = ALIGN(4);
		}
		.dir_init
		{
			_dir_init_vma_ = ADDR(.dir_init);
			 *(driver_file_dirdevice_init)
			. = ALIGN(4);
		}
		.file_init
		{
			_file_init_vma_ = ADDR(.file_init);
			*(driver_file_filedevice_init)
			. = ALIGN(4);
		}
		.fat_init
		{
			_fat_init_vma_ = ADDR(.fat_init);
			 *(driver_volume_fatdevice_init)
			. = ALIGN(4);
		}	
		.par_init
		{
			_par_init_vma_ = ADDR(.par_init);
			*(driver_partion_partiondevice_init)
			. = ALIGN(4);
		}
		.lun_init
		{
			_lun_init_vma_ = ADDR(.lun_init);
			. = ALIGN(4);
		}
		.emmc_init
		{
			_emmc_init_vma_ = ADDR(.emmc_init);
			*(driver_emmc_emmcdevice_init)
			. = ALIGN(4);
		}
		.sdio_init
		{
			_sdio_init_vma_ = ADDR(.sdio_init);
			*(driver_sdio_sdiodevice_init)
			. = ALIGN(4);
		}
		.sd_init
		{
			_sd_init_vma_ = ADDR(.sd_init);
			*(driver_sd_sddevice_init)
			. = ALIGN(4);
		}	
		.sdc_init
		{
			_sdc_init_vma_ = ADDR(.sdc_init);
			*(driver_sdmmc_sdmmcdevice_init)
			. = ALIGN(4);
		}		
		.spiflash_init
		{
			_spiflash_init_vma_ = ADDR(.spiflash_init);
			. = ALIGN(4);
		}
		.spi_init
		{
			_spi_init_vma_ = ADDR(.spi_init);
			. = ALIGN(4);
		}		
		.adc_init
		{
			_adc_init_vma_ = ADDR(.adc_init);
			*(driver_ad_adcdevice_init)
			. = ALIGN(4);
		}
		.usbotg_init
		{
			_usbotg_init_vma_ = ADDR(.usbotg_init);
			*(driver_usb_usbotgdev_init) 
			. = ALIGN(4);
		}
		.uart_init
		{
		 	_uart_init_vma_ = ADDR(.uart_init);
			*(driver_uart_uartdevie_init)
			. = ALIGN(4);
		}

	} 
	
	_dlna_task_init_len_		= SIZEOF (.dlna_task_init);
	_stream_control_init_len_	= SIZEOF (.stream_control_init);
	_audio_control_init_len_	= SIZEOF (.audio_control_init);
	_audio_init_len_			= SIZEOF (.audio_init);
	_rockcodec_init_len_		= SIZEOF (.rockcodec_init);
	_i2s_init_len_				= SIZEOF (.i2s_init);
	_i2c_init_len_				= SIZEOF (.i2c_init);
	_bcore_init_len_			= SIZEOF (.bcore_init);
	_msg_init_len_				= SIZEOF (.msg_init);
	_fifo_init_len_				= SIZEOF (.fifo_init);
	_dir_init_len_				= SIZEOF (.dir_init);
	_file_init_len_				= SIZEOF (.file_init);
	_fat_init_len_				= SIZEOF (.fat_init);
	_par_init_len_				= SIZEOF (.par_init);
	_adc_init_len_				= SIZEOF (.adc_init);
	_lun_init_len_				= SIZEOF (.lun_init);
	_emmc_init_len_				= SIZEOF (.emmc_init);
	_sdio_init_len_				= SIZEOF (.sdio_init);
	_sd_init_len_				= SIZEOF (.sd_init);
	_sdc_init_len_				= SIZEOF (.sdc_init);
	_spiflash_init_len_			= SIZEOF (.spiflash_init);	
	_spi_init_len_				= SIZEOF (.spi_init);
	_usbotg_init_len_ 			= SIZEOF (.usbotg_init);
	_uart_init_len_				= SIZEOF (.uart_init);
	
	_linein_init_len_			= SIZEOF (.linein_init);
	_linein_control_init_len_	= SIZEOF (.linein_control_init);
	_record_control_init_len_	= SIZEOF (.record_control_init);
	
			

	_init_code_max_len  	= _dlna_task_init_len_;
	_init_code_max_len1 	= MAX(_init_code_max_len, _stream_control_init_len_);
	_init_code_max_len2 	= MAX(_init_code_max_len1, _audio_control_init_len_);
	_init_code_max_len3 	= MAX(_init_code_max_len2, _audio_init_len_);
	_init_code_max_len4 	= MAX(_init_code_max_len3, _rockcodec_init_len_);
	_init_code_max_len5 	= MAX(_init_code_max_len4, _i2s_init_len_);
	_init_code_max_len6 	= MAX(_init_code_max_len5, _i2c_init_len_);
	_init_code_max_len7 	= MAX(_init_code_max_len6, _bcore_init_len_);
	_init_code_max_len8 	= MAX(_init_code_max_len7, _msg_init_len_);
	_init_code_max_len9 	= MAX(_init_code_max_len8, _fifo_init_len_);
	_init_code_max_len10 	= MAX(_init_code_max_len9, _dir_init_len_);
	_init_code_max_len11 	= MAX(_init_code_max_len10, _file_init_len_);
	_init_code_max_len12 	= MAX(_init_code_max_len11, _fat_init_len_);
	_init_code_max_len13 	= MAX(_init_code_max_len12, _par_init_len_);
	_init_code_max_len14 	= MAX(_init_code_max_len13, _adc_init_len_);
	_init_code_max_len15 	= MAX(_init_code_max_len14, _lun_init_len_);
	_init_code_max_len16 	= MAX(_init_code_max_len15, _emmc_init_len_);
	_init_code_max_len17 	= MAX(_init_code_max_len16, _sdio_init_len_);
	_init_code_max_len18 	= MAX(_init_code_max_len17, _sd_init_len_);
	_init_code_max_len19 	= MAX(_init_code_max_len18, _sdc_init_len_);
	_init_code_max_len20 	= MAX(_init_code_max_len19, _spiflash_init_len_);
	_init_code_max_len21 	= MAX(_init_code_max_len20, _spi_init_len_);
	_init_code_max_len22 	= MAX(_init_code_max_len21, _usbotg_init_len_);
	_init_code_max_len23 	= MAX(_init_code_max_len22, _uart_init_len_);
	
	_init_code_max_len24 	= MAX(_init_code_max_len23, _linein_init_len_);
	_init_code_max_len25 	= MAX(_init_code_max_len24, _linein_control_init_len_);
	_init_code_max_len26 	= MAX(_init_code_max_len25, _record_control_init_len_);

	_init_code_image_end_addr	=  ADDR(.uart_init) +  _init_code_max_len26;


	_init_code_load_addr  = LOADADDR(.uart_init) + SIZEOF (.uart_init);	
	_init_code_image_addr = ADDR(.uart_init) + SIZEOF (.uart_init);	
    .init_code _init_code_image_addr : AT (_init_code_load_addr)
    {
        *(bsp_evk_v20_bsp_init)
        *(os_plugin_deviceplugin_init)
        *(os_plugin_taskplugin_init)
        *(os_fwanalysis_fwanalysis_init)
        *(driver_dma_dmadevice_init)
        *(driver_spiflash_spiflashdev_init)
        *(system_syssever_powermanager_init)
        *(system_moduleoverlay_sysinfosave_init)
        *(system_shell_shelltask_init)
        *(os_devicemanager_devicemanagertask_init)
        *(SysInitCode)
        *(SysInitData)
        *(SysInitBss)
        . = ALIGN(4);
    }
      
   	_init_code_len =  SIZEOF (.init_code);		

/************************************************/
    /* DEV_CODE_START */
    _i2c_dev_code_image_addr 		= ADDR(.init_code) + SIZEOF ( .init_code );
    _i2c_dev_code_load_addr			= LOADADDR(.init_code) + SIZEOF ( .init_code );	
	.i2c_dev_code _i2c_dev_code_image_addr : AT (_i2c_dev_code_load_addr)
	{
        *(driver_i2c_i2cdevice_read)
        *(driver_i2c_i2cdevice_write)
        *\I2cDevice.o 
        *\i2c.o
        . = ALIGN(4);         
	}
	_i2c_dev_code_len =  SIZEOF (.i2c_dev_code);		

    _i2s_dev_code_image_addr 		= ADDR(.i2c_dev_code) + SIZEOF ( .i2c_dev_code );
    _i2s_dev_code_load_addr		= LOADADDR(.i2c_dev_code) + SIZEOF ( .i2c_dev_code );	
	.i2s_dev_code _i2s_dev_code_image_addr : AT (_i2s_dev_code_load_addr)
	{
        *(driver_i2s_i2sdevice_common)
        *\I2sDevice.o 
        *\I2s.o 
        . = ALIGN(4);
	}
	_i2s_dev_code_len =  SIZEOF (.i2s_dev_code);	

    _rockcodec_dev_code_image_addr 		= ADDR(.i2s_dev_code) + SIZEOF ( .i2s_dev_code );
    _rockcodec_dev_code_load_addr		= LOADADDR(.i2s_dev_code) + SIZEOF ( .i2s_dev_code );	
	.rockcodec_dev_code _rockcodec_dev_code_image_addr : AT (_rockcodec_dev_code_load_addr)
	{
        *(driver_rockcodec_rockcodecdevice_common)
        *\RockCodecDevice.o 
        *\rockcodec.o 
        . = ALIGN(4);
	}
	_rockcodec_dev_code_len =  SIZEOF (.rockcodec_dev_code);	

    _audio_dev_code_image_addr 		= ADDR(.rockcodec_dev_code) + SIZEOF ( .rockcodec_dev_code );
    _audio_dev_code_load_addr		= LOADADDR(.rockcodec_dev_code) + SIZEOF ( .rockcodec_dev_code );	
	.audio_dev_code _audio_dev_code_image_addr : AT (_audio_dev_code_load_addr)
	{
        *(driver_audio_audiodevice_common)
        *\AudioDevice.o
		*\spectrum.o
        *\fft_64.o
        . = ALIGN(4);
	}
	_audio_dev_code_len =  SIZEOF (.audio_dev_code);
	
	_bcore_dev_code_image_addr 	= ADDR(.audio_dev_code) + SIZEOF ( .audio_dev_code );
    _bcore_dev_code_load_addr	= LOADADDR(.audio_dev_code) + SIZEOF ( .audio_dev_code );	
	.bcore_dev_code _bcore_dev_code_image_addr : AT (_bcore_dev_code_load_addr)
	{
        *(driver_bcore_bcoredevice_common)
        *\BcoreDevice.o
        *(cpu_nanod_lib_mailbox_common)
        . = ALIGN(4);
	}
	_bcore_dev_code_len =  SIZEOF (.bcore_dev_code);	
	
	_msg_dev_code_image_addr 	= ADDR(.bcore_dev_code) + SIZEOF ( .bcore_dev_code );
    _msg_dev_code_load_addr	= LOADADDR(.bcore_dev_code) + SIZEOF ( .bcore_dev_code );	
	.msg_dev_code _msg_dev_code_image_addr : AT (_msg_dev_code_load_addr)
	{
        *(driver_msg_msgdevice_common)
        *\MsgDevice.o
        . = ALIGN(4);
	}
	_msg_dev_code_len =  SIZEOF (.msg_dev_code);	
	
	_fifo_dev_code_image_addr 	= ADDR(.msg_dev_code) + SIZEOF ( .msg_dev_code );
    _fifo_dev_code_load_addr	= LOADADDR(.msg_dev_code) + SIZEOF ( .msg_dev_code );	
	.fifo_dev_code _fifo_dev_code_image_addr : AT (_fifo_dev_code_load_addr)
	{
        *(driver_fifo_fifodevice_common)
        *\FIFODevice.o
        . = ALIGN(4);
	}
	_fifo_dev_code_len =  SIZEOF (.fifo_dev_code);	 	
	
	_dir_dev_code_image_addr 	= ADDR(.fifo_dev_code) + SIZEOF ( .fifo_dev_code );
    _dir_dev_code_load_addr		= LOADADDR(.fifo_dev_code) + SIZEOF ( .fifo_dev_code );	
	.dir_dev_code _dir_dev_code_image_addr : AT (_dir_dev_code_load_addr)
	{
        *(driver_file_dirdevice_common)
        *\DirDevice.o
        . = ALIGN(4);
	}
	_dir_dev_code_len =  SIZEOF (.dir_dev_code);	  
	
    _file_dev_code_image_addr 	= ADDR(.dir_dev_code) + SIZEOF ( .dir_dev_code );
    _file_dev_code_load_addr	= LOADADDR(.dir_dev_code) + SIZEOF ( .dir_dev_code );	
	.file_dev_code _file_dev_code_image_addr : AT (_file_dev_code_load_addr)
	{
        *(driver_file_filedevice_common)
        *\FileDevice.o
        . = ALIGN(4);
	}
	_file_dev_code_len =  SIZEOF (.file_dev_code);	    
   
    _fat_dev_code_image_addr 	= ADDR(.file_dev_code) + SIZEOF ( .file_dev_code );
    _fat_dev_code_load_addr		= LOADADDR(.file_dev_code) + SIZEOF ( .file_dev_code );	
	.fat_dev_code _fat_dev_code_image_addr : AT (_fat_dev_code_load_addr)
	{
        *(driver_volume_fatdevice_common)
        *(driver_volume_fatdevice_common_data)
        *\FATDevice.o
        . = ALIGN(4);
	}
	_fat_dev_code_len =  SIZEOF (.fat_dev_code);	 
	    
    _par_dev_code_image_addr 	= ADDR(.fat_dev_code) + SIZEOF ( .fat_dev_code );
    _par_dev_code_load_addr		= LOADADDR(.fat_dev_code) + SIZEOF ( .fat_dev_code );	
	.par_dev_code _par_dev_code_image_addr : AT (_par_dev_code_load_addr)
	{
        *(driver_partion_partiondevice_common)
        *\PartionDevice.o
        . = ALIGN(4);
	}
	_par_dev_code_len =  SIZEOF (.par_dev_code);	    
  
	_lun_dev_code_image_addr 		= ADDR(.par_dev_code) + SIZEOF ( .par_dev_code );
    _lun_dev_code_load_addr		= LOADADDR(.par_dev_code) + SIZEOF ( .par_dev_code );	
	.lun_dev_code _lun_dev_code_image_addr : AT (_lun_dev_code_load_addr)
	{
        . = ALIGN(4);
	}
	_lun_dev_code_len =  SIZEOF (.lun_dev_code);

	_emmc_dev_code_image_addr 		= ADDR(.lun_dev_code) + SIZEOF ( .lun_dev_code );
    _emmc_dev_code_load_addr		= LOADADDR(.lun_dev_code) + SIZEOF ( .lun_dev_code );	
	.emmc_dev_code _emmc_dev_code_image_addr : AT (_emmc_dev_code_load_addr)
	{
         *(driver_emmc_emmcdevice_common)
         *\EmmcDevice.o
        . = ALIGN(4);
	}
	_emmc_dev_code_len =  SIZEOF (.emmc_dev_code);	

	_sdio_dev_code_image_addr 		= ADDR(.emmc_dev_code) + SIZEOF ( .emmc_dev_code );
    _sdio_dev_code_load_addr		= LOADADDR(.emmc_dev_code) + SIZEOF ( .emmc_dev_code );	
	.sdio_dev_code _sdio_dev_code_image_addr : AT (_sdio_dev_code_load_addr)
	{
        *(driver_sdio_sdiodevice_common)
        *\SdioDevice.o
        *\wwd_SDIO.o
        . = ALIGN(4);
	}
	_sdio_dev_code_len =  SIZEOF (.sdio_dev_code);	
	
	_sd_dev_code_image_addr 		= ADDR(.sdio_dev_code) + SIZEOF ( .sdio_dev_code );
    _sd_dev_code_load_addr			= LOADADDR(.sdio_dev_code) + SIZEOF ( .sdio_dev_code );	
	.sd_dev_code _sd_dev_code_image_addr : AT (_sd_dev_code_load_addr)
	{
		*(driver_sd_sddevice_common)
        *\SdDevice.o
        . = ALIGN(4);
	}
	_sd_dev_code_len =  SIZEOF (.sd_dev_code);	

	_sdc_dev_code_image_addr 		= ADDR(.sd_dev_code) + SIZEOF ( .sd_dev_code );
    _sdc_dev_code_load_addr			= LOADADDR(.sd_dev_code) + SIZEOF ( .sd_dev_code );	
	.sdc_dev_code _sdc_dev_code_image_addr : AT (_sdc_dev_code_load_addr)
	{
		*(driver_sdmmc_sdmmcdevice_common)
        *\SdMmcDevice.o
        *\sdmmc.o
        . = ALIGN(4);
	}
	_sdc_dev_code_len =  SIZEOF (.sdc_dev_code);	
	
	_spiflash_dev_code_image_addr 		= ADDR(.sdc_dev_code) + SIZEOF ( .sdc_dev_code );
    _spiflash_dev_code_load_addr		= LOADADDR(.sdc_dev_code) + SIZEOF ( .sdc_dev_code );	
	.spiflash_dev_code _spiflash_dev_code_image_addr : AT (_spiflash_dev_code_load_addr)
	{
        . = ALIGN(4);
	}
	_spiflash_dev_code_len =  SIZEOF (.spiflash_dev_code);	
	
	_spi_dev_code_image_addr 		= ADDR(.spiflash_dev_code) + SIZEOF ( .spiflash_dev_code );
    _spi_dev_code_load_addr		= LOADADDR(.spiflash_dev_code) + SIZEOF ( .spiflash_dev_code );	
	.spi_dev_code _spi_dev_code_image_addr : AT (_spi_dev_code_load_addr)
	{
        . = ALIGN(4);
	}
	_spi_dev_code_len =  SIZEOF (.spi_dev_code);		
	     
    _adc_dev_code_image_addr 	= ADDR(.spi_dev_code) + SIZEOF ( .spi_dev_code );
    _adc_dev_code_load_addr		= LOADADDR(.spi_dev_code) + SIZEOF ( .spi_dev_code );	
	.adc_dev_code _adc_dev_code_image_addr : AT (_adc_dev_code_load_addr)
	{
		*(driver_ad_adcdevice_common)
        *\adcdevice.o
        *\adc.o
        . = ALIGN(4);
	}
	_adc_dev_code_len =  SIZEOF (.adc_dev_code);	
	
    _usbotg_dev_code_image_addr 	= ADDR(.adc_dev_code) + SIZEOF ( .adc_dev_code );
    _usbotg_dev_code_load_addr		= LOADADDR(.adc_dev_code) + SIZEOF ( .adc_dev_code );	
	.usbotg_dev_code _usbotg_dev_code_image_addr : AT (_usbotg_dev_code_load_addr)
	{
        *(driver_usb_usbotgdev_common)
        *\UsbOtgDev.o 
        *\usb.o 
        . = ALIGN(4);
	}
	_usbotg_dev_code_len =  SIZEOF (.usbotg_dev_code);
	
	_uart_dev_code_image_addr 		= ADDR(.usbotg_dev_code) + SIZEOF ( .usbotg_dev_code );
    _uart_dev_code_load_addr		= LOADADDR(.usbotg_dev_code) + SIZEOF ( .usbotg_dev_code );	
	.uart_dev_code _uart_dev_code_image_addr : AT (_uart_dev_code_load_addr)
	{
        . = ALIGN(4);
	}
	_uart_dev_code_len =  SIZEOF (.uart_dev_code);		
	
 	/* DEV_CODE_END */	
/************************************************/

/********************LWIP START****************************/

	_wifiapp_task_code_image_addr 		= ADDR(.uart_dev_code) + SIZEOF ( .uart_dev_code );
    _wifiapp_task_code_load_addr		= LOADADDR(.uart_dev_code) + SIZEOF ( .uart_dev_code );	
	.wifiapp_task_code _wifiapp_task_code_image_addr : AT (_wifiapp_task_code_load_addr)
	{
 		*\wifithread.o
 		. = ALIGN(4);
	}
	_wifiapp_task_code_len =  SIZEOF (.wifiapp_task_code);	

	_ap6181wifi_dev_code_image_addr 		= ADDR(.wifiapp_task_code) + SIZEOF ( .wifiapp_task_code );
    _ap6181wifi_dev_code_load_addr		= LOADADDR(.wifiapp_task_code) + SIZEOF ( .wifiapp_task_code );	
	.ap6181wifi_dev_code _ap6181wifi_dev_code_image_addr : AT (_ap6181wifi_dev_code_load_addr)
	{
         *\waf.o
         *\wifi.o
         *\wiced_network.o
         *\wiced_network_common.o
         *\wiced_platform_common.o
         *\wiced_rtos_common.o
         *\wiced_rtos.o
         *\wwd_management.o
         *\wwd_sdpcm.o
         *\wwd_wifi.o
         *\wwd_ap.o
         *\wwd_rtos.o
         *\platform_mcu_powersave.o
         *\platform_watchdog.o
         *\wwd_buffer.o
         *\wwd_network.o
         *\wwd_platform_common.o
         *\wwd_thread.o
         *\wwd_internal.o
         *\wwd_resources.o
         *\tlv.o
         *\system_monitor.o
         *\platform_resource.o
         *\wwd_platform.o
         *\wwd_bus_protocol.o
		/* *\wwd_bus_common.o   */
         *\changhong.o
         *\easy_setup_wiced.o
         *\easy_setup.o
         *\akiss.o
         *\cooee.o
         *\neeze.o
         *(ap6181wice_wifi_init)
         . = ALIGN(4);
	}
	_ap6181wifi_dev_code_len =  SIZEOF (.ap6181wifi_dev_code);	
	

	_lwip_dev_code_image_addr 		= ADDR(.ap6181wifi_dev_code) + SIZEOF ( .ap6181wifi_dev_code );
    _lwip_dev_code_load_addr		= LOADADDR(.ap6181wifi_dev_code) + SIZEOF ( .ap6181wifi_dev_code );	
	.lwip_dev_code _lwip_dev_code_image_addr : AT (_lwip_dev_code_load_addr)
	{
        *\tcpip.o
        *\sys_arch.o
        *\def.o
        *\dhcp.o
        *\dns.o
        /* *\init.o */
        *\mem.o
        *\memp.o
        *\netif.o
        *\pbuf.o
        *\raw.o
        *\stats.o
        *\sys.o
        *\tcp.o
        *\tcp_in.o
        *\tcp_out.o
        *\udp.o
        *\icmp.o
        *\igmp.o
        *\inet.o
        *\inet_chksum.o
        *\ip.o
        *\ip_addr.o
        *\ip_frag.o
        *\etharp.o
        *\ethernetif.o
        *\slipif.o
        *\timer.o 
        *\ping.o	
        . = ALIGN(4);	
	}
	_lwip_dev_code_len =  SIZEOF (.lwip_dev_code);		
	
	_lwip_dev_api_code_image_addr 		= ADDR(.lwip_dev_code) + SIZEOF ( .lwip_dev_code );
    _lwip_dev_api_code_load_addr		= LOADADDR(.lwip_dev_code) + SIZEOF ( .lwip_dev_code );	
	.lwip_dev_api_code _lwip_dev_api_code_image_addr : AT (_lwip_dev_api_code_load_addr)
	{
        *\api_lib.o
        *\api_msg.o
        *\err.o
        *\netbuf.o
        *\netdb.o
        *\netifapi.o
        *\sockets.o
        . = ALIGN(4);
	}
	_lwip_dev_api_code_len =  SIZEOF (.lwip_dev_api_code);		
	
	_lwip_dev_http_code_image_addr 		= ADDR(.lwip_dev_api_code) + SIZEOF ( .lwip_dev_api_code );
    _lwip_dev_http_code_load_addr		= LOADADDR(.lwip_dev_api_code) + SIZEOF ( .lwip_dev_api_code );	
	.lwip_dev_http_code _lwip_dev_http_code_image_addr : AT (_lwip_dev_http_code_load_addr)
	{
        *\http.o
        . = ALIGN(4);
	}
	_lwip_dev_http_code_len =  SIZEOF (.lwip_dev_http_code);		
	
/********************LWIP END******************************/	


/************************************************/

	_effect_module_code_image_addr 		= ADDR(.lwip_dev_http_code) + SIZEOF ( .lwip_dev_http_code );
    _effect_module_code_load_addr		= LOADADDR(.lwip_dev_http_code) + SIZEOF ( .lwip_dev_http_code );	
	.effect_module_code _effect_module_code_image_addr : AT (_effect_module_code_load_addr)
	{
        *(codecs_audio_effect_effect_common)
        *(codecs_audio_effect_effect_common_data)
		*(codecs_audio_effect_effect_data)
        *\effect.o 
        *\filter_2_int_l.o 
        *\filter_2_int_r.o 
        *\rk_bass_asm_opt_16bit_gcc.o 
        *\rk_bass_asm_opt_24bit_gcc.o 
        *\rk_eq_asm_opt_gcc.o 
        *\rk_eq_asm_opt_16bit_gcc.o 
        *\rk_eq_asm_opt_24bit_gcc.o 
        . = ALIGN(4);
	}
	_effect_module_code_len =  SIZEOF (.effect_module_code);
	
	_fade_module_code_image_addr 		= ADDR(.effect_module_code) + SIZEOF ( .effect_module_code );
    _fade_module_code_load_addr		    = LOADADDR(.effect_module_code) + SIZEOF ( .effect_module_code );	
	.fade_module_code _fade_module_code_image_addr : AT (_fade_module_code_load_addr)
	{
        *(codecs_audio_fade_common)
		*(codecs_audio_fade_data)
        *\Fade.o
        . = ALIGN(4);
	}
	_fade_module_code_len =  SIZEOF (.fade_module_code);

/************************************************/
    /* AUDIO_DECODE_CODE_START */ 	
    
	AUDIO_DECODE_CODE_BASE	= ADDR(.fade_module_code) + SIZEOF ( .fade_module_code );
	AUDIO_DECODE_CODE_LOAD	= LOADADDR(.fade_module_code) + SIZEOF ( .fade_module_code );	
	
	OVERLAY AUDIO_DECODE_CODE_BASE : NOCROSSREFS  AT (AUDIO_DECODE_CODE_LOAD)
	{
		.mp3_audio_decode_code
		{
			_mp3_audio_decode_code_vma_ = ADDR(.mp3_audio_decode_code);
			*\\pMP3.o
	        *(Mp3InitCode)
	        *(Mp3DecCode)
	        *(Mp3DecData)
	        *(Mp3DecBss)
	        *(Mp3DecDataHL)	
	        . = ALIGN(4);	
		}
		.xxx_audio_decode_code
		{
			_xxx_audio_decode_code_vma_ = ADDR(.xxx_audio_decode_code);
			*\\pWMA.o
	        *(XXXDecCode)
	        *(XXXDecData)
	        *(XXXDevBss)	
	        . = ALIGN(4);
		}
		.wav_audio_decode_code
		{
			_wav_audio_decode_code_vma_ = ADDR(.wav_audio_decode_code);
			*\\pWAV.o
	        *(WavDecInitCode)
	        *(WavDecCode)
	        *(WavDecData)
	        *(WavDecBss)	
	        . = ALIGN(4);
		}	
		.flac_audio_decode_code
		{
			_flac_audio_decode_code_vma_ = ADDR(.flac_audio_decode_code);
			*\\pFLAC.o
	        *(FlacDecCode)
	        *(FlacDecData)
	        *(FlacDecBss)
	        . = ALIGN(4);
		}
		.aac_audio_decode_code
		{
			_aac_audio_decode_code_vma_ = ADDR(.aac_audio_decode_code);
			*\\pAAC.o
	        *(AacDecCode)
	        *(AacDecData)
	        *(AacDecBss)
	        . = ALIGN(4);
		}
		.ape_audio_decode_code
		{
			_ape_audio_decode_code_vma_ = ADDR(.ape_audio_decode_code);
			*\\apedec.o
	        *(ApeDecCode)
	        *(ApeDecData)
	        *(ApeDecBss)
	        . = ALIGN(4);
		}
		.ogg_audio_decode_code
		{
			_ogg_audio_decode_code_vma_ = ADDR(.ogg_audio_decode_code);
			*\\pOGG.o
	        *(OggDecCode)
	        *(OggDecData)
	        *(OggDecBss)
	        . = ALIGN(4);
		}	
		.flac_audio_hdecode_code
		{
			_flac_audio_hdecode_code_vma_ = ADDR(.flac_audio_hdecode_code);
			*\\p_hifi_flac.o
	        *(FlacHDecCode)
	        *(FlacHDecData)
	        *(FlacHDecBss)
	        . = ALIGN(4);
		}	
		.alac_audio_hdecode_code
		{
			_alac_audio_hdecode_code_vma_ = ADDR(.alac_audio_hdecode_code);
			*\\p_hifi_alac.o
	        *(AlacHDecCode)
	        *(AlacHDecData)
	        *(AlacHDecBss)
	        . = ALIGN(4);
		}
		.ape_audio_hdecode_code
		{
			_ape_audio_hdecode_code_vma_ = ADDR(.ape_audio_hdecode_code);
			*\\p_hifi_Ape.o
	        *(ApeHDecCode)
	        *(ApeHDecData)
	        *(ApeHDecBss)
	        . = ALIGN(4);
		}
		.sbc_audio_decode_code
		{
			_sbc_audio_decode_code_vma_ = ADDR(.sbc_audio_decode_code);
			*\\PsbcFunc.o
	        *(SbcDecCode)
	        *(SbcDecData)
	        *(SbcDecBss)
	        . = ALIGN(4);
		}
		.wav_audio_encode_code
		{
			_wav_audio_encode_code_vma_ = ADDR(.wav_audio_encode_code);
			*\\pWAVEnc.o
	        *(WavEncCode)
	        *(WavEncData)
	        *(WavEncBss)		
	        . = ALIGN(4);	
		}
		.xxx_audio_encode_code
		{
			_xxx_audio_encode_code_vma_ = ADDR(.xxx_audio_encode_code);
	        *(EncodeXXXCode)
	        *(EncodeXXXData)
	        *(EncodeXXXBss)		
	        . = ALIGN(4);
		}						
	}
	
    _mp3_audio_decode_code_len_		= SIZEOF (.mp3_audio_decode_code);	
    _xxx_audio_decode_code_len_		= SIZEOF (.xxx_audio_decode_code);	
    _wav_audio_decode_code_len_		= SIZEOF (.wav_audio_decode_code);	
    _flac_audio_decode_code_len_	= SIZEOF (.flac_audio_decode_code);	
    _aac_audio_decode_code_len_		= SIZEOF (.aac_audio_decode_code);	
    _ape_audio_decode_code_len_		= SIZEOF (.ape_audio_decode_code);	
    _ogg_audio_decode_code_len_		= SIZEOF (.ogg_audio_decode_code);	
    _flac_audio_hdecode_code_len_	= SIZEOF (.flac_audio_hdecode_code);	
    _alac_audio_hdecode_code_len_	= SIZEOF (.alac_audio_hdecode_code);	
    _ape_audio_hdecode_code_len_	= SIZEOF (.ape_audio_hdecode_code);	
    _sbc_audio_decode_code_len_		= SIZEOF (.sbc_audio_decode_code);	
    _wav_audio_encode_code_len_		= SIZEOF (.wav_audio_encode_code);	
    _xxx_audio_encode_code_len_		= SIZEOF (.xxx_audio_encode_code);	

    _audio_code_max_len		= _mp3_audio_decode_code_len_;
    _audio_code_max_len1 	= MAX(_audio_code_max_len,_xxx_audio_decode_code_len_);
    _audio_code_max_len2 	= MAX(_audio_code_max_len1,_wav_audio_decode_code_len_);
    _audio_code_max_len3 	= MAX(_audio_code_max_len2,_flac_audio_decode_code_len_);
    _audio_code_max_len4 	= MAX(_audio_code_max_len3,_aac_audio_decode_code_len_);
    _audio_code_max_len5 	= MAX(_audio_code_max_len4,_ape_audio_decode_code_len_);
    _audio_code_max_len6 	= MAX(_audio_code_max_len5,_ogg_audio_decode_code_len_);
    _audio_code_max_len7 	= MAX(_audio_code_max_len6,_flac_audio_hdecode_code_len_);
    _audio_code_max_len8 	= MAX(_audio_code_max_len7,_alac_audio_hdecode_code_len_);
    _audio_code_max_len9 	= MAX(_audio_code_max_len8,_ape_audio_hdecode_code_len_);
    _audio_code_max_len10 	= MAX(_audio_code_max_len9,_sbc_audio_decode_code_len_);
    _audio_code_max_len11 	= MAX(_audio_code_max_len10,_wav_audio_encode_code_len_);
    _audio_code_max_len12 	= MAX(_audio_code_max_len11,_xxx_audio_encode_code_len_);
    
    _next_code_image_addr = AUDIO_DECODE_CODE_BASE + _audio_code_max_len12;
    _next_code_load_addr  = __load_stop_wav_audio_encode_code; 
    
 	/* AUDIO_DECODE_CODE_END */ 	
/************************************************/


/************************************************/
	/*  TASK_CODE_START */
	_streamcontrol_task_code_image_addr 	= _next_code_image_addr;
    _streamcontrol_task_code_load_addr		= _next_code_load_addr;	
	.streamcontrol_task_code _streamcontrol_task_code_image_addr : AT (_streamcontrol_task_code_load_addr)
	{
        *(app_filestream_streamcontroltask_common)
        *\StreamControlTask.o
        . = ALIGN(4);
	}
	_streamcontrol_task_code_len =  SIZEOF (.streamcontrol_task_code);		

	_audiocontrol_task_code_image_addr 		= ADDR(.streamcontrol_task_code) + SIZEOF ( .streamcontrol_task_code );
    _audiocontrol_task_code_load_addr		= LOADADDR(.streamcontrol_task_code) + SIZEOF ( .streamcontrol_task_code );	
	.audiocontrol_task_code _audiocontrol_task_code_image_addr : AT (_audiocontrol_task_code_load_addr)
	{
        *(app_audio_audiocontroltask_common)
        *(app_audio_audiocontroltask_data)
        *\AudioControlTask.o
        . = ALIGN(4);
	}
	_audiocontrol_task_code_len =  SIZEOF (.audiocontrol_task_code);		
	
	_lineincontrol_task_code_image_addr 	= ADDR(.audiocontrol_task_code) + SIZEOF ( .audiocontrol_task_code );
    _lineincontrol_task_code_load_addr		= LOADADDR(.audiocontrol_task_code) + SIZEOF ( .audiocontrol_task_code );	
	.lineincontrol_task_code _lineincontrol_task_code_image_addr : AT (_lineincontrol_task_code_load_addr)
	{
        *(app_line_in_lineincontroltask_common)
        *\LineInControlTask.o
        . = ALIGN(4);
	}
	_lineincontrol_task_code_len =  SIZEOF (.lineincontrol_task_code);
	
	_recordcontrol_task_code_image_addr 	= ADDR(.lineincontrol_task_code) + SIZEOF ( .lineincontrol_task_code );
    _recordcontrol_task_code_load_addr		= LOADADDR(.lineincontrol_task_code) + SIZEOF ( .lineincontrol_task_code );	
	.recordcontrol_task_code _recordcontrol_task_code_image_addr : AT (_recordcontrol_task_code_load_addr)
	{
        *(app_record_recordcontroltask_common)
        *(app_record_recordcontroltask_data)
        *\RecordControlTask.o
        . = ALIGN(4);
	}
	_recordcontrol_task_code_len =  SIZEOF (.recordcontrol_task_code);			
	
	/*  TASK_CODE_END */
/************************************************/

/********************DLNA START****************************/	
	_dlna_task_code_load_addr  = LOADADDR(.recordcontrol_task_code) + SIZEOF (.recordcontrol_task_code);	
/*	_dlna_task_code_image_addr = ADDR(.recordcontrol_task_code) + SIZEOF (.recordcontrol_task_code);	*/	
	_dlna_task_code_image_addr = DLNA_CODE_BASE;		
    .dlna_task_code _dlna_task_code_image_addr : AT (_dlna_task_code_load_addr)
    {
    	_dlna_code_start_ = .;
		*\dlna.o
		. = ALIGN(4);	
    }
    _dlna_task_code_len =  SIZEOF (.dlna_task_code);	
    
 	_upnp_dev_code_load_addr  = LOADADDR(.dlna_task_code) + SIZEOF (.dlna_task_code);	
	_upnp_dev_code_image_addr = ADDR(.dlna_task_code) + SIZEOF (.dlna_task_code);	
    .upnp_dev_code _upnp_dev_code_image_addr : AT (_upnp_dev_code_load_addr)
    {
        *(upnpCode)
        *\chttp.o
        *\chttp_date.o
        *\chttp_header.o
        *\chttp_header_list.o
        *\chttp_packet.o
        *\chttp_persistent_connection.o
        *\chttp_request.o
        *\chttp_response.o
        *\chttp_server.o
        *\chttp_server_list.o
        *\cfile.o
        *\cfile.list.o
        *\cdatagram_packet.o
        *\cinterface.o
        *\cinterface_function.o
        *\cinterface_list.o
        *\cnet_function.o
        *\csocket.o
        *\csocket_list.o
        *\curi.o
        *\curl.o
        *\csoap.o
        *\csoap_request.o
        *\csoap_response.o
        *\caction_ctrl.o
        *\caction_request.o
        *\caction_response.o
        *\ccontrol_function.o
        *\cquery_ctrl.o
        *\cquery_request.o
        *\cquery_response.o
        *\ceventlistener_list.o
        *\cnotify_request.o
        *\cproperty.o
        *\cproperty_list.o
        *\csubscriber.o
        *\csubscriber_list.o
        *\csubscription.o
        *\csubscription_request.o
        *\csubscription_response.o
        *\chttpmu_socket.o
        *\chttpu_socket.o
        *\cssdp.o
        *\cssdp_packet.o
        *\cssdp_request.o
        *\cssdp_response.o
        *\cssdp_response_server.o
        *\cssdp_response_server_list.o
        *\cssdp_server.o
        *\cssdp_server_list.o
        *\cssdp_socket.o
        *\caction.o
        *\caction_list.o
        *\cargument.o
        *\cargument_list.o
        *\cdevice.o
        *\cdevice_advertiser.o
        *\cdevice_http_server.o
        *\cdevice_list.o
        *\cdevice_ssdp_server.o
        *\cicon.o
        *\cicon_list.o
        *\cservice.o
        *\cservice_list.o
        *\cservice_notify.o
        *\cservice_ssdp_server.o
        *\cservice_std_dcp.o
        *\cservicestate_table.o
        *\cstatevariable.o
        *\cupnp_function.o
        *\cupnp_status.o
        *\cdictionary.o
        *\cdictionary_elem.o
        *\clist.o
        *\clog.o
        *\cmutex.o
        *\cstring.o
        *\cstring_function.o
        *\cstring_tokenizer.o
        *\cthread.o
        *\cthread_list.o
        *\ctime.o
        *\ctime_wince.o
        *\cxml_attribute.o
        *\cxml_attribute_list.o
        *\cxml_function.o
        *\cxml_node.o
        *\cxml_node_list.o
        *\cxml_parser.o
        *\cxml_parser_expat.o
        *\cxml_parser_libxml2.o
        *\cavtransport_service.o
        *\cconnectionmgrr_service.o
        *\cmediarenderer_device.o
        *\crenderingcontrol_service.o
        *\cconnectionmgr_service.o
        *\ccontentdir_service.o
        *\cmediaserver_device.o
        *\ccontent.o
        *\ccontent_list.o
        *\cdidl.o
        *\cmd5.o
        *\cprotocolinfo.o
        *\cprotocolinfo_list.o
        *\cresource.o
        *\cresource_data.o
        *\cresource_list.o
        /* *\rk_heap.o */
        . = ALIGN(4);
    } 
	_upnp_dev_code_len =  SIZEOF (.upnp_dev_code);	  

	_xmlparse_dev_code_load_addr  = LOADADDR(.upnp_dev_code) + SIZEOF (.upnp_dev_code);	
	_xmlparse_dev_code_image_addr = ADDR(.upnp_dev_code) + SIZEOF (.upnp_dev_code);	
    .xmlparse_dev_code _xmlparse_dev_code_image_addr : AT (_xmlparse_dev_code_load_addr)
    {
		*\xmlparse.o
        *\xmlrole.o
        *\xmltok.o
        *\xmltok_impl.o
        *\xmltok_ns.o
        . = ALIGN(4);
    }
 	_xmlparse_dev_code_len =  SIZEOF (.xmlparse_dev_code);	  
 	   
	_dlna_dev_code_load_addr  = LOADADDR(.xmlparse_dev_code) + SIZEOF (.xmlparse_dev_code);	
	_dlna_dev_code_image_addr = ADDR(.xmlparse_dev_code) + SIZEOF (.xmlparse_dev_code);	
    .dlna_dev_code _dlna_dev_code_image_addr : AT (_dlna_dev_code_load_addr)
    {
        *\controller.o
        *\mplayer-ctrl.o
        *\player.o
        *\playlist.o
        *\dmr.o
        *\dmr-action.o
        *\dmr-http.o
        *\dmr-avtransport-service.o
        *\dmr-connectionmgr-service.o
        *\dmr-renderingctrl-service.o
        *\dms.o
        *\dms_filesys.o
        *\dms_filesys_data.o
        *\dms-action.o
        *\dms-http.o
        *\dms-playlist.o
        *\descriptions_load.o
        *\encoding_trans.o
        *\rewrite_api.o
        *\rk_list.o
        *\udn.o
        *\dlna-service.o
        . = ALIGN(4);
 		_dlna_code_end_ = .;       
    }    
 	_dlna_dev_code_len =  SIZEOF (.dlna_dev_code);	
 	
 	_dlna_code_len_ = _dlna_code_end_ -_dlna_code_start_ ;
 	    
/********************DLNA END******************************/	


/************************************************/		
 	/* SYS_CODE_START */	
	_image_load_addr	= LOADADDR(.dlna_dev_code) + SIZEOF ( .dlna_dev_code );
/*	_image_start_addr 	= ADDR(.dlna_dev_code) + SIZEOF (.dlna_dev_code);	*/
	_image_start_addr 	= ADDR(.recordcontrol_task_code) + SIZEOF (.recordcontrol_task_code);	
    .ap_sys_code _image_start_addr : AT (_image_load_addr)
    {
        KEEP(*(.interrupt_vector .interrupt_vector.*))        
        *(AP_START_VECT)
        *(AP_START_CODE)
        *(cpu_nanod_lib_pmc_common)
        *(system_shell_shelltask_common)
        *\ShellTask.o(.text .text.*)
        *\ShellTask.o(.rodata .rodata*)
        *(os_fwanalysis_fwupdate_common)
        . = ALIGN(4);
    }

	_sys_text_code_image_addr = ADDR(.ap_sys_code) + SIZEOF ( .ap_sys_code );	
	_sys_text_code_load_addr  = LOADADDR(.ap_sys_code) + SIZEOF ( .ap_sys_code );	
	.sys_text_code (_sys_text_code_image_addr) : AT (_sys_text_code_load_addr)
	{
        *(.text .text.* .gnu.linkonce.t.*)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        . = ALIGN(4);	
	}	
	_sys_text_code_len_ = SIZEOF(.sys_text_code);
	
	_temp_image_addr = ADDR(.sys_text_code) + SIZEOF ( .sys_text_code );	
	_temp_load_addr  = LOADADDR(.sys_text_code) + SIZEOF ( .sys_text_code );	
	.temp (_temp_image_addr) : AT (_temp_load_addr)
	{
		*(bsp_evk_v20_bsp_common)
		*(driver_dma_dmadevice_common)		
		*(driver_lun_lundevice_read)		
		*(driver_lun_lundevice_init)
		*(driver_spiflash_spiflashdev_common)
		*(driver_spi_spidevice_init)		
		*(driver_uart_uartdevie_read)
		*(os_devicemanager_devicemanagertask_common)
		*(os_fwanalysis_fwanalysis_common)
		*(os_plugin_rkos_common)
		*(os_plugin_taskplugin_common)
		*(os_plugin_taskplugin_shell)
		*(system_moduleoverlay_sysinfosave_common)
		*(system_syssever_powermanager_common)
		*(cpu_nanod_lib_pmu_common)
		*(driver_spi_spidevice_read)
		
		*(ap6181wice_wifi_init)	
		. = ALIGN(4);
		
	}
	
	_c_cpp_lib_image_addr = ADDR(.temp) + SIZEOF ( .temp );	
	_c_cpp_lib_load_addr  = LOADADDR(.temp) + SIZEOF ( .temp );	
	.c_cpp_lib (_c_cpp_lib_image_addr) : AT (_c_cpp_lib_load_addr)
	{
        *(.glue_7t) *(.glue_7)	
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		
        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(0x4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);	
	}
	_c_cpp_lib_len_ = SIZEOF(.c_cpp_lib);
	
	_exidx_image_addr = ADDR(.c_cpp_lib) + SIZEOF ( .c_cpp_lib );	
	_exidx_load_addr = LOADADDR(.c_cpp_lib) + SIZEOF ( .c_cpp_lib );	
	
    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx (_exidx_image_addr) : AT (_exidx_load_addr)
    {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
      *(.vfp11_veneer .v4_bx .iplt)
      . = ALIGN(4);
    }
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4);
   
	_image_end_addr = .; 
	_image_end_leng = _image_end_addr - _image_start_addr; 
	
 	/* SYS_CODE_END */	
/************************************************/	



	_pmu_code_load_addr  = LOADADDR(.ARM.exidx) + SIZEOF (.ARM.exidx);	
	_pmu_code_image_addr = PMU_CODE_BASE;	
    .pmu_code _pmu_code_image_addr : AT (_pmu_code_load_addr)
    {
		*(pmucode)
    }
      
   	_pmu_code_len =  SIZEOF (.pmu_code);		
   	
	_ap_sys_data_load_addr = LOADADDR(.pmu_code) + SIZEOF (.pmu_code);	
	 _etext = LOADADDR(.ap_sys_data);
    .ap_sys_data PMU_DATA_BASE : AT (_ap_sys_data_load_addr)
    {
        . = ALIGN(4);
        _srelocate = .;
		KEEP(*(ap_vect ap_vect*))
        *(DevInfo_Table_)
        *\SysInfoSave.o(.data .data.*)
        *\FwAnalysis.o(.bss .bss.*)  
        *\FwAnalysis.o(COMMON)      
        *\ShellTask.o(.data .data.*)  
        *\ShellTask.o(.bss .bss.*)  
        *\ShellTask.o(COMMON)      
        . = ALIGN(4); 	
    }
    _ap_sys_data_len_ = SIZEOF(.ap_sys_data);
   
  
  	_sys_data_code_image_addr = ADDR(.ap_sys_data) + SIZEOF ( .ap_sys_data );	
	_sys_data_code_load_addr  = LOADADDR(.ap_sys_data) + SIZEOF ( .ap_sys_data );	
	.sys_data_code (_sys_data_code_image_addr) : AT (_sys_data_code_load_addr)
	{
        *(.data .data.*);
        . = ALIGN(4);
	}	
	_sys_text_code_len_ = SIZEOF(.sys_text_code);  
	
	_erelocate = .;
	_relocate_len = _erelocate - _srelocate ;


  	_pmu_data_image_addr = PMU_DATA_BASE;	
	_pmu_data_load_addr  = LOADADDR(.sys_data_code) + SIZEOF ( .sys_data_code );	
	.pmu_data (_pmu_data_image_addr) : AT (_pmu_data_load_addr)
	{
		/* KEEP(*.o(pmu_vect)) */
	}	
	_pmu_data_len  = SIZEOF(.pmu_data);  

	_pmu_data_bss_start = ADDR(.pmu_data) + SIZEOF ( .pmu_data );
	.pmu_bss (_pmu_data_bss_start) (NOLOAD) :
	{	
	
	}
	_pmu_data_bss_len =  SIZEOF(.pmu_bss);	
		
    /* .bss section which is used for uninitialized data */
    .bss (ADDR(.sys_data_code) + SIZEOF ( .sys_data_code )) (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = . ;
		_bss_start = . ;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = . ;
    }
	_bss_len = _ebss - _bss_start;

	.init_data (ADDR(.bss) + SIZEOF ( .bss)) :  AT (LOADADDR(.pmu_data) + SIZEOF ( .pmu_data ))
	{
	
	}

	
    /* .stack_dummy section doesn't contains any symbols. It is only
       used for linker to calculate size of stack sections, and assign
       values to stack symbols later */
    .stack_dummy (ADDR(.init_data) + SIZEOF ( .init_data )) (NOLOAD) :
    {
   		 _main_stack_start = .;
        KEEP(*(APMainStack))
        _main_stack_end = .;
    }
	. = ALIGN(8);
    .heap_stack(ADDR(.stack_dummy) + SIZEOF ( .stack_dummy )) (NOLOAD) :
    {
        KEEP(*(HeapStack))
    }
 	. = ALIGN(8);
    _end = . ;
    _lm_heap_start = _end;
    _lm_heap_end = PMU_DATA_LIMIT;
    _lm_heap_len = _lm_heap_end - _lm_heap_start;
    
	. = ALIGN(8);
    .app_heap_stack(SYS_DATA_BASE) (NOLOAD) :
    {
        KEEP(*(AppHeapStack))
    }   

	_bcore_code_load_addr	= LOADADDR(.init_data) + SIZEOF (.init_data);
	OVERLAY HRAM_CODE_BASE : NOCROSSREFS  AT (_bcore_code_load_addr)
	{
		.bb_sys_code
		{
		 	_bb_sys_code_vma_ = ADDR(.bb_sys_code);
			KEEP(*(BBSysCode BBSysCode.*))
		}
		.xxx_encode_code
		{
		 	_xxx_encode_code_vma_ = ADDR(.xxx_encode_code);
			KEEP(*(XXXEncBinCode XXXEncBinCode.*))		
		}
		.wav_encode_code
		{
		 	_wav_encode_code_vma_ = ADDR(.wav_encode_code);
			KEEP(*(WavEncBinCode WavEncBinCode.*))			
		}
		.sbc_decode_code
		{
		 	_sbc_decode_code_vma_ = ADDR(.sbc_decode_code);
			KEEP(*(SbcDecBinCode SbcDecBinCode.*))
		}
		.mp3_decode_code
		{
		 	_mp3_decode_code_vma_ = ADDR(.mp3_decode_code);
			KEEP(*(Mp3DecBinCode Mp3DecBinCode.*))
		}		
		.xxx_decode_code
		{
		 	_xxx_decode_code_vma_ = ADDR(.xxx_decode_code);
			KEEP(*(XXXCommonBinCode XXXCommonBinCode.*))
		}	
		.wav_decode_code
		{
		 	_wav_decode_code_vma_ = ADDR(.wav_decode_code);
			KEEP(*(WavDecBinCode WavDecBinCode.*))
		}
		.flac_decode_code
		{
		 	_flac_decode_code_vma_ = ADDR(.flac_decode_code);
			KEEP(*(FlacDecBinCode FlacDecBinCode.*))
		}
		.aac_decode_code
		{
		 	_aac_decode_code_vma_ = ADDR(.aac_decode_code);
			KEEP(*(AacDecBinCode AacDecBinCode.*))
		}
		.ape_decode_code
		{
		 	_ape_decode_code_vma_ = ADDR(.ape_decode_code);
			KEEP(*(ApeDecBinCode ApeDecBinCode.*))		
		}
		.ogg_decode_code
		{
		 	_ogg_decode_code_vma_ = ADDR(.ogg_decode_code);
			KEEP(*(OggDecBinCode OggDecBinCode.*))		
		}	
		.flac_hdecode_code
		{
		 	_flac_hdecode_code_vma_ = ADDR(.flac_hdecode_code);
			KEEP(*(FlacHDecBinCode FlacHDecBinCode.*))		
		}
		.alac_hdecode_code
		{
		 	_alac_hdecode_code_vma_ = ADDR(.alac_hdecode_code);
			KEEP(*(AlacHDecBinCode AlacHDecBinCode.*))		
		}	
		.ape_hdecode_code
		{
		 	_ape_hdecode_code_vma_ = ADDR(.ape_hdecode_code);
			KEEP(*(ApeHDecBinCode ApeHDecBinCode.*))		
		}
		
	}
	_bb_sys_code_len_		= SIZEOF (.bb_sys_code);
	_xxx_encode_code_len_	= SIZEOF (.xxx_encode_code);	
	_wav_encode_code_len_	= SIZEOF (.wav_encode_code);	
	_sbc_decode_code_len_	= SIZEOF (.sbc_decode_code);	
	_mp3_decode_code_len_	= SIZEOF (.mp3_decode_code);	
	_xxx_decode_code_len_	= SIZEOF (.xxx_decode_code);	
	_wav_decode_code_len_	= SIZEOF (.wav_decode_code);	
	_flac_decode_code_len_	= SIZEOF (.flac_decode_code);	
	_aac_decode_code_len_	= SIZEOF (.aac_decode_code);
	_ape_decode_code_len_	= SIZEOF (.ape_decode_code);	
	_ogg_decode_code_len_	= SIZEOF (.ogg_decode_code);	
	_flac_hdecode_code_len_	= SIZEOF (.flac_hdecode_code);	
	_alac_hdecode_code_len_	= SIZEOF (.alac_hdecode_code);	
	_ape_hdecode_code_len_	= SIZEOF (.ape_hdecode_code);	
	
	
	OVERLAY HRAM_DATA_BASE : NOCROSSREFS  AT (__load_stop_ape_hdecode_code)
	{
		.bb_sys_data
		{
		 	_bb_sys_data_vma_ = ADDR(.bb_sys_data);
			KEEP(*(BBSysData BBSysData.*))
			KEEP(*(BBSysBss BBSysBss.*))
		}
		.xxx_encode_data
		{
		 	_xxx_encode_data_vma_ = ADDR(.xxx_encode_data);
			KEEP(*(XXXEncBinData XXXEncBinData.*))
			KEEP(*(XXXEncBinBss XXXEncBinBss.*))		
		}
		.wav_encode_data
		{
		 	_wav_encode_data_vma_ = ADDR(.wav_encode_data);
			KEEP(*(WavEncBinData WavEncBinData.*))
			KEEP(*(WavEncBinBss WavEncBinBss.*))	
		}
		.sbc_decode_data
		{
		 	_sbc_decode_data_vma_ = ADDR(.sbc_decode_data);
			KEEP(*(SbcDecBinData SbcDecBinData.*))
			KEEP(*(SbcDecBinBss SbcDecBinBss.*))
		}
		.mp3_decode_data
		{
		 	_mp3_decode_data_vma_ = ADDR(.mp3_decode_data);
			KEEP(*(Mp3DecBinData Mp3DecBinData.*))
			KEEP(*(Mp3DecBinBss Mp3DecBinBss.*))
		}
		.xxx_decode_data
		{
		 	_xxx_decode_data_vma_ = ADDR(.xxx_decode_data);
			KEEP(*(XXXCommonBinData XXXCommonBinData.*))
			KEEP(*(XXXCommonBinBss XXXCommonBinBss.*))
		}
		.wav_decode_data
		{
		 	_wav_decode_data_vma_ = ADDR(.wav_decode_data);
			KEEP(*(WavDecBinData WavDecBinData.*))
			KEEP(*(WavDecBinBss WavDecBinBss.*))
		}
		.flac_decode_data
		{
		 	_flac_decode_data_vma_ = ADDR(.flac_decode_data);
			KEEP(*(FlacDecBinData FlacDecBinData.*))
			KEEP(*(FlacDecBinBss FlacDecBinBss.*))
		}
		.aac_decode_data
		{
		 	_aac_decode_data_vma_ = ADDR(.aac_decode_data);
			KEEP(*(AacDecBinData AacDecBinData.*))
			KEEP(*(AacDecBinBss AacDecBinBss.*))
		}
		.ape_decode_data
		{
		 	_ape_decode_data_vma_ = ADDR(.ape_decode_data);
			KEEP(*(ApeDecBinData ApeDecBinData.*))
			KEEP(*(ApeDecBinBss ApeDecBinBss.*))		
		}
		.ogg_decode_data
		{
		 	_ogg_decode_data_vma_ = ADDR(.ogg_decode_data);
			KEEP(*(OggDecBinData OggDecBinData.*))
			KEEP(*(OggDecBinBss OggDecBinBss.*))		
		}
		.flac_hdecode_data
		{
		 	_flac_hdecode_data_vma_ = ADDR(.flac_hdecode_data);
			KEEP(*(FlacHDecBinData FlacHDecBinData.*))
			KEEP(*(FlacHDecBinBss FlacHDecBinBss.*))		
		}
		.alac_hdecode_data
		{
		 	_alac_hdecode_data_vma_ = ADDR(.alac_hdecode_data);
			KEEP(*(AlacHDecBinData AlacHDecBinData.*))
			KEEP(*(AlacHDecBinBss AlacHDecBinBss.*))		
		}					
		.ape_hdecode_data
		{
		 	_ape_hdecode_data_vma_ = ADDR(.ape_hdecode_data);
			KEEP(*(ApeHDecBinData ApeHDecBinData.*))
			KEEP(*(ApeHDecBinBss ApeHDecBinBss.*))		
		}	
		
	}
	
	_bb_sys_data_len_		= SIZEOF (.bb_sys_data);
	_xxx_encode_data_len_	= SIZEOF (.xxx_encode_data);	
	_wav_encode_data_len_	= SIZEOF (.wav_encode_data);	
	_sbc_decode_data_len_	= SIZEOF (.sbc_decode_data);	
	_mp3_decode_data_len_	= SIZEOF (.mp3_decode_data);	
	_xxx_decode_data_len_	= SIZEOF (.xxx_decode_data);	
	_wav_decode_data_len_	= SIZEOF (.wav_decode_data);	
	_flac_decode_data_len_	= SIZEOF (.flac_decode_data);	
	_aac_decode_data_len_	= SIZEOF (.aac_decode_data);
	_ape_decode_data_len_	= SIZEOF (.ape_decode_data);		
	_ogg_decode_data_len_	= SIZEOF (.ogg_decode_data);	
	_flac_hdecode_data_len_	= SIZEOF (.flac_hdecode_data);	
	_alac_hdecode_data_len_	= SIZEOF (.alac_hdecode_data);	
	_ape_hdecode_data_len_	= SIZEOF (.ape_hdecode_data);

	OVERLAY HRAM_DATA_BASE : NOCROSSREFS  AT (__load_stop_ape_hdecode_data)
	{
		.eq_data
		{
		 	_eq_data_vma_ = ADDR(.eq_data);
			KEEP(*(RkEqData RkEqData.*))		
		}
		.wifi_fw
		{
		 	_wifi_fw_vma_ = ADDR(.wifi_fw);
			KEEP(*(_wifi_fw_ _wifi_fw_.*))
		}
		.wifi_nvram
		{
		 	_wifi_nvram_vma_ = ADDR(.wifi_nvram);
			KEEP(*(wifi_nvram wifi_nvram.*))
		}


	}
	_wifi_fw_len_									= SIZEOF (.wifi_fw);	
	_wifi_nvram_len_								= SIZEOF (.wifi_nvram);		
	_eq_data_len_									= SIZEOF (.eq_data);



}
