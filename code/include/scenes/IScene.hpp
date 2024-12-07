#pragma once

class IScene {
public:
    virtual ~IScene() = default;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
};