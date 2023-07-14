#pragma once

#include <vector>
#include <string>
#include "utils.h"
#include "characteristic.h"

// this macro is used to represent the layer in actor and camera. use like LAYER(0), LAYER(31). n in [0, 32)
#define LAYER(n) (1u << (n))

namespace gamo
{
    class Actor
    {
    private:
        std::vector<Characteristic *> _characteristics = std::vector<Characteristic *>();
        Transform *_transform;
        unsigned int _layer = 0u;

        std::vector<Actor *> _sub_actors = std::vector<Actor *>();
        Actor *_sup_actor = nullptr;

    public:
        std::vector<Characteristic *> &GetCharacteristics() { return _characteristics; }

        Transform *GetTransform() { return _transform; }

        std::vector<Actor *> SubActors() { return _sub_actors; }

        Actor *SupActor() { return _sup_actor; }

        unsigned int Layer() { return _layer; }
        void Layer(unsigned int layer) { _layer = layer; }

        Actor(FPoint position, float rotate, FVect scale);
        ~Actor();

        /// @brief get characteristic by type, 
        /// this function will only return the first characteristic with the type
        /// @param type a string value of the class name you want to get
        /// @return the characteristic with the type, null on not found
        Characteristic *GetCharacteristicByType(std::string type);
        
        /// @brief get characteristics by type,
        /// this function will return all characteristics with the type in this actor
        /// @param type a string value of the class name you want to get
        /// @return the characteristic with the type, null on not found
        std::vector<Characteristic *> GetCharacteristicsByType(std::string type);

        Actor *CreateSubActor(FPoint position, float rotate, FVect scale);

    private:
        Actor(Actor *sup_actor, FPoint position, float rotate, FVect scale) : Actor(position, rotate, scale) { _sup_actor = sup_actor; }
    };
} // namespace gamo
