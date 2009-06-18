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
#define DEBUG 1

#include "analyzer.h"
#include "avi.h"
#include "config.h"

/*
void usage( char *app ){
	printf( "\nUsage %s -d <device> -t <threshold> -q <quadrantsize> -a <alarm percentage>\n\n", app );	
}
*/

int main( int argc, char *argv[] ){
	kb_video_driver_t  *v4l = (kb_video_driver_t *)&kb_video_drivers[0]; 
	kb_device_t         device;
	kb_video_buffer_t   frame;
	Analyzer           *analyzer;
	char                moviename[0xFF] = {0};
	AVI                *movie = NULL;
	Config              config;
	double 				delta;
	
	if( v4l->open( config.get("DEVICE"), &device ) < 0 ){
		exit(-1);	
	}

	analyzer = new Analyzer( v4l, device.width, device.height, 3, config.get_int("QUADRANTSIZE"), config.get_double("THRESHOLD") );
	
	#ifdef DEBUG 
		config.dump();
		
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

	while( 1 ){
		v4l->capture( &device, &frame );

		delta = analyzer->update( &frame );
		
		//if( delta > 0 ) printf( "DELTA : %f\n", delta );
		if( delta > 0.0f ){
			// create movie and start adding frames
			if( movie == NULL ){
				sprintf( moviename, "%s/%d.avi", config.get("RECORDPATH"), time(NULL) );
				#ifdef DEBUG 
					printf( "@ Creating new movie %s .\n", moviename );
				#endif
				movie = new AVI( moviename, device.width, device.height );
				movie->start();
			}
			// continue with previously created movie	
			#ifdef DEBUG 
				printf( "@ Adding frame to %s .\n", moviename );
			#endif
			movie->addFrame(&frame);
		}
		else{
			if( movie != NULL ){
				#ifdef DEBUG 
					printf( "@ Closing movie %s .\n", moviename );
				#endif
				delete movie;	
			}	
			movie = NULL;
		}
	}

	v4l->close( &device );
	
	delete analyzer;
	
	return 0;	
}

