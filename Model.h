#pragma once

class Model
{
private:
	bool m_notLoaded;
public:
	Model();
	~Model() = default;
	bool NotLoaded();
};