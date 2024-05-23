#pragma once

#include <string>
#include <iostream>
#include <rend/rend.h>

namespace red
{

struct Resources;

struct Resource
{
public:
	virtual void onLoad() = 0;

	std::string getPath() const { return m_path; }

private:
	friend struct Resources;

	void load() { onLoad(); }

	std::string m_path;
	std::weak_ptr<Resources> m_resources;
	
};

}