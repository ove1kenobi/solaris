#pragma once

const unsigned int numberOfResources = 8;

enum class Resource{
	Fuel = 0,
	Oxygen,
	Titanium,
	ScrapMetal,
	Nanotech,
	Plasma,
	Radium,
	Khionerite
};

struct Inventory {
	int fuel;
	int oxygen;
	int titanium;
	int scrapMetal;
	int nanotech;
	int plasma;
	int radium;
	int khionerite;
};