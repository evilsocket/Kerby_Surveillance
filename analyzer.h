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
#ifndef __analyzer_h__
#	define __analyzer_h__

#include "driver.h"
#include <vector>

#include <CImg.h>

using std::vector;
using namespace cimg_library; 

typedef struct _kb_quadrant{
	int xt;
	int yt;
	int xb;
	int yb;
	
	double delta;
	
	_kb_quadrant( int xt, int yt, int xb, int yb, double delta ){
		this->xt = xt;
		this->yt = yt;
		this->xb = xb;
		this->yb = yb;
		this->delta = delta;
	}	
}
kb_quadrant_t;

typedef vector<kb_quadrant_t *> kb_delta_quadrants_t;

class Analyzer
{
	private :
	
		double		 m_threshold;
		
		uint	     m_quadrant_size;		
	
		CImg<byte> * m_masterframe;
		CImg<byte> * m_frame;
		
		void convert( kb_video_buffer_t *frame, CImg<byte> *image );
		void convert( kb_video_buffer_t *frame );
		
		CImg<byte> quadrant( CImg<byte> *image, int xt, int yt, int xb, int yb );
		CImg<byte> quadrant( int xt, int yt, int xb, int yb );
		
		double delta( short total, short value );
		double delta( CImg<byte> *quadrant, int xt, int yt, int xb, int yb );
				
	public  :
	
		Analyzer( uint width, uint height, uint depth, uint quadrant_size, double threshold );
	   ~Analyzer();
	   
	   kb_delta_quadrants_t * update( kb_video_buffer_t *frame );
	   
	   CImg<byte> * frame();
	   CImg<byte> * masterframe();
};



#endif

