/***************************************************************************
 *   @brief Kerby - Light weight video surveillance system                 *
 *   @author Simone Margaritelli (aka evilsocket) <evilsocket@gmail.com>   *
 *                       		                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef __driver_h__
#	define __driver_h__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <linux/videodev.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef unsigned int  uint;

typedef struct{
	int  fd;
	byte * mmap;
	char device[0xFF];
	char name[0xFF];
	uint width;
	uint height;
	uint depth;
}
kb_device_t;

typedef struct{
	byte * data;
	
	uint   width;
	uint   height;
	uint   depth;
	uint   size;
}
kb_video_buffer_t;

typedef struct{
	byte red;
	byte green;
	byte blue;
}
kb_rgb_t;

typedef int  				(* kb_open_t)	 ( char *devname, kb_device_t * dev );
typedef int  				(* kb_capture_t) ( kb_device_t * dev, kb_video_buffer_t * vbuffer );
typedef void 				(* kb_close_t)	 ( kb_device_t * dev );
typedef void 				(* kb_pixel_t)   ( kb_video_buffer_t * vbuffer, uint x, uint y, kb_rgb_t *pixel );
typedef void 				(* kb_swap_t)    ( kb_video_buffer_t * src, kb_video_buffer_t * dst );
typedef kb_video_buffer_t * (* kb_create_t)  ( uint width, uint height, uint depth );

typedef struct{
	char 		 name[0xFF];
	
	kb_open_t	 open;
	kb_capture_t capture;
	kb_close_t   close;
	kb_pixel_t	 pixel;
	kb_swap_t    swap;
	kb_create_t  create;
}
kb_video_driver_t;

int  				v4l_open    ( char *devname, kb_device_t * dev );
int  			    v4l_capture ( kb_device_t * dev, kb_video_buffer_t * vbuffer );
void 				v4l_close	( kb_device_t * dev );
void 			    v4l_pixel   ( kb_video_buffer_t * vbuffer, uint x, uint y, kb_rgb_t *pixel );
void 			    v4l_swap    ( kb_video_buffer_t * src, kb_video_buffer_t * dst );
kb_video_buffer_t * v4l_create  ( uint width, uint height, uint depth );

static const kb_video_driver_t kb_video_drivers[] = 
{
	{ "Video 4 Linux v1.0", v4l_open, v4l_capture, v4l_close, v4l_pixel, v4l_swap, v4l_create }
};

#define SSVIDEODRIVERSN sizeof(kb_video_drivers) / sizeof(kb_video_drivers[0])

#endif
