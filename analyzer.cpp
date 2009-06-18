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

Analyzer::Analyzer( kb_video_driver_t *driver, uint width, uint height, uint depth, uint quadrant_size, double threshold ){
	m_driver        = driver;
	m_masterframe   = driver->create( width, height, depth );	
	m_quadrant_size = quadrant_size;
	m_threshold	    = threshold;
	m_quadrants     = (width * height) / this->m_quadrant_size;
}

Analyzer::~Analyzer(){
	free( m_masterframe->data );
	free( m_masterframe );
}

double Analyzer::update( kb_video_buffer_t *frame ){
	int        x, y,           // main coordinates 
			   xt, yt, xb, yb, // top-x, top-y, bottom-x and bottom-x of a quadrant
			   xq, yq,         // quadrants coordinates
			   count = 0;      // relevant quadrants counter
	
	kb_rgb_t   mpixel,         // master pixel
			   fpixel;         // frame pixel
	
	double     rd, gd, bd,     // red, green and blue delta values
		 	   qdelta;		   // quadrant delta value
	
	for( x = 0; x < frame->width; x += m_quadrant_size ){
		for( y = 0; y < frame->height; y += m_quadrant_size ){
			// compute quadrants coordinates
			xt = x;
			yt = y;
			xb = x + m_quadrant_size;
			yb = y + m_quadrant_size;
			// compute delta value between frame and m_masterframe (the previous frame)
			qdelta = 0;
			for( xq = xt; xq < xb; xq++ ){
				for( yq = yt; yq < yb; yq++ ){
					// acquire pixels
					m_driver->pixel( m_masterframe, xq, yq, &mpixel );	
					m_driver->pixel( frame,         xq, yq, &fpixel );
					// compute delta					
					rd = delta( mpixel.red,   fpixel.red );
        			gd = delta( mpixel.green, fpixel.green );
					bd = delta( mpixel.blue,  fpixel.blue );

					qdelta += ((rd + gd + bd) / 3.0f);
				}	
			}
			qdelta /= m_quadrants;
		
			if( qdelta >= m_threshold ){
				count++;
			}
		}
	}
	
	memcpy( m_masterframe->data, frame->data, frame->size );
		
	return (count * 100.0f) / (double)m_quadrants;
}
