#pragma once

#include "../GameObject.h"

class Ground :public GameObject
{
public:

public:
	Ground();
	~Ground();

public:
	virtual void Initialize() override;
	virtual void Update(float delta_second) override;
	virtual void Draw(const Vector2D camera_pos) const override;
	virtual void Finalize() override;
};

