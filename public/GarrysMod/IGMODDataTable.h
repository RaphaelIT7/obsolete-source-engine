#pragma once

#include "basehandle.h"

class CBaseHandle;
struct CGMODVariant
{
	enum GMODVariantType : unsigned char {
		TYPE_NIL,
		TYPE_FLOAT,
		TYPE_INT,
		TYPE_BOOL,
		TYPE_VECTOR,
		TYPE_ANGLE,
		TYPE_ENTITY,
		TYPE_STRING,
	};

	CGMODVariant(const CGMODVariant& other) {
		*this = other;
	}

	CGMODVariant() { memset(this, 0, sizeof(CGMODVariant)); }
	GMODVariantType type = TYPE_NIL; // Used by Push_GMODVariant to determen which function to use to push it.
	union {
		struct {
			int m_Length;
			char* m_pString;
		};
		bool m_Bool;
		float m_Float;
		int m_Int;
		CBaseHandle m_Ent;
		Vector m_Vec;
		QAngle m_Ang;
	};

	GMODVariantType GetType() const {
		return type;
	}

	void ToVector(Vector& vec) const {
		if (type != TYPE_VECTOR) {
			vec.Init();
			return;
		}

		vec = m_Vec;
	}

	void ToAngle(QAngle& ang) const {
		if (type != TYPE_ANGLE) {
			ang.Init();
			return;
		}

		ang = m_Ang;
	}

	bool ToBool() const {
		switch (type) {
			case TYPE_BOOL:   return m_Bool;
			case TYPE_INT:    return m_Int != 0;
			case TYPE_FLOAT:  return m_Float != 0.0f;
			case TYPE_STRING: return m_pString && m_Length > 0;
			case TYPE_VECTOR: return m_Vec.x != 0 || m_Vec.y != 0 || m_Vec.z != 0;
			case TYPE_ANGLE:  return m_Ang.x != 0 || m_Ang.y != 0 || m_Ang.z != 0;
			default:          return false;
		}
	}

	int ToInt() const {
		switch (type) {
			case TYPE_INT:    return m_Int;
			case TYPE_BOOL:   return m_Bool ? 1 : 0;
			case TYPE_FLOAT:  return static_cast<int>(m_Float);
			case TYPE_STRING: return m_pString ? atoi(m_pString) : 0;
			default:          return 0;
		}
	}

	float ToFloat() const {
		switch (type) {
			case TYPE_FLOAT:  return m_Float;
			case TYPE_INT:    return static_cast<float>(m_Int);
			case TYPE_BOOL:   return m_Bool ? 1.0f : 0.0f;
			case TYPE_STRING: return m_pString ? static_cast<float>(atof(m_pString)) : 0.0f;
			default:          return 0.0f;
		}
	}

	const CBaseHandle& ToEntity() const {
		if (type != TYPE_ENTITY)
			return CBaseHandle();

		return m_Ent;
	}

	const char* ToString() const {
		if (type != TYPE_STRING)
			return NULL;

		return m_pString;
	}

	CGMODVariant& operator=(bool b) {
		type = TYPE_BOOL;
		m_Bool = b;
		return *this;
	}

	CGMODVariant& operator=(int i) {
		type = TYPE_INT;
		m_Int = i;
		return *this;
	}

	CGMODVariant& operator=(float f) {
		type = TYPE_FLOAT;
		m_Float = f;
		return *this;
	}

	CGMODVariant& operator=(const Vector& vec) {
		type = TYPE_VECTOR;
		m_Vec = vec;
		return *this;
	}

	CGMODVariant& operator=(const QAngle& ang) {
		type = TYPE_ANGLE;
		m_Ang = ang;
		return *this;
	}

	CGMODVariant& operator=(const CBaseHandle& ent) {
		type = TYPE_ENTITY;
		m_Ent = ent;
		return *this;
	}

	CGMODVariant& operator=(const char* str) {
		type = TYPE_STRING;
		if (str) {
			m_Length = strlen(str);
			m_pString = new char[m_Length + 1];
			memcpy(m_pString, str, m_Length);
			m_pString[m_Length] = '\0';
		} else {
			m_Length = 0;
			m_pString = nullptr;
		}
		return *this;
	}

	CGMODVariant& operator=(const CGMODVariant& val) {
		if (this == &val)
			return *this;

		if (type == TYPE_STRING && m_pString) {
			delete[] m_pString;
			m_pString = nullptr;
			m_Length = 0;
		}

		type = val.GetType();
		switch (type) {
			case TYPE_INT:
				m_Int = val.m_Int;
				break;
			case TYPE_FLOAT:
				m_Float = val.m_Float;
				break;
			case TYPE_BOOL:
				m_Bool = val.m_Bool;
				break;
			case TYPE_VECTOR:
				m_Vec = val.m_Vec;
				break;
			case TYPE_ANGLE:
				m_Ang = val.m_Ang;
				break;
			case TYPE_ENTITY:
				m_Ent = val.m_Ent;
				break;
			case TYPE_STRING:
				if (val.m_pString && val.m_Length > 0) {
					m_Length = val.m_Length;
					m_pString = new char[m_Length + 1];
					memcpy(m_pString, val.m_pString, m_Length);
					m_pString[m_Length] = '\0';
				} else {
					m_pString = nullptr;
					m_Length = 0;
				}
				break;
			default:
				memset(this, 0, sizeof(CGMODVariant));
				break;
		}
	}

	bool operator==(const CGMODVariant& rhs) const {
		if (type != rhs.type)
			return false;

		switch (type) {
			case TYPE_BOOL:   return m_Bool == rhs.m_Bool;
			case TYPE_INT:    return m_Int == rhs.m_Int;
			case TYPE_FLOAT:  return m_Float == rhs.m_Float;
			case TYPE_STRING:
				if (!m_pString || !rhs.m_pString) return false;
				return m_Length == rhs.m_Length && strcmp(m_pString, rhs.m_pString) == 0;
			case TYPE_VECTOR: return m_Vec == rhs.m_Vec;
			case TYPE_ANGLE:  return m_Ang == rhs.m_Ang;
			case TYPE_ENTITY: return m_Ent == rhs.m_Ent;
			default:          return true;
		}
	}
	bool operator!=(const CGMODVariant& rhs) const { return !(*this == rhs); }
};

abstract_class IGMODDataTable
{
public:
	// ToDo - Finish implementation
	class Iterator
	{
	public:
		Iterator(const IGMODDataTable* pTable, unsigned short nIteratorPos) {
			m_nIteratorPos = nIteratorPos;
		}

	private:
		unsigned short m_nIteratorPos;
	};

	virtual int GetKey( int index ) const = 0;
	virtual const CGMODVariant& GetValue( int index ) const = 0;
	virtual void IncrementIterator( int& index ) const = 0;
	virtual const CGMODVariant& Get( int index ) const = 0;
	virtual void Set( int index, CGMODVariant const& value ) = 0;
	virtual bool HasKey( int index ) const = 0;
	virtual const CGMODVariant& GetLocal( const char* key ) const = 0;
	virtual void SetLocal( const char* key, CGMODVariant const& value ) = 0;
	virtual void ClearLocal( const char* key ) = 0;
	virtual void Clear() = 0;
	virtual Iterator Begin() const = 0;
	virtual Iterator End() const = 0;
};

typedef void (*GMODRecvProxy)(void* pStruct, int nIndex, const CGMODVariant& pValue);