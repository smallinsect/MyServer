#pragma once
class IServer{
public:
	virtual int Start() = 0;
	virtual int Stop() = 0;
};

