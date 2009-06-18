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
#include "analyzer.h"

// TODO: Those values should be read from a configuration file .
#define QUADRANTSIZE 8
#define THRESHOLD	 0.2f
#define RECORDPATH   "/media/pendrive/kerbyd"

#define DEBUG        1

void usage( char *app ){
	printf( "\nUsage %s -d <device> -t <threshold> -q <quadrantsize> -a <alarm percentage>\n\n", app );	
}

int main( int argc, char *argv[] ){
	kb_video_driver_t  *v4l = (kb_video_driver_t *)&kb_video_drivers[0]; 
	kb_device_t         device;
	kb_video_buffer_t   frame;
	Analyzer           *analyzer;
	
	if( v4l->open( argv[1], &device ) < 0 ){
		exit(-1);	
	}

	analyzer = new Analyzer( v4l, device.width, device.height, 3, QUADRANTSIZE, THRESHOLD );
	
	#ifdef DEBUG 
		printf( "Device :\n"
				"\tfile descriptor : %d\n"
				"\tmemory map      : 0x%X\n"
				"\tdevice          : %s\n"
				"\tname            : %s\n"
				"\tmax width       : %d\n"
				"\tmax height      : %d\n"
				"\tdepth           : %d\n",
				device.fd,
				device.mmap,
				device.device,
				device.name,
				device.width,
				device.height,
				device.depth );
	#endif
	
	double delta;
	
	while( 1 ){
		v4l->capture( &device, &frame );

		delta = analyzer->update( &frame );
		
		if( delta > 0 ) printf( "DELTA : %f\n", delta );
	}

	v4l->close( &device );
	
	delete analyzer;
	
	return 0;	
}

