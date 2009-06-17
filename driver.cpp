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
#include "driver.h"

int v4l_open( char *devname, kb_device_t * dev ){
	struct video_capability vcap;
	struct video_channel    vchan;
	
	if( (dev->fd = open( devname, O_RDWR )) <= 0 ){
		perror("open");
		return -1;	
	}
	
	strcpy( dev->device, devname );
	
	if( ioctl( dev->fd, VIDIOCGCAP, &vcap ) < 0 ){
		perror("VIDIOCGCAP");
		return -1;	
  	}
  	
  	strcpy( dev->name, vcap.name );
  	
  	dev->width  = vcap.maxwidth;
  	dev->height = vcap.maxheight;
  	dev->depth  = 3;
  	
  	vchan.channel = 1; 
  	if( ioctl( dev->fd, VIDIOCGCHAN, &vchan ) < 0 ){
    	perror("VIDIOCGCHAN");
    	return -1;	
    }
    vchan.norm = 1;
    if( ioctl( dev->fd, VIDIOCSCHAN, &vchan ) < 0 ){
    	perror("VIDIOCSCHAN");
    	return -1;	
    }
    
	dev->mmap = (byte *)mmap( 0, dev->width * dev->height * dev->depth, PROT_READ|PROT_WRITE, MAP_SHARED, dev->fd, 0 );	
	  	
  	return 1;
}

int v4l_capture( kb_device_t * dev, kb_video_buffer_t * vbuffer ){
	struct video_mmap vmap;
	
	vmap.width     = vbuffer->width  = dev->width;
	vmap.height    = vbuffer->height = dev->height;
	vmap.frame     = 0;
	vmap.format    = VIDEO_PALETTE_RGB24;
	vbuffer->depth = dev->depth;
	vbuffer->size  = vmap.width * vmap.height * vbuffer->depth;

	if( ioctl( dev->fd, VIDIOCMCAPTURE, &vmap ) < 0 ){
    	perror("VIDIOCMCAPTURE");
    	return -1;
  	}
  	
	if( ioctl( dev->fd, VIDIOCSYNC, &vmap.frame ) < 0 ){
		perror("VIDIOCSYNC");
		return -1;
	}

	vbuffer->data = dev->mmap;
	
	return 1;
}

void v4l_close( kb_device_t * dev ){
	munmap( dev->mmap, dev->width * dev->height * dev->depth );
	close( dev->fd);
}

void v4l_pixel( kb_video_buffer_t * vbuffer, uint x, uint y, kb_rgb_t *pixel ){
	uint offset = (y * vbuffer->width + x) * vbuffer->depth;
	
	pixel->red   = vbuffer->data[ offset + 0 ];
	pixel->green = vbuffer->data[ offset + 1 ];
	pixel->blue  = vbuffer->data[ offset + 2 ];
}

void v4l_swap( kb_video_buffer_t * src, kb_video_buffer_t * dst ){
	memcpy( dst->data, src->data, dst->size );
}

kb_video_buffer_t * v4l_create( uint width, uint height, uint depth ){
	kb_video_buffer_t *buffer = (kb_video_buffer_t *)malloc(sizeof(kb_video_buffer_t));
	
	buffer->width  = width;
	buffer->height = height;
	buffer->depth  = depth;
	buffer->size   = width * height * depth;
	buffer->data   = (byte *)malloc( buffer->size );
	
	return buffer;
}
