#pragma once
class IConnection{
public:
	virtual int Start() = 0;
	virtual int Stop() = 0;
};

