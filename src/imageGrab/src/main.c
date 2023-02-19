#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <jpeglib.h>

#include "ak_common.h"
#include "ak_vi.h"
//#include "ak_venc.h"

#include "http_server.h"

#include "defs.h"
#include "log.h"

// Debug errors
char *out_dir = SNAPSHOT_DEFAULT_DIR;

int res_w = 1280, res_h = 720;


// Camera vars
void *vi_handle = NULL;						//vi operating handle
struct video_channel_attr attr;				//vi channel attribute
struct video_resolution res;				//max sensor resolution

const char *cfg = "/etc/jffs2/isp_gc1054.conf";

int keepCapturing = 1;

void SIGINT_handler(int s){
   printf("Caught signal %d - Exiting... \n",s);
	 keepCapturing = 0;
   usleep(200);
   exit(1);
}

struct snapshot_t snapshot_ref = {
	.count = 0,
	.ready = NULL,
	.capture = 0
};

void stop_capture(){

	logi("stop_capture");

	/*
	 * step 7: release resource
	 */
	ak_vi_capture_off(vi_handle);

	ak_vi_close(vi_handle);

}

int capture_init(){

	// Creating the named file(FIFO)
	// This is used for IPC communication.
	
	
	// logi("Create fifo " QRCODE_WRITE_FIFO);
	// mkfifo(QRCODE_WRITE_FIFO, 0666);

	logi("image module init");

	int ret = -1;								//return value

	attr.crop.left = 0;
	attr.crop.top  = 0;
	attr.crop.width = 1280;
	attr.crop.height = 720;

	printf("VIDEO_CHN_SUB index %d \n", VIDEO_CHN_SUB);
	printf("VIDEO_CHN_MAIN index %d \n", VIDEO_CHN_MAIN);

	attr.res[VIDEO_CHN_MAIN].width = 1280;
	attr.res[VIDEO_CHN_MAIN].height = 720;
	attr.res[VIDEO_CHN_SUB].width = 640;
	attr.res[VIDEO_CHN_SUB].height = 360;

	// Some horrible HACK right here ;-;
	attr.res[VIDEO_CHN_SUB].max_width = 1280;
	attr.res[VIDEO_CHN_SUB].max_height = 720;
	attr.res[VIDEO_CHN_MAIN].max_width = 640;
	attr.res[VIDEO_CHN_MAIN].max_height = 360;
	
	// logi("Create fifo " QRCODE_WRITE_FIFO);
	// mkfifo(QRCODE_WRITE_FIFO, 0666);

	/*
	 * step 1: match sensor
	 * the location of isp config can either a file or a directory
	 */
	ret = ak_vi_match_sensor(cfg);
	if (ret) {
		loge("match sensor failed\n");
		return -1;
	}

	/*
	 * step 2: open video input device
	 */
	vi_handle = ak_vi_open(VIDEO_DEV0);
	if (NULL == vi_handle) {
		loge("vi device open failed\n");
		return -1;
	}

	/*
	 * step 3: get sensor support max resolution
	 */
	ret = ak_vi_get_sensor_resolution(vi_handle, &res);
	logv("ak_vi_get_sensor_resolution\n");
	if (ret) {
		ak_vi_close(vi_handle);
		return -1;
	} else {
		attr.crop.width = res.width;
		attr.crop.height = res.height;
	}

	logv("mw: %d, mh: %d, sw: %d, sh: %d\n",
		attr.res[VIDEO_CHN_MAIN].width, attr.res[VIDEO_CHN_MAIN].height,
		attr.res[VIDEO_CHN_SUB].width, attr.res[VIDEO_CHN_SUB].height);

	/*
	 * step 4: set vi working parameters
	 * default parameters: 25fps, day mode, auto frame-control
	 */
	ret = ak_vi_set_channel_attr(vi_handle, &attr);
	logv("ak_vi_set_channel_attr\n");
	if (ret) {
		loge("ak_vi_set_channel_attr FAILLL GO TO CLOSE\n");
		ak_vi_close(vi_handle);
		return -1;
	}

	/*
	 * step 5: start capture frames
	 */
	ret = ak_vi_capture_on(vi_handle);
	logv("ak_vi_capture_on\n");
	if (ret) {
		ak_vi_close(vi_handle);
		return -1;
	}

	// qrcode_init();

	return 1;
}

#define WORD  unsigned short
#define DWORD unsigned int
#define LONG  unsigned int

/* Bitmap header */
typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
}__attribute__((packed)) BITMAPFILEHEADER;

/* Bitmap info header */
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}__attribute__((packed)) BITMAPINFOHEADER;

typedef int (*ConverFunc)(void *rgbData,void *yuvData,int width,int height);

int YUV420ToRGB24(void *RGB24,void *YUV420,int width,int height)
{
    unsigned char *src_y = (unsigned char *)YUV420;
    unsigned char *src_u = (unsigned char *)YUV420 + width * height;
    unsigned char *src_v = (unsigned char *)YUV420 + width * height * 5 / 4;

    unsigned char *dst_RGB = (unsigned char *)RGB24;

    int temp[3];

    if(RGB24 == NULL || YUV420 == NULL || width <= 0 || height <= 0)
    {
        printf(" YUV420ToRGB24 incorrect input parameter!\n");
        return -1;
    }

    for(int y = 0;y < height;y ++)
    {
        for(int x = 0;x < width;x ++)
        {
            int Y = y*width + x;
            int U = (y >> 1)*(width >> 1) + (x >> 1);
            int V = U;

            temp[0] = src_y[Y] + ((7289 * src_u[U])>>12) - 228;  //b
            temp[1] = src_y[Y] - ((1415 * src_u[U])>>12) - ((2936 * src_v[V])>>12) + 136;  //g
            temp[2] = src_y[Y] + ((5765 * src_v[V])>>12) - 180;  //r

            dst_RGB[3*Y] = (temp[0]<0? 0: temp[0]>255? 255: temp[0]);
            dst_RGB[3*Y+1] = (temp[1]<0? 0: temp[1]>255? 255: temp[1]);
            dst_RGB[3*Y+2] = (temp[2]<0? 0: temp[2]>255? 255: temp[2]);
        }
    }

    return 0;
}

int YUVToBMP(const char *bmp_path,char *yuv_data,ConverFunc func,int width,int height)
{
    unsigned char *rgb_24 = NULL;
    FILE *fp = NULL;

    BITMAPFILEHEADER BmpFileHeader;
    BITMAPINFOHEADER BmpInfoHeader;

    if(bmp_path == NULL || yuv_data == NULL || func == NULL || width <= 0 || height <= 0)
    {
        printf(" YUVToBMP incorrect input parameter!\n");
        return -1;
    }

   /* Fill header information */
   BmpFileHeader.bfType = 0x4d42;
   BmpFileHeader.bfSize = width*height*3 + sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);
   BmpFileHeader.bfReserved1 = 0;
   BmpFileHeader.bfReserved2 = 0;
   BmpFileHeader.bfOffBits = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

   BmpInfoHeader.biSize = sizeof(BmpInfoHeader);
   BmpInfoHeader.biWidth = width;
   BmpInfoHeader.biHeight = -height;
   BmpInfoHeader.biPlanes = 0x01;
   BmpInfoHeader.biBitCount = 24;
   BmpInfoHeader.biCompression = 0;
   BmpInfoHeader.biSizeImage = 0;
   //BmpInfoHeader.biXPelsPerMeter = 0;
   //BmpInfoHeader.biYPelsPerMeter = 0;
   BmpInfoHeader.biClrUsed = 0;
   BmpInfoHeader.biClrImportant = 0;

    rgb_24 = (unsigned char *)malloc(width*height*3);
    if(rgb_24 == NULL)
    {
       printf(" YUVToBMP alloc failed!\n");
       return -1;
    }

    func(rgb_24,yuv_data,width,height);

    /* Create bmp file */
    fp = fopen(bmp_path,"wb+");

    if(!fp)
    {
        free(rgb_24);
        printf(" Create bmp file:%s faled!\n", bmp_path);
        return -1;
    }

    fwrite(&BmpFileHeader,sizeof(BmpFileHeader),1,fp);

    fwrite(&BmpInfoHeader,sizeof(BmpInfoHeader),1,fp);

    fwrite(rgb_24,width*height*3,1,fp);

    free(rgb_24);

    fclose(fp);

    return 0;
}

static void put_jpeg_yuv420p_file(FILE *fp, unsigned char *image, int width,
int height, int quality)
{
	int i, j;

	JSAMPROW y[16],cb[16],cr[16]; // y[2][5] = color sample of row 2 and pixel column 5; (one plane)
	JSAMPARRAY data[3]; // t[0][2][5] = color sample 0 of row 2 and column 5

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	data[0] = y;
	data[1] = cb;
	data[2] = cr;

	cinfo.err = jpeg_std_error(&jerr); // Errors get written to stderr

	jpeg_create_compress(&cinfo);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	jpeg_set_defaults(&cinfo);

	jpeg_set_colorspace(&cinfo, JCS_YCbCr);

	cinfo.raw_data_in = TRUE; // Supply downsampled data
	#if JPEG_LIB_VERSION >= 70
	#warning using JPEG_LIB_VERSION >= 70
	cinfo.do_fancy_downsampling = FALSE; // Fix segfault with v7
	#endif
	cinfo.comp_info[0].h_samp_factor = 2;
	cinfo.comp_info[0].v_samp_factor = 2;
	cinfo.comp_info[1].h_samp_factor = 1;
	cinfo.comp_info[1].v_samp_factor = 1;
	cinfo.comp_info[2].h_samp_factor = 1;
	cinfo.comp_info[2].v_samp_factor = 1;

	jpeg_set_quality(&cinfo, quality, TRUE);
	cinfo.dct_method = JDCT_FASTEST;

	jpeg_stdio_dest(&cinfo, fp); // Data written to file
	jpeg_start_compress(&cinfo, TRUE);

	for (j = 0; j < height; j += 16) {
	for (i = 0; i < 16; i++) {
	y[i] = image + width * (i + j);
	if (i % 2 == 0) {
	cb[i / 2] = image + width * height + width / 2 * ((i + j) /
	2);
	cr[i / 2] = image + width * height + width * height / 4 +
	width / 2 * ((i + j) / 2);
	}
	}
	jpeg_write_raw_data(&cinfo, data, 16);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	}

void debug_capture(unsigned char *rawimg, unsigned int len){

if(snapshot_ref.capture == 1){

		char bmp_data_path[50];
		sprintf(bmp_data_path, "%s/preview.bmp", "/tmp");
		logw(" >>>>> Saving snapshot: %s", bmp_data_path);

//		write(1,rawimg,len);

//		YUVToBMP(bmp_data_path, rawimg, YUV420ToRGB24, res_w, res_h);

		FILE *fp =  fopen("/tmp/out.jpg","wb+");
		put_jpeg_yuv420p_file(fp,rawimg,res_w,res_h,90);
		fclose(fp);

		snapshot_ref.count++;
		snapshot_ref.capture = 0;
		pthread_cond_signal(&snapshot_ref.ready);

}

}

void capture_loop(){

	struct video_input_frame frame = {{{0}, {0}}};

	logv("capture start");

	/*
	 * To get frame by loop - just one
	 */

		__LOG_TIME_START();

		memset(&frame, 0x00, sizeof(frame));

		while (keepCapturing == 1)
		{
			/* to get frame */
			int ret = ak_vi_get_frame(vi_handle, &frame);
			if (!ret) {

				//ak_print_normal("[%d] main chn yuv len: %u\n", count,frame.vi_frame[VIDEO_CHN_MAIN].len);
				//ak_print_normal("[%d] sub  chn yuv len: %u\n\n", count,frame.vi_frame[VIDEO_CHN_SUB].len);

				unsigned int len = frame.vi_frame[VIDEO_CHN_MAIN].len;
				unsigned char *buf = frame.vi_frame[VIDEO_CHN_MAIN].data;

				debug_capture(buf, len);

				// qrcode_decode(buf, len);

				// release frame data
				ak_vi_release_frame(vi_handle, &frame);

			} else {
						// not ready， sleep to release CPU
				ak_sleep_ms(10);
			}

			//small sleep
			ak_sleep_ms(25);
		}
		__LOG_TIME_END("capture");


	stop_capture();
}

int main(int argc, char *argv[]) {

	signal(SIGINT, SIGINT_handler);

	logi("capture_init...");
	int status = capture_init();

	start_server(3000, SNAPSHOT_DEFAULT_DIR, &snapshot_ref);

	if(status){
		logi("loop...");
		capture_loop();
	}

	return 0;
}
