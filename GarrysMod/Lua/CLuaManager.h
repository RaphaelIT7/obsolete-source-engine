class CLuaManager
{
public:
	static void Startup();
	static void RunScript( const char* file, const char* pathID, bool, const char* );
	static void RunScriptsInFolder( const char* folder, const char* pathID );
	static void SendScriptsInFolder( const char* folder, const char* pathID );
	static bool ScriptExists( const char* file, const char* pathID );
	static void CreateEntity( const char* ent_class );
};