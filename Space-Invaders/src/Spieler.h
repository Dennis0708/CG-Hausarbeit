
#include "Model.h"

class Spieler: public Model
{
	int lebenspunkte;
	float linksRechts;
public:
	Spieler(const char* ModelFile, bool FitSize, int lebenspunkte);
	void steuern(float lr);
	void update(float dtime);
};