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
#include "config.h"

bool Config::config_exists(){
	FILE *fp = fopen( m_config_file, "rt" );
	if( fp ){
		fclose(fp);
		return true;	
	}
	return false;
}

void Config::config_create(){
	FILE *fp = fopen( m_config_file, "w+t" );
	
	fprintf( fp, "# Kerby video surveillance system configuration file .\n\n"
				
				 "# DEVICE defines the video device to use .\n"
				 "DEVICE = /dev/video0\n\n"
				
				 "# QUADRANTSIZE defines the minimum size of the quadrants to analyze in the video stream .\n"
				 "# It has to be a divisor fo the acquiring width and height .\n"
				 "QUADRANTSIZE = 8\n\n"
				 
				 "# THRESHOLD defines the threshold value to use for motion detection .\n"
				 "THRESHOLD = 0.2\n\n"
				 
				 "# RECORDPATH defines the path to save videos into .\n"
				 "RECORDPATH = /media/pendrive/kerby/\n" );
	
	fclose(fp);
}

void Config::trim(char *str){
	// Trim spaces and tabs from beginning:
	int i=0,j;
	while((str[i]==' ')||(str[i]=='\t')||(str[i]=='\n')) {
		i++;
	}
	if(i>0) {
		for(j=0;j<strlen(str);j++) {
			str[j]=str[j+i];
		}
		str[j]='\0';
	}

	// Trim spaces and tabs from end:
	i = strlen(str)-1;
	while((str[i]==' ')||(str[i]=='\t')||(str[i]=='\n')) {
		i--;
	}
	if(i<(strlen(str)-1)) {
		str[i+1]='\0';
	}
}


Config::Config(){
	sprintf( m_config_file, "%s/.kerbyrc", getenv("HOME") );
	
	if( config_exists() == false ){
		config_create();
	}	
	
	char line[0xFF]  = {0},
		 key[0xFF]   = {0},
		 value[0xFF] = {0};
	FILE *fp         = fopen( m_config_file, "rt" );
	int  i, j;

	while( fgets( line, 0xFF, fp ) != NULL ){
		trim(line);
		// skip comments and empty lines
		if( line[0] != '#' && line[0] != 0x00 ){
			j = i = 0;
			// parse key
			while( line[i] != ' ' && line[i] != '=' && line[i] != '\t' ){
				key[i] = line[i];
				i++;	
			}
			
			// skip white spaces and '='
			while( line[i] == ' ' || line[i] == '=' || line[i] == '\t' ){
				i++;
			}
			
			// parse value
			for( j = 0; j < 0xFF; j++, i++ ){
				value[j] = line[i];	
			}
			
			m_hash.insert( hash_pair_t( key, value ) );
			
			#ifdef DEBUG 
				printf( "KEY : '%s'\nVAL : '%s'\n", key, value );
			#endif
		}
		
		memset( line,  0x00, 0xFF );
		memset( key,   0x00, 0xFF );
		memset( value, 0x00, 0xFF );
	}
	
	fclose(fp);
}

Config::~Config(){
	m_hash.clear();
}

void Config::dump(){
	hash_iterator_t i;
	printf( "-- Kerby Configuration --\n" );
	for( i = m_hash.begin(); i != m_hash.end(); i++ ){
		printf( "\t%s = %s\n", i->first.c_str(), i->second.c_str() );
	}
	printf( "-------------------------\n" );
}

char *Config::get( const char *key ){
	hash_iterator_t i = m_hash.find(key);
	return (char *)( i == m_hash.end() ? "<undefined>" : i->second.c_str() );
}

int Config::get_int( const char *key ){
	char *value = get(key);
	return (strcmp( value, "<undefined>" ) == 0 ? -1 : atoi(value));
}

double Config::get_double( const char *key ){
	char *value = get(key);	
	return (strcmp( value, "<undefined>" ) == 0 ? 0.0f : atof(value));
}
