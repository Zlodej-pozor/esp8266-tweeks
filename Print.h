/*
 Print.h - Base class that provides print() and println()
 Copyright (c) 2008 David A. Mellis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef Print_h
#define Print_h

#include <stdint.h>
#include <stddef.h>

#include "WString.h"
#include "Printable.h"

#include "stdlib_noniso.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class Print {
    private:
        int write_error;
        size_t printNumber(unsigned long, uint8_t);
        size_t printFloat(double, uint8_t);
    protected:
        void setWriteError(int err = 1) {
            write_error = err;
        }
    public:
        Print() :
                write_error(0) {
        }

        int getWriteError() {
            return write_error;
        }
        void clearWriteError() {
            setWriteError(0);
        }

        virtual size_t write(uint8_t) = 0;
        size_t write(const char *str) {
            if(str == NULL)
                return 0;
            return write((const uint8_t *) str, strlen_P(str));
        }
        virtual size_t write(const uint8_t *buffer, size_t size);
        size_t write(const char *buffer, size_t size) {
            return write((const uint8_t *) buffer, size);
        }
        // These handle ambiguity for write(0) case, because (0) can be a pointer or an integer
        inline size_t write(short t) { return write((uint8_t)t); }
        inline size_t write(unsigned short t) { return write((uint8_t)t); }
        inline size_t write(int t) { return write((uint8_t)t); }
        inline size_t write(unsigned int t) { return write((uint8_t)t); }
        inline size_t write(long t) { return write((uint8_t)t); }
        inline size_t write(unsigned long t) { return write((uint8_t)t); }
        // Enable write(char) to fall through to write(uint8_t)
        inline size_t write(char c) { return write((uint8_t) c); }
        inline size_t write(int8_t c) { return write((uint8_t) c); }

        size_t printf(const char * format, ...)  __attribute__ ((format (printf, 2, 3)));
        size_t printf_P(PGM_P format, ...) __attribute__((format(printf, 2, 3)));
        size_t print(const __FlashStringHelper *);
        size_t print(const String &);
        size_t print(const char[]);
        size_t print(char);
        size_t print(unsigned char, int = DEC);
        size_t print(int, int = DEC);
        size_t print(unsigned int, int = DEC);
        size_t print(long, int = DEC);
        size_t print(unsigned long, int = DEC);
        size_t print(double, int = 2);
        size_t print(const Printable&);

        size_t println(const __FlashStringHelper *);
        size_t println(const String &s);
        size_t println(const char[]);
        size_t println(char);
        size_t println(unsigned char, int = DEC);
        size_t println(int, int = DEC);
        size_t println(unsigned int, int = DEC);
        size_t println(long, int = DEC);
        size_t println(unsigned long, int = DEC);
        size_t println(double, int = 2);
        size_t println(const Printable&);
        size_t println(void);

        virtual void flush() { /* Empty implementation for backward compatibility */ }


//  This section is here only untill ESP arduino developement creates proper equivalent
//	They are here instead of cpp file to make reduce number of edited files
        #include <stdarg.h>
#define PRINTF_BUF 80 // define the tmp buffer size (change if desired)
#define MAXPRINTFFINPUTS 20 // Maximum number of variables printf takes with flash string helper
   size_t printf(const __FlashStringHelper *format, ...){
	   char vystup[PRINTF_BUF] __attribute__ ((aligned(4)));
	   PGM_P p = reinterpret_cast<PGM_P>(format);
	    auto len = strlen_P(p); 
	    //printf("Dĺžka: %d\n",len);
	   size_t odoslane=0;
	   char vstup;
	   uint8_t vstupov=0;
	   va_list vl;
		va_start(vl,vystup);
		//print("Tlačím na výstup");
	   for(uint8_t n=0;n<len;n++){
		   //printf("%d.",n);
		   vstup=pgm_read_byte(p+n);
		   // println(vstup);
		   if(vstup=='%'){		// If I get variable formating
			   //println(".");
			   vystup[vstupov++]=vstup;
			   n++;
			   vstup=pgm_read_byte(p+n);
			   bool citam=1;
			   while(citam==1){
				   print(".");
				   switch(vstup){	// These need to be each handled properly to deal with data > 8bit
					   case 'd':
					   case 'i':
					   case 'u':
					   case 'o':
					   case 'x':
					   case 'X':
					   case 'f':
					   case 'F':
					   case 'e':
					   case 'E':
					   case 'g':
					   case 'G':
					   case 'a':
					   case 'A':
					   case 'c':
					   case 's':
					   case 'p':
					   case 'n':
							vystup[vstupov++]=vstup;
							citam=0;
							//println("Rozhodnute");
							break;
						case 0:
							return odoslane;
							break;
						default:
							vystup[vstupov]=vstup;
							vstupov++;
							vstup=pgm_read_byte(p+n);
							n++;
							if(vstupov>=len) citam=0;
							break;
					}
				}
				// print("Posielam %% :");
				auto premenna=va_arg(vl,int); //needs to  be more flexible as variable might be in different format
				//printf("\nHodnota premennej :\t%d\n", premenna);
				odoslane+=printf(vystup,premenna);
				for(uint8_t i=0; i<vstupov; i++) vystup[i]=0;
				vstupov=0;
				//print("\nVystup po %% :");
			} else {
				vystup[vstupov]=vstup;
				vstupov++;
				//print("-");
			}
		}
		print(vystup);
		//println("-------------------------------");
		return len; //counts wrong;
	}
				
	/*void printf(const __FlashStringHelper *format){ //maybe somehow replace for print directly
		char buff[PRINTF_BUF];
		PGM_P p=reinterpret_cast<PGM_P>(format);
		uint8_t i=0;
		char vstup='x';
		while(vstup!=NULL && i<PRINTF_BUF) { // the hole while loop can be replaced with strlen_P when it gets fixed to take FSH rather then *char
			vstup=pgm_read_byte(p++);
			i++;
		}
		memcpy_P(buff,format,i);
		print(buff);
	}*/
	
	/*void printf(const __FlashStringHelper *format, ...){ // Not capable of anything bigger then uint8 and not able to escape \% properly
		char buff[PRINTF_BUF];
		PGM_P p=reinterpret_cast<PGM_P>(format);
		uint8_t i=0;
		uint8_t premene[MAXPRINTFFINPUTS]={};
		bool presun=0;
		uint8_t premenych=0;
		va_list vl;
		va_start(vl,buff);
		for(i=0;i<PRINTF_BUF && buff!=0; i++) { // since I am counting it I may as well copy it a a same time and check if a variable
			buff[i]=pgm_read_byte(p++);
			switch(buff[i]){
				case '%':
					if(!presun) {
						 if(premenych<20) premene[premenych]=va_arg(vl,uint8_t);
						 premenych++;
						 }
					break;
				case '\\':
					presun=1;
					break;
				default:
					presun=0;
					break;
			}
		}
		if(premenych>MAXPRINTFFINPUTS) premenych=MAXPRINTFFINPUTS;
		// And now the realy dirthy part
		switch(premenych){
		case 0:{
			printf(buff);
			} break;
		case 1:{
			printf(buff,premene[0]);
			} break;
		case 2:{
			printf(buff,premene[0],premene[1]);
			} break;
		case 3:{
			printf(buff,premene[0],premene[1],premene[2]);
			} break;
		case 4:{
			printf(buff,premene[0],premene[1],premene[2],premene[3]);
			} break;
		case 5:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4]);
			} break;
		case 6:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5]);
			} break;
		case 7:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6]);
			} break;
		case 8:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7]);
			} break;
		case 9:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8]);
			} break;
		case 10:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9]);
			} break;
		case 11:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10]);
			} break;
		case 12:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10],premene[11]);
			} break;
		case 13:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10],premene[11],premene[12]);
			} break;
		case 14:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10],premene[11],premene[12],premene[13]);
			} break;
		case 15:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10],premene[11],premene[12],premene[13],premene[14]);
			} break;
		case 16:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10],premene[11],premene[12],premene[13],premene[14],premene[15]);
			} break;
		case 17:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10],premene[11],premene[12],premene[13],premene[14],premene[15],premene[16]);
			} break;
		case 18:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10],premene[11],premene[12],premene[13],premene[14],premene[15],premene[16],premene[17]);
			} break;
		case 19:{
			printf(buff,premene[0],premene[1],premene[2],premene[3],premene[4],premene[5],premene[6],premene[7],premene[8],premene[9],premene[10],premene[11],premene[12],premene[13],premene[14],premene[15],premene[16],premene[17],premene[18]);
			} break;
	}
		
	}*/
};

#endif
