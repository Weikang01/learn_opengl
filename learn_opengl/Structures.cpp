#include "Structures.h"

void Material::setAsUniform(Shader& shader)
{
	shader.set3fv(  (name + ".ambient"  ).c_str(), ambient  );
	shader.set3fv(  (name + ".diffuse"  ).c_str(), diffuse  );
	shader.set3fv(  (name + ".specular" ).c_str(), specular );
	shader.setFloat((name + ".shininess").c_str(), shininess);
}

#pragma region light
void Light::setAsUniform(Shader& shader)
{
	//shader.set3fv("light.direction", glm::value_ptr(this->direction));
	shader.set3fv("light.position"      ,position   );
	shader.set3fv("light.ambient"       ,ambient    );
	shader.set3fv("light.diffuse"       ,diffuse    );
	shader.set3fv("light.specular"      ,specular   );
	shader.set3fv("light.front"         ,front      );
	shader.setFloat("light.cutOff"      ,cutOff     );
	shader.setFloat("light.outerCutOff" ,outerCutOff);
	shader.setFloat("light.constant"    ,constant   );
	shader.setFloat("light.linear"      ,linear     );
	shader.setFloat("light.quadratic"   ,quadratic  );
}
void Light::setPositionAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv("light.position", value);
}
void Light::setAmbientAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv("light.ambient", value);
}
void Light::setDiffuseAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv("light.diffuse", value);
}
void Light::setSpecularAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv("light.specular", value);
}
void Light::setFrontAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv("light.front", value);
}
void Light::setAttenuationCoefficients(Shader& shader, float constant, float linear, float quadratic)
{
	shader.setFloat("light.constant" , constant );
	shader.setFloat("light.linear"   , linear   );
	shader.setFloat("light.quadratic", quadratic);
}
#pragma endregion

#pragma region dirLight
void DirLight::setAsUniform(Shader& shader)
{
	shader.set3fv((name + ".direction").c_str(), direction);
	shader.set3fv((name + ".ambient"  ).c_str(), ambient  );
	shader.set3fv((name + ".diffuse"  ).c_str(), diffuse  );
	shader.set3fv((name + ".specular" ).c_str(), specular );
}
void DirLight::setDirectionAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".direction").c_str(), value);
}
void DirLight::setAmbientAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".ambient").c_str(), value);
}
void DirLight::setDiffuseAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv("light.diffuse", value);
}
void DirLight::setSpecularAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv("light.specular", value);
}
#pragma endregion

#pragma region pointLight
void PointLight::setAsUniform(Shader& shader)
{
	//shader.set3fv("light.direction", glm::value_ptr(this->direction));
	shader.set3fv(  (name + ".position" ).c_str(), position );
	shader.set3fv(  (name + ".ambient"  ).c_str(), ambient  );
	shader.set3fv(  (name + ".diffuse"  ).c_str(), diffuse  );
	shader.set3fv(  (name + ".specular" ).c_str(), specular );
	shader.setFloat((name + ".constant" ).c_str(), constant );
	shader.setFloat((name + ".linear"   ).c_str(), linear   );
	shader.setFloat((name + ".quadratic").c_str(), quadratic);
}
void PointLight::setPositionAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".position").c_str(), value);
}
void PointLight::setAmbientAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".ambient").c_str(), value);
}
void PointLight::setDiffuseAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".diffuse").c_str(), value);
}
void PointLight::setSpecularAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".specular").c_str(), value);
}
void PointLight::setAttenuationCoefficients(Shader& shader, float constant, float linear, float quadratic)
{
	shader.setFloat((name + ".constant" ).c_str(), constant );
	shader.setFloat((name + ".linear"   ).c_str(), linear   );
	shader.setFloat((name + ".quadratic").c_str(), quadratic);
}
#pragma endregion

#pragma region spotLight
void SpotLight::setAsUniform(Shader& shader)
{
	//shader.set3fv("light.direction", glm::value_ptr(this->direction));
	shader.set3fv(  (name + ".position"   ).c_str(), position   );
	shader.set3fv(  (name + ".ambient"    ).c_str(), ambient    );
	shader.set3fv(  (name + ".diffuse"    ).c_str(), diffuse    );
	shader.set3fv(  (name + ".specular"   ).c_str(), specular   );
	shader.set3fv(  (name + ".front"      ).c_str(), front      );
	shader.setFloat((name + ".cutOff"     ).c_str(), cutOff     );
	shader.setFloat((name + ".outerCutOff").c_str(), outerCutOff);
	shader.setFloat((name + ".constant"   ).c_str(), constant   );
	shader.setFloat((name + ".linear"     ).c_str(), linear     );
	shader.setFloat((name + ".quadratic"  ).c_str(), quadratic  );
}
void SpotLight::setPositionAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".position").c_str(), value);
}
void SpotLight::setAmbientAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".ambient").c_str(), value);
}
void SpotLight::setDiffuseAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".diffuse").c_str(), value);
}
void SpotLight::setSpecularAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".specular").c_str(), value);
}
void SpotLight::setFrontAsUniform(Shader& shader, const glm::vec3& value)
{
	shader.set3fv((name + ".front").c_str(), value);
}
void SpotLight::setAttenuationCoefficients(Shader& shader, float constant, float linear, float quadratic)
{
	shader.setFloat((name + ".constant").c_str(), constant);
	shader.setFloat((name + ".linear").c_str(), linear);
	shader.setFloat((name + ".quadratic").c_str(), quadratic);
}

#pragma endregion