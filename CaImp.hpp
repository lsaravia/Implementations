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

#ifndef __CAImp_HPP
#define __CAImp_HPP

#include "cabase.hpp"

//
//	CAImp : CA Discreto.
//
class CellEvent : public Cell
	{
	public:
		char event;
	CellEvent() : Cell() { event =0;};
	CellEvent & operator=(const CellEvent &src){ NumSpecie[0] = src.NumSpecie[0];  NumSpecie[1] = src.NumSpecie[1]; event=src.event; return *this;};
	CellEvent(const CellEvent &src){ NumSpecie[0] = src.NumSpecie[0];  NumSpecie[1] = src.NumSpecie[1]; event=src.event;};
	};



class CAImp : public CABase
	{
	protected:
	char EvalType,TimeType;
	int EventType;
	void JumbleXY(CellEvent * pos, int max);
	void EvalCellA2(int &x,int &y);
	void EvalCellA3(int &x,int &y);
	void EvalCellB1(int &x,int &y);
	void EvalCellB2(int &x,int &y);
	void EvalCellB3(int &x,int &y);

	public:

	CAImp( unsigned numSp, int dimX, int dimY, int rr=0 ) : CABase(numSp,dimX,dimY,rr) {};
	CAImp() : CABase() {};
	~CAImp() {};

	void Colonize(int xx,int yy);
	int  ReadParms( int &nEval, int &nRuns, char * file );
	
	void Evaluate();
	void EvalCell(int x,int y);
	void SetN( unsigned n ){ N= n;};
	void Growth(int x,int y);
	int  Survival(int x,int y);

	void PrintMap(unsigned O);
	void PrintMem();

	};


#endif
