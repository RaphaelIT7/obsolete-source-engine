#include <GarrysMod/Lua/LuaObject.h>
#include <GarrysMod/Lua/LuaGameCallback.h>
#include <GarrysMod/Lua/LuaInterface.h>

class CLuaGameCallback : public GarrysMod::Lua::ILuaGameCallback
{
public:
	virtual GarrysMod::Lua::ILuaObject* CLuaGameCallback::CreateLuaObject();
	virtual void CLuaGameCallback::DestroyLuaObject(GarrysMod::Lua::ILuaObject* pObject);
	virtual void CLuaGameCallback::ErrorPrint(const char* error, bool print);
	virtual void CLuaGameCallback::Msg(const char* msg, bool unknown);
	virtual void CLuaGameCallback::MsgColour(const char* msg, const Color& color);
	virtual void CLuaGameCallback::LuaError(const CLuaError* error);
	virtual void CLuaGameCallback::InterfaceCreated(GarrysMod::Lua::ILuaInterface* iface);
};

//Color server_msg(156, 241, 255, 200);
//Color server_error(136, 221, 255, 255);
//Color client_msg(255, 241, 122, 200);
//Color client_error(255, 221, 102, 255);
//Color menu_msg(100, 220, 100, 200);
//Color menu_error(120, 220, 100, 255);

void UTLVarArgs(char* buffer, const char* format, ...) {
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
}

GarrysMod::Lua::ILuaObject* CLuaGameCallback::CreateLuaObject()
{
	return nullptr;// new CLuaObject();
}

void CLuaGameCallback::DestroyLuaObject(GarrysMod::Lua::ILuaObject* pObject)
{
	delete pObject;
}

void CLuaGameCallback::ErrorPrint(const char* error, bool print)
{
	// Write into the lua_errors_server.txt if error logging is enabled.

	//Color ErrorColor = server_error; // ToDo: Change this later and finish this function.
	//ColorSpewMessage(SPEW_MESSAGE, &ErrorColor, "%s\n", error);
}

void CLuaGameCallback::Msg(const char* msg, bool unknown)
{
	//MsgColour(msg, server_msg);
}

void CLuaGameCallback::MsgColour(const char* msg, const Color& color)
{
	//ColorSpewMessage(SPEW_MESSAGE, &color, "%s\n", msg);
}

void CLuaGameCallback::LuaError(const CLuaError* error)
{
	// fk it.
}

void CLuaGameCallback::InterfaceCreated(GarrysMod::Lua::ILuaInterface* iface) {} // Unused