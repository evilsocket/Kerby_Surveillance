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

void Analyzer::convert( kb_video_buffer_t *frame, CImg<byte> *image ){
	int x,y;
	
	for( x = 0; x < frame->width; x++ ){
		for( y = 0; y < frame->height; y++ ){
			*image->ptr( x, y, 0 ) = frame->data[ (y * frame->width + x) * frame->depth + 0 ];	
			*image->ptr( x, y, 1 ) = frame->data[ (y * frame->width + x) * frame->depth + 1 ];	
			*image->ptr( x, y, 2 ) = frame->data[ (y * frame->width + x) * frame->depth + 2 ];	
		}	
	}	
}

void Analyzer::convert( kb_video_buffer_t* frame ){
	this->convert( frame, this->m_masterframe );
}

CImg<byte> Analyzer::quadrant( CImg<byte> *image, int xt, int yt, int xb, int yb ){
	CImg<byte> quad( xb - xt, yb - yt, 1, 3 );	
	
	int xq,yq,xi,yi;
	
	for( xi = xt, xq = 0; xi < xb; xi++, xq++ ){
    	for( yi = yt, yq = 0; yi < yb; yi++, yq++ ){
			*quad.ptr( xq, yq, 0 ) = *image->ptr( xi, yi, 0 );	
			*quad.ptr( xq, yq, 1 ) = *image->ptr( xi, yi, 1 );	
			*quad.ptr( xq, yq, 2 ) = *image->ptr( xi, yi, 2 );	
		}
	}
	
	return quad;
}

CImg<byte> Analyzer::quadrant( int xt, int yt, int xb, int yb ){
	return this->quadrant( this->m_masterframe, xt, yt, xb, yb );
}

double Analyzer::delta( short total, short value ){
	double a = total,
           b = value;

	if( total < value ){
		a = value;
		b = total;
	}

    return ((a - b) / b) * 100.0f;
}

double Analyzer::delta( CImg<byte> *quadrant, int xt, int yt, int xb, int yb ){
	CImg<byte> masterquadrant = this->quadrant( xt, yt, xb, yb );
    double     delta = 0.0f;

    int h = yb - yt,
        w = xb - xt,
        y, x;
    
    double rdelta,gdelta,bdelta;

	for( y = 0; y < h; y++ ){
		for( x = 0; x < w; x++ ){
			rdelta = this->delta( *masterquadrant.ptr( x, y, 0 ), *quadrant->ptr( x, y, 0 ) );
        	gdelta = this->delta( *masterquadrant.ptr( x, y, 1 ), *quadrant->ptr( x, y, 1 ) );
			bdelta = this->delta( *masterquadrant.ptr( x, y, 2 ), *quadrant->ptr( x, y, 2 ) );
        	
        	delta += ((rdelta + gdelta + bdelta) / 3.0f);
		}
	}

	return (delta / (w * h));
}

Analyzer::Analyzer( uint width, uint height, uint depth, uint quadrant_size, double threshold ){
	this->m_masterframe   = new CImg<byte>( width, height, 1, depth );	
	this->m_frame		  = new CImg<byte>( width, height, 1, depth );	
	this->m_quadrant_size = quadrant_size;
	this->m_threshold	  = threshold;
	this->m_quadrants     = (width * height) / this->m_quadrant_size;
}

Analyzer::~Analyzer(){
	delete this->m_masterframe;
	delete this->m_frame;
}

double Analyzer::update( kb_video_buffer_t *frame ){
	CImg<byte> iframe( frame->width, frame->height, 1, frame->depth ), 
			   quadrant;
	int        x, y, xt, yt, xb, yb, count = 0;
	double 	   qdelta;
		
	this->convert( frame, &iframe );
	
	this->m_frame->swap(iframe);
	
	for( x = 0; x < iframe.width; x += this->m_quadrant_size ){
		for( y = 0; y < iframe.height; y += this->m_quadrant_size ){
			xt = x;
			yt = y;
			xb = x + this->m_quadrant_size;
			yb = y + this->m_quadrant_size;
			
			quadrant = this->quadrant( &iframe, xt, yt, xb, yb );
			
			qdelta = this->delta( &quadrant, xt, yt, xb, yb );
			
			if( qdelta >= this->m_threshold ){
				count++;
			}
		}
	}
	
	this->m_masterframe->swap(iframe);
	
	
	return (count * 100.0f) / (double)this->m_quadrants;
}
