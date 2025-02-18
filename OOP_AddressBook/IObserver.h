#pragma once
class IObserver
{
public:
	IObserver(){}
	virtual ~IObserver() = default;
	virtual void Update() = 0;
};

