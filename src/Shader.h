#define GL_SILENCE_DEPRECATION

#ifndef SOLARSIM_SHADER_H
#define SOLARSIM_SHADER_H

#include <string>
#include <glm/glm.hpp>

using namespace std;

class Shader {
private:
    int handle;

public:
    explicit Shader(const string& vertexShaderPath, const string& geometryShaderPath, const string& fragmentShaderPath);

    void use() const;

    void setBool(const std::string& name, bool value) const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setVec2(const std::string& name, const glm::vec2& value) const;

    void setVec2(const std::string& name, float x, float y) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;

    void setVec3(const std::string& name, float x, float y, float z) const;

    void setVec4(const std::string& name, const glm::vec4& value) const;

    void setVec4(const std::string& name, float x, float y, float z, float w) const;

    void setMat2(const std::string& name, const glm::mat2& mat) const;

    void setMat3(const std::string& name, const glm::mat3& mat) const;

    void setMat4(const std::string& name, const glm::mat4& mat) const;
};


#endif //SOLARSIM_SHADER_H
