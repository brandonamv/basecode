#pragma once
class camera {
public:
    camera(glm::vec3 pos, glm::vec3 center, glm::vec3 up);
    
    auto getView();

    void setcameraSpeed(float s, GLfloat delta);
    float getcameraSpeed();

    glm::vec3 getcameraFront();

    void setGodMode(bool active);

    void move_front();
    void move_left();
    void move_right();
    void move_back();
    void move_up();
    void move_down();

private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float cameraSpeed = 0.1f;
    float deltaTime;
    bool godMode = false;

};

inline camera::camera(glm::vec3 pos, glm::vec3 center, glm::vec3 up) {
    cameraPos = pos;
    cameraFront = center;
    cameraUp = up;
}

inline auto camera::getView() {
    return glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
}

inline void camera::setcameraSpeed(float s,GLfloat delta) {
    cameraSpeed = s;
    deltaTime = delta;
}
inline float camera::getcameraSpeed() {
    return cameraSpeed;
}

inline void camera::move_front()
{
    godMode ?
        cameraPos += deltaTime * cameraFront * cameraSpeed :
        cameraPos += deltaTime * glm::vec3(cameraFront.x, 0.0f, cameraFront.z) * cameraSpeed;
}
inline void camera::move_back() {
    godMode ?
        cameraPos -= deltaTime * cameraFront * cameraSpeed :
        cameraPos -= deltaTime * glm::vec3(cameraFront.x, 0.0f, cameraFront.z) * cameraSpeed;
}
inline void camera::move_left() {
    glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), deltaTime * cameraSpeed, cameraUp);
    cameraFront = glm::mat3(rotator) * cameraFront;
}
inline void camera::move_right() {
    glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), -deltaTime * cameraSpeed, cameraUp);
    cameraFront = glm::mat3(rotator) * cameraFront;
}
inline void camera::move_up() {
    glm::vec3 left = glm::cross(cameraFront, cameraUp);
    glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), deltaTime * cameraSpeed, left);
    cameraFront = glm::mat3(rotator) * cameraFront;
}
inline void camera::move_down() {
    glm::vec3 left = glm::cross(cameraFront, cameraUp);
    glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), -deltaTime * cameraSpeed, left);
    cameraFront = glm::mat3(rotator) * cameraFront;

}

inline glm::vec3 camera::getcameraFront() {
    return cameraPos+cameraFront;
}

inline void camera::setGodMode(bool active)
{
    godMode = !((active || godMode) && (!active || !godMode));
}


