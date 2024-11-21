#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "Externals.h"

void CLuaObject::Init()
{
	Error("What called this?");
}

void CLuaObject::Init(ILuaInterface* Lm_pLua)
{
	m_pLua = Lm_pLua;

	if (!m_pLua)
		m_pLua = g_Lua;
}

void CLuaObject::UnReference()
{
	m_pLua->ReferenceFree(m_reference);
	m_reference = -1;
	// Should we also reset the Metatable?
	// m_pLua->ReferenceFree(m_metatable)
	// m_metatable = -1;
}

void CLuaObject::SetReference(int reference)
{
	m_reference = reference;
}

void CLuaObject::Set(ILuaObject* obj)
{
	UnReference();
	SetReference(((CLuaObject*)obj)->m_reference);
}

void CLuaObject::SetFromStack(int pos)
{
	m_pLua->Push(pos);

	//if (m_pLua->GetType(-1) != Type::Nil)
	{
		UnReference();
		m_iLUA_TYPE = m_pLua->GetType( -1 );
		m_bUserData = m_iLUA_TYPE > 7;

		m_reference = m_pLua->ReferenceCreate();
	//} else {
	//	m_pLua->Pop(1);
	}
}

void CLuaObject::Push()
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
	} else {
		m_pLua->PushNil();
	}
}

int CLuaObject::GetType()
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		int type = m_pLua->GetType(-1);
		m_pLua->Pop(1);
		return type;
	} else {
		return Type::Nil;
	}
}

const char* CLuaObject::GetString()
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		const char* str = m_pLua->GetString(-1);
		m_pLua->Pop(1);
		return str == NULL ? "" : str;
	} else {
		return "";
	}
}

float CLuaObject::GetFloat()
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		float num = m_pLua->GetNumber(-1);
		m_pLua->Pop(1);
		return num;
	} else {
		return 0;
	}
}

int CLuaObject::GetInt()
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		int num = m_pLua->GetNumber(-1);
		m_pLua->Pop(1);
		return num;
	} else {
		return 0;
	}
}

double CLuaObject::GetDouble()
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		double num = m_pLua->GetNumber(-1);
		m_pLua->Pop(1);
		return num;
	} else {
		return 0;
	}
}

void* CLuaObject::GetUserData()
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		ILuaBase::UserData* data = m_pLua->GetUserdata(-1);
		m_pLua->Pop(1);
		return data->data;
	} else {
		return 0;
	}
}

void CLuaObject::SetMember(const char* name)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->Push(-2);
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(const char* name, ILuaObject *obj)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			obj->Push();
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(const char* name, float val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(val);
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(const char* name, bool val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushBool(val);
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(const char* name, const char* val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushString(val);
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(const char* name, CFunc val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushCFunction(val);
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

bool CLuaObject::GetMemberBool(const char* name, bool b)
{
	bool val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->IsType(-1, Type::Bool)) {
				val = m_pLua->GetBool(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

int CLuaObject::GetMemberInt(const char* name, int b)
{
	int val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->IsType(-1, Type::Number)) {
				val = m_pLua->GetNumber(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

float CLuaObject::GetMemberFloat(const char* name, float b)
{
	float val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->IsType(-1, Type::Number)) {
				val = m_pLua->GetNumber(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

const char* CLuaObject::GetMemberStr(const char* name, const char* b)
{
	const char* val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->IsType(-1, Type::String)) {
				val = m_pLua->GetString(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

void* CLuaObject::GetMemberUserData(const char* name, void* b)
{
	void* val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->IsType(-1, Type::UserData)) {
				val = m_pLua->GetUserdata(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

void* CLuaObject::GetMemberUserData(float name, void* b)
{
	void* val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(name);
			m_pLua->GetTable(-1);
			if (m_pLua->IsType(-1, Type::UserData)) {
				val = m_pLua->GetUserdata(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

ILuaObject* CLuaObject::GetMember(const char* name, ILuaObject* obj)
{
	if ( !obj )
		return NULL;

	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			obj->SetFromStack(-1);
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return obj;
}

ILuaObject* CLuaObject::GetMember(ILuaObject* name, ILuaObject* b)
{
	ILuaObject* val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			name->Push();
			m_pLua->GetTable(-1);
			if (m_pLua->IsType(-1, Type::Table)) {
				CLuaObject* obj = new CLuaObject;
				obj->Init(m_pLua);
				obj->SetFromStack(-1);
				val = obj;
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

void CLuaObject::SetMetaTable(ILuaObject* meta)
{
	Push();
	meta->Push();
	m_pLua->SetMetaTable( -2 );
}

void CLuaObject::SetUserData(void* data)
{
	UnReference();
	m_pLua->PushUserType(data, Type::UserData);
	SetFromStack(-1);
}

bool CLuaObject::isNil()
{
	return GetType() == Type::Nil;
}

bool CLuaObject::isTable()
{
	return GetType() == Type::Table;
}

bool CLuaObject::isString()
{
	return GetType() == Type::String;
}

bool CLuaObject::isNumber()
{
	return GetType() == Type::Number;
}

bool CLuaObject::isFunction()
{
	return GetType() == Type::Function;
}

bool CLuaObject::isUserData()
{
	return GetType() == Type::UserData;
}

ILuaObject* CLuaObject::GetMember(float name, ILuaObject* b)
{
	ILuaObject* val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(name);
			m_pLua->GetTable(-1);
			if (m_pLua->IsType(-1, Type::Table)) {
				CLuaObject* obj = new CLuaObject;
				obj->Init(m_pLua);
				obj->SetFromStack(-1);
				val = obj;
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

void* CLuaObject::Remove_Me_1(const char* name, void*)
{
	Error("Now u made me Angry :< (Don't call Im_pLuaObject->Remove_Me_1)");
	return nullptr;
}

void CLuaObject::SetMember(float name)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->Push(-2);
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(float name, ILuaObject *obj)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			obj->Push();
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(float name, float val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(val);
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

/*void CLuaObject::SetMember(float name, float val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(val);
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}*/

void CLuaObject::SetMember(float name, bool val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushBool(val);
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(float name, const char* val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushString(val);
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(float name, CFunc val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushCFunction(val);
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

const char* CLuaObject::GetMemberStr(float name, const char* b)
{
	const char* val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(name);
			m_pLua->GetTable(-1);
			if (m_pLua->IsType(-1, Type::String)) {
				val = m_pLua->GetString(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

void CLuaObject::SetMember(ILuaObject* name, ILuaObject* val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			name->Push();
			val->Push();
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

bool CLuaObject::GetBool()
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		bool val = m_pLua->GetBool(-1);
		m_pLua->Pop(1);
		return val;
	} else {
		return false;
	}
}

bool CLuaObject::PushMemberFast( int iStackPos )
{
	Error("What calls this? What does it do? (CLuaObject::PushMemberFast)");
	return false;
}

void CLuaObject::SetMemberFast(int name, int val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(name);
			m_pLua->PushNumber(val);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetFloat(float val)
{
	UnReference();
	m_pLua->PushNumber(val);
	SetFromStack(-1);
}

void CLuaObject::SetString(const char* val)
{
	UnReference();
	m_pLua->PushString(val);
	SetFromStack(-1);
}

void CLuaObject::SetMember_FixKey(const char* key, float val)
{
	Error("No.1");
}

void CLuaObject::SetMember_FixKey(const char* key, const char* val)
{
	Error("No.2");
}

void CLuaObject::SetMember_FixKey(const char* key, ILuaObject* val)
{
	Error("No.3");
}

void CLuaObject::SetMember_FixKey(const char* key, double val)
{
	Error("No.4");
}

void CLuaObject::SetMember_FixKey(const char* key, int val)
{
	Error("No.5");
}

bool CLuaObject::isBool()
{
	return GetType() == Type::Bool;
}

void CLuaObject::SetMemberDouble(const char* name, double val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(val);
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMemberNil(const char* name)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNil();
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMemberNil(float name)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(name);
			m_pLua->PushNil();
			m_pLua->SetTable(-2);
		}
		m_pLua->Pop(1);
	}
}

bool CLuaObject::RemoveMe()
{
	return false;
}

void CLuaObject::SetFromGlobal( const char* name )
{
	m_pLua->PushSpecial(SPECIAL_GLOB);
		m_pLua->GetField(-1, name);
		SetFromStack(-1);
	m_pLua->Pop(2);
}

int CLuaObject::GetStringLen(unsigned int* idk)
{
	m_pLua->ReferencePush(m_reference);
	const char* str = m_pLua->GetString(-1);
	m_pLua->Pop(1);

	return strlen(str);
}

unsigned int CLuaObject::GetMemberUInt(const char* name, unsigned int b)
{
	unsigned int val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->IsType(-1, Type::Number)) {
				val = m_pLua->GetNumber(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

void CLuaObject::SetMember(const char* name, unsigned long long val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(val);
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMember(const char* name, int val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(val);
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::RemoveMember(const char* name)
{
	SetMemberNil(name);
}

void CLuaObject::RemoveMember(float name)
{
	SetMemberNil(name);
}

bool CLuaObject::MemberIsNil(const char* name)
{
	bool val = true;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->GetType(-1) != Type::Nil) {
				val = false;
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

void CLuaObject::SetMemberDouble(float name, double val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(val);
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

double CLuaObject::GetMemberDouble(const char* name, double b)
{
	double val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->GetType(-1) == Type::Number) {
				val = m_pLua->GetNumber(-1);
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

BaseEntity* CLuaObject::GetMemberEntity(const char* name, BaseEntity* b)
{
	BaseEntity* val = b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->GetType(-1) == Type::Entity) {
				//val = Get_Entity
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return val;
}

void CLuaObject::SetMemberEntity(float name, BaseEntity* val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			// Push_Entity
			m_pLua->PushNumber(name);
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMemberEntity(const char* name, BaseEntity* val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			// Push_Entity
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

bool CLuaObject::isEntity()
{
	return GetType() == Type::Entity;
}

BaseEntity* CLuaObject::GetEntity()
{
	m_pLua->ReferencePush(m_reference);
	//BaseEntity* ent = Get_Entity();
	m_pLua->Pop(1);
	return nullptr; //ent;
}

void CLuaObject::SetEntity(BaseEntity* ent)
{
	UnReference();
	// Push_Entity()
	m_reference = m_pLua->ReferenceCreate();
	m_pLua->Pop(1);
}

void CLuaObject::SetMemberVector(const char* name, Vector* val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			// Push_Vector
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMemberVector(const char* name, Vector& val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			// Push_Vector
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMemberVector(float name, Vector* val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->PushNumber(name);
			// Push_Vector
			m_pLua->SetTable(-3);
		}
		m_pLua->Pop(1);
	}
}

Vector* CLuaObject::GetMemberVector(const char* name, const Vector* b)
{
	Vector val = *b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->GetType(-1) == Type::Vector) {
				//val = Get_Vector
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return &val;
}

Vector* CLuaObject::GetMemberVector(int what)
{
	Error("What? (CLuaObject::GetMemberVector)");
	// Get_Vector

	return nullptr;
}

bool CLuaObject::isVector()
{
	return GetType() == Type::Vector;
}

Vector* CLuaObject::GetVector()
{
	m_pLua->ReferencePush(m_reference);
	Vector vec = Vector(); // Get_Vector
	m_pLua->Pop(1);

	return &vec;
}

void CLuaObject::SetMemberAngle(const char* name, QAngle& val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			// Push_Angle
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

void CLuaObject::SetMemberAngle(const char* name, QAngle* val)
{
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			// Push_Angle
			m_pLua->SetField(-2, name);
		}
		m_pLua->Pop(1);
	}
}

QAngle* CLuaObject::GetMemberAngle(const char* name, QAngle* b)
{
	QAngle val = *b;
	if (m_reference != -1)
	{
		m_pLua->ReferencePush(m_reference);
		if (m_pLua->IsType(-1, Type::Table))
		{
			m_pLua->GetField(-1, name);
			if (m_pLua->GetType(-1) == Type::Angle) {
				// val = Get_Angle
			}
			m_pLua->Pop(1);
		}
		m_pLua->Pop(1);
	}

	return &val;
}

bool CLuaObject::isAngle()
{
	return GetType() == Type::Angle;
}

QAngle* CLuaObject::GetAngle()
{
	m_pLua->ReferencePush(m_reference);
	QAngle ang = QAngle(); // Get_Angle
	m_pLua->Pop(1);

	return &ang;
}

void CLuaObject::SetMemberMatrix(const char* name, const VMatrix* val)
{
	Error("Not implemented! (CLuaObject::SetMemberMatrix)");
}

void CLuaObject::SetMemberMatrix(const char* name, const VMatrix& val)
{
	Error("Not implemented! (CLuaObject::SetMemberMatrix)");
}

void CLuaObject::SetMemberMatrix(float name, const VMatrix* val)
{
	Error("Not implemented! (CLuaObject::SetMemberMatrix)");
}

void CLuaObject::SetMemberMatrix(int name, const VMatrix* val)
{
	Error("Not implemented! (CLuaObject::SetMemberMatrix)");
}

void CLuaObject::SetMemberPhysObject(const char* name, IPhysicsObject* val)
{
	Error("Not implemented! (CLuaObject::SetMemberPhysObject)");
}

/*
	CLuaClass
*/

CLuaClass::CLuaClass(const char* name, int type, CLuaClassFunc func, const char* baseclass)
{
	m_strName = name;
	m_iType = type;
	m_pInitFunc = func;
	m_strBaseClass = baseclass;
}

void* CLuaClass::Get(int index) // Probably done differently.
{
	if (g_Lua->IsType(index, m_iType))
		return g_Lua->GetUserType<void*>(index, m_iType);

	return nullptr;
}

void CLuaClass::Push(void* udata)
{
	g_Lua->PushUserType(udata, m_iType);
	g_Lua->ReferencePush(m_iReference);
	g_Lua->SetMetaTable(-2);
}

void CLuaClass::InitClass()
{
	m_pInitFunc();
	m_iReference = g_Lua->ReferenceCreate();
}

void CLuaClass::MetaTableDerive()
{
	// ToDo: Implement it for later when we have Players and Entities
}

void InitLuaClasses(ILuaInterface* LUA)
{
	angle_class.InitClass();
	vector_class.InitClass();
	Lua::LC_File.InitClass();
	LC_ConVar.InitClass();

#ifndef MENUSYSTEM
	entity_class.InitClass();
#endif

#ifdef GAME_DLL
	recipientfilter_class.InitClass();
#endif

#ifdef CLIENT_DLL
	LC_bf_read.InitClass();
#endif
}

/*
	CLuaLibrary
*/

std::vector<CLuaLibrary*> libaries;
CLuaLibrary::CLuaLibrary(const char* name, CLuaClassFunc func)
{
	m_strName = name;
	m_pFuncs.push_back(func);

	libaries.push_back(this);
}

void CLuaLibrary::Add(CLuaClassFunc func)
{
	m_pFuncs.push_back(func);
}

void CLuaLibrary::Push() // Idk
{
	for (CLuaClassFunc func : m_pFuncs)
	{
		func();
	}
}

void InitLuaLibraries(ILuaInterface* LUA)
{
	LL_Global.Push();
	file_library.Push();

#ifndef MENUSYSTEM
	ents_library.Push();
#endif

#ifdef GAME_DLL
	umsg_library.Push();
	LL_Engine.Push();
#endif

#ifdef CLIENT_DLL
	LL_Render.Push();
#endif
}