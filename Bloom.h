#pragma once

/*takes in the finished rendered scene from forward rendering to apply an bloom effect to it,
then returns it back to the forward render. -steph*/
class Bloom {
private:
protected:
	void Apply();
public:
	Bloom();
	~Bloom();

};
