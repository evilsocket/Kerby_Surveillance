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
#ifndef __config_h__
#	define __config_h__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>

using std::map;
using std::string;
using std::pair;

typedef map < string, string > 			 hash_t;
typedef pair< string, string > 			 hash_pair_t;
typedef map < string, string >::iterator hash_iterator_t;

class Config
{
	private :
	
		char   m_config_file[0xFF];
		
		hash_t m_hash;
		
		bool config_exists();
		void config_create();
		
		void trim( char *str );
					
	public  :
	
		Config();
	   ~Config();
	   
	   void dump();
	   
	   char *get( const char *key );
	   
	   int    get_int( const char *key );
	   double get_double( const char *key );
};

#endif


