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
#include "avi.h"

AVI::AVI( char *filename, uint width, uint height, uint fps /*= 5*/ ){
	try{
		memset( &m_bmpinfo, 0, sizeof(m_bmpinfo) );
		m_bmpinfo.biSize        = sizeof(m_bmpinfo);
		m_bmpinfo.biWidth       = width;
		m_bmpinfo.biHeight      = height;
		m_bmpinfo.biSizeImage   = m_bmpinfo.biWidth * m_bmpinfo.biHeight * 3;
		m_bmpinfo.biPlanes      = 1;
		m_bmpinfo.biBitCount    = 24;	
		m_bmpinfo.biCompression = BI_RGB;

		m_fps   = fps;
		// TODO: Handle avi compression, uncompressed format it's just too *hard-disk-expensive* !
		m_codec = fccYV12;
		
		m_file_writer   = avm::CreateWriteFile(filename);
		m_stream_writer = m_file_writer->AddVideoStream( m_codec, &m_bmpinfo, 1000000 / m_fps );
	}
	catch(FatalError& error) {
    	error.Print();
	} 	 
}

AVI::~AVI(){
	this->stop();
	delete m_file_writer;	
}

void AVI::start(){
	try{
		m_stream_writer->Start();	
	}
	catch(FatalError& error) {
    	error.Print();
	} 	
}

void AVI::addFrame( kb_video_buffer_t *buffer ){
	try{
		avm::CImage frame( buffer->data, buffer->width, buffer->height );
    	m_stream_writer->AddFrame(&frame);	
	}
	catch(FatalError& error) {
    	error.Print();
	} 	
}

void AVI::stop(){
	try{
		m_stream_writer->Stop();	
	}
	catch(FatalError& error) {
    	error.Print();
	} 	
}
