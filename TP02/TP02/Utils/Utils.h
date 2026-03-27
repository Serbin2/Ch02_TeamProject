#pragma once

// [작성자] : 박재현 
// [DESC] : 유틸 클래스  

#define DECLARE_SINGLE(classname)			\
private :									\
	classname() {}							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname m_sInstance;			\
		return &m_sInstance;					\
	}						

#define GET_SINGLE(classname)	classname::GetInstance()


namespace MathUtil
{
	template <typename T>
	[[nodiscard]] inline T Clamp(T Value, T Min, T Max)
	{
		if (Value < Min)
		{
			return Min;
		}
		else if (Max < Value)
		{
			return Max;
		}
		else
		{
			return Value;
		}
	}
}