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

#define QUADRANTSIZE 8
#define THRESHOLD	 5.0f

void freequads( kb_delta_quadrants_t * quads ){
	kb_delta_quadrants_t::iterator i;
	for( i = quads->begin(); i != quads->end(); i++ ){
		delete *i;	
	}
	delete quads;
}

void usage( char *app ){
	printf( "\nUsage %s -d <device> -t <threshold> -q <quadrantsize> -a <alarm percentage>\n\n", app );	
}

int main( int argc, char *argv[] ){
	kb_video_driver_t * v4l = (kb_video_driver_t *)&kb_video_drivers[0]; 
	kb_device_t         dev;
	kb_video_buffer_t   frame;
	Analyzer		    analyzer( 320, 240, 3, QUADRANTSIZE, THRESHOLD );
	
	v4l->open( argv[1], &dev );

	printf( "Device :\n"
			"\tfile descriptor : %d\n"
			"\tmemory map      : 0x%X\n"
			"\tdevice          : %s\n"
			"\tname            : %s\n"
			"\tmax width       : %d\n"
			"\tmax height      : %d\n"
			"\tdepth           : %d\n",
			dev.fd,
			dev.mmap,
			dev.device,
			dev.name,
			dev.width,
			dev.height,
			dev.depth );

	CImg<byte>  iframe( 320, 240, 1, 3 );
				
	double delta, total = (320.0f * 240.0f) / QUADRANTSIZE;
	kb_delta_quadrants_t * quads;
	
	do{
		v4l->capture( &dev, &frame );

		quads = analyzer.update( &frame );
		iframe.swap( *analyzer.frame() );
		
		delta = (quads->size() * 100.0f) / total;
		
		printf( "DELTA : %f%%\n", delta );
		
		freequads(quads);
	}
	while( 1 );
	

	v4l->close( &dev );
	
	return 0;	
}

