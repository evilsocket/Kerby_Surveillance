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

class Analyzer
{
	private :
		kb_video_driver_t *m_driver;	
			
		double		       m_threshold;
		
		uint	           m_quadrant_size;		
		uint               m_quadrants;
		
		kb_video_buffer_t *m_masterframe;
		
		inline double delta( short total, short value ){
			double a = total,
				   b = value;
			if( total < value ){
				a = value;
				b = total;
			}
			return (b > 0 ? (((a - b) / b) * 100.0f) : 0);
		}
					
	public  :
	
		Analyzer( kb_video_driver_t *driver, uint width, uint height, uint depth, uint quadrant_size, double threshold );
	   ~Analyzer();
	   
	   double update( kb_video_buffer_t *frame );
};



#endif

