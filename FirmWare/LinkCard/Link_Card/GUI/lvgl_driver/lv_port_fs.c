/**
 * @file lv_port_fs_templ.c
 *
 */

 /*Copy this file as "lv_port_fs.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_fs.h"
#include "fatfs.h"
#include "lv_fs.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/* Create a type to store the required data about your file.
 * If you are using a File System library
 * it already should have a File type.
 * For example FatFS has `FIL`. In this case use `typedef FIL file_t`*/
//typedef struct {
//    /*Add the data you need to store about a file*/
//    uint32_t dummy1;
//    uint32_t dummy2;
//}file_t;

///*Similarly to `file_t` create a type for directory reading too */
//typedef struct {
//    /*Add the data you need to store about directory reading*/
//    uint32_t dummy1;
//    uint32_t dummy2;
//}dir_t;


typedef FIL file_t;		// 把FIL类型定义成file_t
typedef DIR dir_t; 		// 把DIR类型定义成dir_t



/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static lv_fs_res_t fs_open(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t* drv, void* file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br);
static lv_fs_res_t fs_write(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos);
static lv_fs_res_t fs_size(lv_fs_drv_t* drv, void* file_p, uint32_t* size_p);
static lv_fs_res_t fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p);
static lv_fs_res_t fs_remove(lv_fs_drv_t* drv, const char* path);
static lv_fs_res_t fs_trunc(lv_fs_drv_t* drv, void* file_p);
static lv_fs_res_t fs_rename(lv_fs_drv_t* drv, const char* oldname, const char* newname);
static lv_fs_res_t fs_free(lv_fs_drv_t* drv, uint32_t* total_p, uint32_t* free_p);
static lv_fs_res_t fs_dir_open(lv_fs_drv_t* drv, void* rddir_p, const char* path);
static lv_fs_res_t fs_dir_read(lv_fs_drv_t* drv, void* rddir_p, char* fn);
static lv_fs_res_t fs_dir_close(lv_fs_drv_t* drv, void* rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/
FATFS   fs_lv_sd;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_fs_init(void)
{
     /*----------------------------------------------------
     * Initialize your storage device and File System
     * -------------------------------------------------*/
    
    /* 这个函数实现在下面，里头是空的，由你自己实现 */
    fs_init();

    /*---------------------------------------------------
     * Register the file system interface  in LVGL
     *--------------------------------------------------*/

    /* Add a simple drive to open images */
    
    lv_fs_drv_t fs_drv;
	/* 上面这里默认只有一个fs_drv，如果我有多个存储设备的话，我们可以这样 */
	// lv_fs_drv_t fs_drv[FF_VOLUMES]; // FF_VOLUMES 定义在fatfs的ffconf.h文件中

    lv_fs_drv_init(&fs_drv);

    /*Set up fields...*/
    // ↓ 这里的file_t的size会被LVGL使用文件系统时调用进行内存分配，所以你上面一定要改好
    fs_drv.file_size = sizeof(file_t);	
    // ↓ 这里letter是比较重要的一个东西，比如调用SD卡时，我们直接“S:/xxx/xxx”即可
    // 如果我还用SPI Flash的话，这里的letter也可以写成F，避免和S冲突
    fs_drv.letter = '0';
    /* 下面就是各个接口的注册 */
    fs_drv.open_cb = fs_open;			// 打开
    fs_drv.close_cb = fs_close;			// 关闭
    fs_drv.read_cb = fs_read;			// 读
    fs_drv.write_cb = fs_write;			// 写
    fs_drv.seek_cb = fs_seek;			// 寻址

	/* 注册设备 */
    lv_fs_drv_register(&fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Initialize your Storage device and File system. */
static void fs_init(void)
{
     /*E.g. for FatFS initalize the SD card and FatFS itself*/
    /*You code here*/
    
    /* 这里是文件系统和你存储设备初始化的地方，你所有的设备最好别在别的地方初始化了 */
	/* 到这里都会进行初始化和挂载 */
//    FATFS * fs = (FATFS*)lv_mem_alloc(sizeof(FATFS));
	FRESULT fres = FR_NOT_READY;


	/* 挂载设备 */
  	if (fres != FR_OK) {
  		/* 这里的驱动号我直接使用字符串“SD:”，在ffconf.h文件中的FF_VOLUME_STRS定义 */
    	fres = f_mount(&fs_lv_sd, "0:", 1);
    	if (fres != FR_OK) {
      		DEBUG_PRINT("SD Card mounted error. (%d)\n", fres );
    	} else
	    	DEBUG_PRINT("SD Card mounted successfully.\n\n");
  	}

//	lv_mem_free(fs);
}

/**
 * Open a file
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode)
{   
	lv_fs_res_t res = LV_FS_RES_NOT_IMP;

	/************************************************
   	* 在这里，我们需要先进行设备的判断，因为我们实际上
   	* 可能不只有一个设备，而且LVGL里使用letter（即单个字符）
   	* 作为驱动号
   	* 而在FatFS里使用VOLUME_STR（字符串）或者一个字节的pdrv
   	* 驱动号，使用前要进行转换。
   	* 
   	* 有一种方便的方法，就是上面提到的user_data，我们可以把
   	* VOLUME_STR提前装入user_data，那我们的程序进行判断时
   	* 耦合度会更低，在更多设备时，这种方法会更方便
 	 *************************************************/

	char *path_buf = NULL;
  	uint8_t opt_mode = 0;
	uint16_t path_len = strlen(path);
	
  	// 根据传入的letter判断是什么存储设备
  	switch (drv->letter) {
  	case '0':       // SD card
//  		printf("SD drv detect\r\n");
    	break;
  	case 'F':       // SPI FALSH
  		path_buf = (char *)lv_mem_alloc(sizeof(char) * (path_len + 6));
    	sprintf(path_buf, "SPIF:/%s", path);
    	break;
  	default:
    	printf("No drive %c\n", drv->letter);
    	return LV_FS_RES_NOT_EX;
  	}
  	
	/* 文件操作方法，将FatFS的转换成LVGL的操作方法 */
    if(mode == LV_FS_MODE_WR) {
    	opt_mode = FA_CREATE_ALWAYS | FA_WRITE;
    } else if(mode == LV_FS_MODE_RD) {
    	opt_mode = FA_OPEN_EXISTING | FA_READ;
    } else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) {
		opt_mode = FA_WRITE | FA_READ;
    }

	/* 调用FatFs的函数 */
  	FRESULT fres = f_open((FIL*)file_p, path, opt_mode);
	if (fres != FR_OK) {
    	printf("f_open error (%d)\n", fres);
    	res = LV_FS_RES_NOT_IMP;
  	} else
    	res = LV_FS_RES_OK;

	lv_mem_free(path_buf);

    return res;
}


/**
 * Close an opened file
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close (lv_fs_drv_t* drv,void * file_p)
{
    if (f_close((FIL*)file_p) != FR_OK)
    	return LV_FS_RES_NOT_IMP;
  	else
    	return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read (lv_fs_drv_t* drv, void *file_p,void * buf, uint32_t btr, uint32_t * br)
{
    FRESULT fres = f_read((FIL*)file_p, buf, btr, br);
  	if (fres != FR_OK) {
    	printf("f_read error (%d)\n", fres);
    	return LV_FS_RES_NOT_IMP;
  	} else 
    	return LV_FS_RES_OK;
}

/**
 * Write into a file
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t* drv,void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    /* Add your code here*/
  	FRESULT fres = f_write((FIL*)file_p, buf, btw, bw);
  	if (fres != FR_OK) {
    	printf("f_write error (%d)\n", fres);
    	return LV_FS_RES_NOT_IMP;
  	} else
    	return LV_FS_RES_OK;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek (lv_fs_drv_t* drv,void * file_p, uint32_t pos)
{
    FRESULT fres = f_lseek((FIL*)file_p, pos);
 	 if (fres != FR_OK) {
    	printf("f_lseek error (%d)\n", fres);
    	return LV_FS_RES_NOT_IMP;
  	} else
    	return LV_FS_RES_OK;
}

/**
 * Give the size of a file bytes
 * @param file_p pointer to a file_t variable
 * @param size pointer to a variable to store the size
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_size (lv_fs_drv_t* drv,void * file_p, uint32_t * size_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}
/**
 * Give the position of the read write pointer
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell (lv_fs_drv_t* drv,void * file_p, uint32_t * pos_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Delete a file
 * @param path path of the file to delete
 * @return  LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_remove (lv_fs_drv_t* drv,const char *path)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Truncate the file size to the current position of the read write pointer
 * @param file_p pointer to an 'ufs_file_t' variable. (opened with lv_fs_open )
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_trunc (lv_fs_drv_t* drv,void * file_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Rename a file
 * @param oldname path to the file
 * @param newname path with the new name
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
static lv_fs_res_t fs_rename (lv_fs_drv_t* drv,const char * oldname, const char * newname)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Get the free and total size of a driver in kB
 * @param letter the driver letter
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free size [kB]
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_free (lv_fs_drv_t* drv,uint32_t * total_p, uint32_t * free_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Initialize a 'fs_read_dir_t' variable for directory reading
 * @param rddir_p pointer to a 'fs_read_dir_t' variable
 * @param path path to a directory
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_open (lv_fs_drv_t* drv,void * rddir_p, const char *path)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param rddir_p pointer to an initialized 'fs_read_dir_t' variable
 * @param fn pointer to a buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read (lv_fs_drv_t* drv,void * rddir_p, char *fn)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Close the directory reading
 * @param rddir_p pointer to an initialized 'fs_read_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close (lv_fs_drv_t* drv,void * rddir_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
