#pragma once

class ILuaInterface;

abstract_class ILuaUser
{
public:
	virtual ~ILuaUser() = 0;
	virtual bool IsUsingLua() = 0;
	virtual void InitLibraries(ILuaInterface*) = 0;
};