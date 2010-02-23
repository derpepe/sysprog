/*
 *  Marker.h
 *  sysprog
 *
 *  Created by Peter Schneider on 22.02.10.
 *  Copyright 2010 Schneider Medien Services. All rights reserved.
 *
 */

#include <iostream>
#include <string>
#include <sstream>

class Marker
{
	
public:
	Marker();
	static int markerCount;
	
	int getName();
	
private:
	int myMarker;


};