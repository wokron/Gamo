#include "actor.h"
#include "scene.h"
#include <algorithm>

namespace gamo
{
    Actor::Actor(Vect position, float rotate, Vect scale)
    {
        _transform = new Transform(this, position, rotate, scale);
        _characteristics.push_back(_transform);
    }

    Actor::~Actor()
    {
        for (auto sub_actor : _characteristics)
        {
            delete sub_actor;
        }
    }

    Characteristic *Actor::GetCharacteristicByType(std::string type)
    {
        auto found = std::find_if(
            _characteristics.begin(), _characteristics.end(),
            [type](Characteristic *ch)
            { return ch->Type() == type; });
        if (found == _characteristics.end())
        {
            return nullptr;
        }
        else
        {
            return *found;
        }
    }

    std::vector<Characteristic *> Actor::GetCharacteristicsByType(std::string type)
    {
        std::vector<Characteristic *> result;

        auto cmp = [type](Characteristic *ch)
        { return ch->Type() == type; };

        auto it = _characteristics.begin();
        while ((it = std::find_if(it, _characteristics.end(), cmp)) != _characteristics.end())
        {
            result.push_back(*it);
            it++;
        }

        return result;
    }

    void Actor::RegisterSystemEvents()
    {
        for (auto item : GetCharacteristics())
        {
            item->RegisterEvents();
        }

        // recursively register events in sub actors
        for (auto sub_actor : _sub_actors)
        {
            sub_actor->RegisterSystemEvents();
        }
    }

    void Actor::UnregisterSystemEvents()
    {
        for (auto item : GetCharacteristics())
        {
            item->UnregisterEvents();
        }

        // recursively unregister events in sub actors
        for (auto sub_actor : _sub_actors)
        {
            sub_actor->UnregisterSystemEvents();
        }
    }

    Actor *Actor::CreateSubActor(Vect position, float rotate, Vect scale)
    {
        Actor *sub_actor = new Actor(this, position, rotate, scale);
        _sub_actors.push_back(sub_actor);
        return sub_actor;
    }

    void Actor::Destroy()
    {
        if (_sup_actor != nullptr)
        {
            _sup_actor->RemoveResource(this);
        }
        else if (_belong_scene != nullptr)
        {
            _belong_scene->RemoveResource(this);
        }
    }

    void Actor::Deref()
    {
        RegisterHandleMemFree();
        while (!_sub_actors.empty())
        {
            auto item = _sub_actors.back();
            _sub_actors.pop_back();
            item->Deref();
        }
        while (!_characteristics.empty())
        {
            auto item = _characteristics.back();
            _characteristics.pop_back();
            item->Deref();
        }
    }

    void Actor::RemoveResource(Actor *item)
    {
        auto find = std::find(_sub_actors.begin(), _sub_actors.end(), item);
        if (find == _sub_actors.end())
            return;

        _sub_actors.erase(find);
        item->Deref();
    }

    void Actor::RemoveResource(Characteristic *item)
    {
        auto find = std::find(_characteristics.begin(), _characteristics.end(), item);
        if (find == _characteristics.end())
            return;

        _characteristics.erase(find);
        item->Deref();
    }

    Actor *Actor::Clone()
    {
        auto obj = new Actor(_transform->Position(), _transform->Rotate(), _transform->Scale());
        for (auto item : _characteristics)
        {
            if (item->Type() == "Transform")
                continue;
            auto item_clone = item->Clone();
            item_clone->BelongActor(obj);
            obj->AddCharacteristic(item_clone);
        }
        
        obj->_layer = _layer;

        for (auto sub_actor : _sub_actors)
        {
            auto sub_actor_clone = sub_actor->Clone();
            sub_actor_clone->_sup_actor = obj;
            obj->_sub_actors.push_back(sub_actor_clone);
        }

        obj->_sup_actor = nullptr;
        obj->_belong_scene = nullptr;
        return obj;
    }

} // namespace gamo
