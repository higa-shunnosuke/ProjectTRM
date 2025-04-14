#include "InputEventManager.h"

void InputEventManager::InputActionEvent()
{
	//入力管理クラスのインスタンスを取得する
	InputManager* input = InputManager::GetInstance();

	//キーボード入力イベントがあれば、
	//キーボード入力イベントの実行
	if (!keycode_event.empty())
	{
		for (EventSender sender : keycode_event)
		{
			/*if (input->GetKeyState(sender.input_id) == sender.event_state &&
				sender.value != 0.0f)
			{
				sender.call_base_func->Invoke();
			}
			else */if (input->GetKeyState(sender.input_id) == sender.event_state &&
				sender.event_state != eInputState::None)
			{
				sender.call_base_func->Invoke();
			}
			else if (input->GetKeyState(sender.input_id) == eInputState::Hold)
			{
				sender.call_base_func->Invoke(sender.value);
			}
		}
	}
}

void InputEventManager::AllUnbind()
{
	//登録されているイベント内容を削除する
	if (!keycode_event.empty())
	{
		for (EventSender sender : keycode_event)
		{
			delete sender.call_base_func;
			sender.call_base_func = nullptr;
		}
		keycode_event.clear();
	}
}