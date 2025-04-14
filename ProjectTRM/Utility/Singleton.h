#pragma once

template <class Type>
class Singleton
{
protected:
	Singleton() = default;

	// コピーガード
	Singleton(const Singleton&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	// ~コピーガード

protected:
	~Singleton() = default;

public:
	static Type* GetInstance()
	{
		static Type* instance = new Type();
		return instance;
	}
};
