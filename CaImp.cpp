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

//	Clase Base : CABase
//
//	Clase Asociada : Specie
//
//


#pragma implementation

#include "CaImp.hpp"
#include "fortify.h"
#include <strstream.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#ifdef __GNUG__
#include <pc.h>
#endif


int CAImp::ReadParms( int &nEval, int &nRuns, char * file )
	{
	ifstream in;
	char buff[255];

	int sp=0;


	in.open(file);
	if( !in )
		{
		cerr << "Cannot open Parms file.\n";
		return 1;
		}

	//fgets(buff,80,in);
	//sscanf(buff,"%i %i",&DimX,&DimY);
	in >> DimX >> DimY;
	
	//fgets(buff,80,in);
	//sscanf(buff,"%i %i",&nEval,&nRuns);
	in >> nEval >> nRuns;
	
	//fgets(buff,80,in);
	//sscanf(buff,"%i",&NumSpecies);
	
	in >> NumSpecies >> EvalType >> TimeType >> EventType;


#ifdef TEST
	Init( NumSpecies, DimX, DimY, 100);
#else
	Init( NumSpecies, DimX, DimY);
#endif


	in.getline(buff,255);

	for(int i=0;i<NumSpecies;i++)
		{
		in.get(buff,254);
		sp = Sp[i].Scan(buff);
		if( sp>=NumSpecies || sp!=i )
			{
			fprintf(stderr, "Error reading Parms file.\n");
			return 1;
			}

		}
	return 0;
	}

void CAImp::Evaluate()
	{
	int x,y,i,max;

	if( TimeType == 'D' )
		{
		A = T % 2;
		N = (T+1) % 2;
		}
	else
		N = 0;


	switch(EvalType)
		{
		case 'E':
			switch( Rand(4) )
				{
				case 0:
					for(x=0; x<DimX; x++)
						for(y=0; y<DimY; y++)
							EvalCell(x,y);
					break;

				case 2:
					for(x=DimX-1; x>=0; x--)
						for(y=0; y<DimY; y++)
							EvalCell(x,y);
					break;

				case 3:
					for(x=0; x<DimX; x++)
						for(y=DimY-1; y>=0; y--)
							EvalCell(x,y);
					break;

				case 1:
					for(x=DimX-1; x>=0; x--)
						for(y=DimY-1; y>=0; y--)
							EvalCell(x,y);
				}
			break;
		case 'A':
			switch(EventType)
				{
				case 1:
				case 4:
					for(x=0; x<DimX; x++)
						for(y=0; y<DimY; y++)
							EvalCell(x,y);
					break;

				case 2:
					EvalCellA2(x,y);
					break;

				case 3:
					EvalCellA3(x,y);
					break;
				}
			break;

		case 'B':
			switch(EventType)
				{
				case 1:
				case 4:
					EvalCellB1(x,y);
					break;

				case 2:
					EvalCellB2(x,y);
					break;

				case 3:
					EvalCellB3(x,y);
					break;
				}
			break;

		case 'D':
			switch(EventType)
				{
				case 1:
				case 4:
					max = DimX*DimY;
					for(i=0; i<max; i++)
						{
						x = Rand(DimX);
						y = Rand(DimY);
						EvalCell(x,y);
						}
					break;
				}
			break;
		}
	T++;


	if( TimeType == 'D' )
		EmptyPrevious();

#ifdef TEST
	PrintMem();
#endif


//	Ojo esto vale porque al morir utilizo C(x,y).Elem(0,0)
//	Sino hay que hacer el borrado del anterior
//
	}

void CAImp::EvalCellB1(int &x,int &y)
	{
	int max = DimX*DimY;
	CellEvent * pos = new CellEvent[max];
	int p=0;

	for(x=0; x<DimX; x++)
		for(y=0; y<DimY; y++)
			{
			pos[p].Elem(x,y);
			p++;
			}
	JumbleXY(pos, max);
	for(p=0;p<max;p++)
	   {
	   x = pos[p].Elem(0);
	   y = pos[p].Elem(1);
	   EvalCell(x,y);
	   }
	delete pos;
	}


void CAImp::EvalCellB2(int &x,int &y)
	{
	int p=0,max;
	max = DimX*DimY;
	static CellEvent * pos = new CellEvent[max];
	static int privez=1;


	if( privez )
	{
	for(x=0; x<DimX; x++)
		for(y=0; y<DimY; y++)
			{
			pos[p].Elem(x,y);
			p++;

			}
	privez=0;
	}

	//JumbleXY(pos, max);
	for(p=0;p<max;p++)
		{
		x = pos[p].Elem(0);
		y = pos[p].Elem(1);
		Survival(x,y);
		}
	JumbleXY(pos, max);
	for(p=0;p<max;p++)
		{
		x = pos[p].Elem(0);
		y = pos[p].Elem(1);
		Growth(x,y);
		}
	//delete pos;
	}

void CAImp::EvalCellB3(int &x,int &y)
	{
	int p=0,i,max;

	max = DimX*DimY*2;
	i = max /2;
	CellEvent * pos = new CellEvent[max];
	for(x=0; x<DimX; x++)
		for(y=0; y<DimY; y++)
			{
			pos[p].Elem(x,y);
			pos[p].event=1;
			pos[p+i].Elem(x,y);
			pos[p+i].event=0;
			p++;
			}

	JumbleXY(pos, max);

	for(p=0;p<max;p++)
		{
		x = pos[p].Elem(0);
		y = pos[p].Elem(1);
		//cout << x << "  " << y << " " << int(pos[p].event) << endl;
		if( pos[p].event==1 )
			Growth(x,y);
		else
			Survival(x,y);
		}
	delete pos;
	}

void CAImp::EvalCellA2(int &x,int &y)
	{
	int i;
	for(i=0; i<2; i++)
		{
		for(x=0; x<DimX; x++)
			for(y=0; y<DimY; y++)
				if( i==0)
					Survival(x,y);
				else
					Growth(x,y);
		//PrintMap(A);
		//PrintMap(N);
		}
	}

void CAImp::EvalCellA3(int &x,int &y)
	{
	int i;
	for(i=0; i<2; i++)
		{
		for(x=0; x<DimX; x++)
			for(y=0; y<DimY; y++)
				if( Rand(2)== 1)
					Growth(x,y);
				else
					Survival(x,y);
		}
	}


void CAImp::JumbleXY(CellEvent * pos, int max)
	{
	int p,p1,p2,m;
	CellEvent tmp;
	m = DimX*DimY*3;
	for(p=0; p<m; p++)
		{
		p1 = Rand(max);
		p2 = Rand(max);
		while(p1==p2)
			{
			p1 = Rand(max);
			p2 = Rand(max);
			}

		tmp=pos[p1];
		pos[p1] = pos[p2];
		pos[p2] = tmp;
		}
	}


void CAImp::EvalCell(int x,int y)
	{
	//ActualSp = Elem(x,y,A);
	if(EventType == 1)
		{
		Growth(x,y);
		Survival(x,y);
		}
	else if(EventType==4 )
		{
		if( Survival( x,y) )
			Growth(x,y);
		}
	}


void CAImp::Growth(int x,int y)
	{
	int dx=0,dy=0;
	unsigned dd,dis,ne;
	char dn;
	float pe ;
	ActualSp = Elem(x,y,A);
	if(ActualSp>0)
		{
		dd = Sp[ActualSp-1].VegDispersalDistance;
		ne = Sp[ActualSp-1].NVegetativeGrowth;
		pe = Sp[ActualSp-1].PVegetativeGrowth;
		dn = Sp[ActualSp-1].DispersalNorm;

		for(int i=0; i<ne; i++)
			{
			if( Rand() < pe )
				{
				do	{
					switch( dn )
						{
						// Euclidian Norm (Norma 2)
						case 'E':
							dx = Rand( dd*2+1 ) - dd;
							dis = sqrt( dd * dd - dx * dx );
							dy = Rand(dis*2+1) - dis ;
							break;

						// Von Newman (norma 1)
						case 'N':
							dx=Rand(dd*2+1)-dd;
							dis = dd - abs(dx);
							dy = Rand(dis*2+1) - dis ;
							break;
						// More (Norma infinito)
						case 'M':
							dx=Rand(dd*2+1)-dd;
							dy=Rand(dd*2+1)-dd;
						}
					}while( dx==0 && dy==0 );

				Colonize(x+dx,y+dy);
				}
			}
		}
#ifdef TEST
//		PrintMem();
#endif
	}



void CAImp::Colonize(int x,int y)
	{
	x = (x + DimX) % DimX;
	y = (y + DimY) % DimY;
	if( TimeType == 'D' )   //Discreto
		{
		if( Elem(x,y,A)==0 )
			if( Elem(x,y,N)==0 )
				{
				Elem(x,y,N)=ActualSp;
				}
		}
	else
		{
		if( Elem(x,y,N)==0 )
			Elem(x,y,N)=ActualSp;
		}

	}

#ifdef TEST
static int Hay=0,NoHay=0;
#endif


int CAImp::Survival(int x, int y)
	{
	ActualSp = Elem(x,y,A);
	double Ps = Sp[ActualSp-1].PSurvival;
	if(ActualSp>0)
		{
		if( Rand() > Ps )
			{
			C(x,y).Elem(0,0);
			return 0; 	//no sobrevivio o no hay nadie en ese lugar
			}
		else
			{
			if( TimeType == 'D' )
				{
				C(x,y).Transfer(N,A);
				//Elem(x,y,N)= Elem(x,y,A);
				// Podria hacer una funcion por la que
				// directamente pase de un array al otro
				// Seria MAS RAPIDO

				}

			return 1;					// Sobrevivio
			}
		}
	return 0;
	}


void CAImp::PrintMap(unsigned O)
	{
#ifdef TEST
	unsigned hay=0,nohay=0;
	for(int i=0; i<DimX; i++)
		{
		for(int e=0; e<DimY; e++)
			if( C(i,e).Elem(O) == 1 )
				hay++;
			else if( C(i,e).Elem(O) == 0 )
				nohay++;
			else
				cout << "Aqui hay un problema " << endl;
			//cout << C(i,e).Elem(O) << "  ";
		//cout << endl;
		}
	cout << O << " : " << hay << " Vacios : " << nohay << "  " << Sp[0].PSurvival << endl;
	cout << "Hay : " << Hay << " NoHay : " << NoHay << endl;
	Hay=0;
	NoHay=0;
	getch();
#endif
	}

void CAImp::PrintMem()
	{
	int i;
	cout << T << endl;
	for(i=0; i<DimY; i++)
		{
		for(int j=0;j<DimX;j++)
			{
			cout.width(2);
			cout << C(j,i).Elem(A) ;
			}
		cout << endl;
		}
	cout << endl << endl;

	for(i=0; i<DimY; i++)
		{
		for(int j=0;j<DimX;j++)
			{
			cout.width(2);
			cout << C(j,i).Elem(N) ;
			}
		cout << endl;
		}
	cout << endl << endl;

	}
