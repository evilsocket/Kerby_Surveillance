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
#ifndef __avi_h__
#	define __avi_h__

#include <avifile.h>
#include <aviplay.h>
#include <avm_fourcc.h>
#include <avm_except.h>

#include <unistd.h>	
#include <string.h>
#include <stdlib.h>	
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>

#include "driver.h"

class AVI {
	private :
		
		uint                    m_fps;
		BITMAPINFOHEADER        m_bmpinfo;
		fourcc_t 		        m_codec;
		
		avm::IWriteFile        *m_file_writer;
		avm::IVideoWriteStream *m_stream_writer;
		
	public  :
	
		AVI( char *filename, uint width, uint height, uint fps = 5 );
		~AVI();
		
		void start();
		void addFrame( kb_video_buffer_t *buffer );
		void stop();
};

#endif



