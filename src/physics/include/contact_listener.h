#pragma once

#include "box2d/box2d.h"
#include <map>
#include <vector>

namespace gamo
{
    class Actor;
    class Collider;

    using ContactMap = std::map<Collider *, std::vector<Collider *>>;

    class ContactListener : public b2ContactListener
    {
    private:
        ContactMap _begin_contact_buffer;
        ContactMap _end_contact_buffer;

    public:
        void BeginContact(b2Contact *contact) override
        {
            auto collider1 = (Collider *)contact->GetFixtureA()->GetUserData().pointer;
            auto collider2 = (Collider *)contact->GetFixtureB()->GetUserData().pointer;
            if (_begin_contact_buffer.count(collider1) == 0)
            {
                _begin_contact_buffer[collider1] = std::vector<Collider *>();
            }
            _begin_contact_buffer[collider1].push_back(collider2);
            if (_begin_contact_buffer.count(collider2) == 0)
            {
                _begin_contact_buffer[collider2] = std::vector<Collider *>();
            }
            _begin_contact_buffer[collider2].push_back(collider1);
        }

        void EndContact(b2Contact *contact) override
        {
            auto collider1 = (Collider *)contact->GetFixtureA()->GetUserData().pointer;
            auto collider2 = (Collider *)contact->GetFixtureB()->GetUserData().pointer;
            if (_end_contact_buffer.count(collider1) == 0)
            {
                _end_contact_buffer[collider1] = std::vector<Collider *>();
            }
            _end_contact_buffer[collider1].push_back(collider2);
            if (_end_contact_buffer.count(collider2) == 0)
            {
                _end_contact_buffer[collider2] = std::vector<Collider *>();
            }
            _end_contact_buffer[collider2].push_back(collider1);
        }

        ContactMap &BeginContactBuffer() { return _begin_contact_buffer; }
        ContactMap &EndContactBuffer() { return _end_contact_buffer; }
    };

} // namespace gamo
