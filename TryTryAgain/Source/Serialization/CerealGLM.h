#pragma once
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace glm
{
    template<typename Archive> void serialize(Archive& archive, glm::vec3& v3)
    {
        archive(cereal::make_nvp("x", v3.x), cereal::make_nvp("y", v3.y), cereal::make_nvp("z", v3.z));
    }

    template<typename Archive> void serialize(Archive& archive, glm::quat& q)
    {
        archive(cereal::make_nvp("x", q.x), cereal::make_nvp("y", q.y), cereal::make_nvp("z", q.z), cereal::make_nvp("w", q.w));
    }
}