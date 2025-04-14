#pragma once

#include "InputManager.h"
#include "vector"
#include "../Singleton.h"

//イベント基底クラス
__interface BaseEvent
{
public:
	virtual void Invoke(float param = 0.0f) = 0;
};

//float型の引数を持つ関数イベントクラス
template <class T>
class FEvent :public BaseEvent
{
private:
	T* object;
	void (T::* function)(float);

public:
	FEvent(T* object, void (T::* function)(float))
	{
		this->object = object;
		this->function = function;
	}

	void Invoke(float param = 0.0f) override
	{
		(object->*function)(param);
	}
};

//引数を持たない関数イベントクラス
template <class T>
class VEvent :public BaseEvent
{
private:
	T* object;
	void (T::* function)(void);

public:
	VEvent(T* object, void (T::* function)(void))
	{
		this->object = object;
		this->function = function;
	}

	void Invoke(float param) override
	{
		(object->*function)();
	}
};

//関数イベント呼出クラス
class EventSender
{
public:
	BaseEvent* call_base_func = nullptr;
	int input_id = 0;
	eInputState event_state = eInputState::None;
	float value = 0.0f;
};

//入力イベント管理クラス
class InputEventManager : public Singleton<InputEventManager>
{
private:
	std::vector<EventSender> keycode_event;

public:
	//バインドされた入力イベントの呼び出し
	void InputActionEvent();

public:
	//バインド関数
	template <class T>
	void ActionKeyBind(int keycode, eInputState state, T* object, void (T::* func)(void))
	{
		EventSender sender = {};
		sender.call_base_func = new VEvent<T>(object, func);
		sender.event_state = state;
		sender.input_id = keycode;
		keycode_event.push_back(sender);
	}

	template <class T>
	void ActionKeyBind(int keycode, float value, T* object, void (T::* func)(float))
	{
		EventSender sender = {};
		sender.call_base_func = new FEvent<T>(object, func);
		sender.value = value;
		sender.input_id = keycode;
		keycode_event.push_back(sender);
	}

	template <class T>
	void ActionKeyBind(int keycode, eInputState state, float value, T* object, void (T::* func)(float))
	{
		EventSender sender = {};
		sender.call_base_func = new FEvent<T>(object, func);
		sender.event_state = state;
		sender.value = value;
		sender.input_id = keycode;
		keycode_event.push_back(sender);
	}

	//バインド解除関数
	void AllUnbind();
};