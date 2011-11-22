/*  Copyright 2009 Leonardo A. Saravia
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

#ifndef __CELL_HPP
#define __CELL_HPP

#ifdef __GNUC__
#include "fortify.h"
#endif

class Cell
	{
	protected:
	int NumSpecie[2];
	public:
	Cell() { NumSpecie[0]=0;  NumSpecie[1]=0;};
	int & Elem( int idx ) { return NumSpecie[idx]; };
	const Cell & Elem( int nsp0, int nsp1 ) { NumSpecie[0]=nsp0;NumSpecie[1]=nsp1; return *this; };
	Cell & operator=(const Cell &src){ NumSpecie[0] = src.NumSpecie[0];  NumSpecie[1] = src.NumSpecie[1]; return *this;};
	Cell(const Cell &src){ NumSpecie[0] = src.NumSpecie[0];  NumSpecie[1] = src.NumSpecie[1];};
	void Transfer(int nsp0, int nsp1) {NumSpecie[nsp0]=NumSpecie[nsp1];};
	};


#endif //__CELL_HPP
