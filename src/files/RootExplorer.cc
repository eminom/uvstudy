

#include "RootExplorer.h"
#include "DirWalker.h"

static RootExplorer *_instance = nullptr;

RootExplorer::~RootExplorer(){}
RootExplorer::RootExplorer(){}

RootExplorer* RootExplorer::getInstance()
{
	if(!_instance)
	{
		_instance = new RootExplorer;
	}
	return _instance;
}

void RootExplorer::setWorkingDir(const std::string &d)
{
	_rootdir = d;
}

const char* RootExplorer::getWorkingDir()const
{
	return _rootdir.c_str();
}

void RootExplorer::retrieveContent(std::string &out)
{
	elicitDir(_rootdir.c_str(), out);
}

