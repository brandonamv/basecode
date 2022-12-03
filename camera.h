#pragma once
class camera {
public:
    camera(glm::vec3 pos, glm::vec3 center, glm::vec3 up, glm::vec3 left);
    
    auto getView();

    void setcameraSpeed(float s);
    float getcameraSpeed();

    glm::vec3 getcameraFront();

    void move_front();
    void move_left();
    void move_right();
    void move_back();
    void move_up();
    void move_down();

private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraLeft;
    glm::vec3 cameraUp;
    float cameraSpeed = 0.01f;
    float  yAngle= -90.0f;
    float xAngle = 0.0f;

    void update();
};

inline camera::camera(glm::vec3 pos, glm::vec3 center, glm::vec3 up, glm::vec3 left) {
    cameraPos = pos;
    cameraFront = center;
    cameraUp = up;
    cameraLeft = left;
}

inline auto camera::getView() {
    return glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
}

inline void camera::setcameraSpeed(float s) {
    cameraSpeed = s;
}
inline float camera::getcameraSpeed() {
    return cameraSpeed;
}

inline void camera::move_front() {
    cameraPos += cameraSpeed * cameraFront;
}
inline void camera::move_left() {
    cameraFront += glm::normalize(glm::cross(cameraPos, cameraUp)) * cameraSpeed;
    cameraLeft = glm::normalize(glm::cross(cameraFront, cameraUp));
}
inline void camera::move_right() {
    cameraFront += glm::normalize(glm::cross( cameraUp,cameraPos)) * cameraSpeed;
    cameraLeft = glm::normalize(glm::cross(cameraFront, cameraUp));
}
inline void camera::move_up() {
    cameraFront += glm::normalize(glm::cross(cameraPos, cameraLeft)) * cameraSpeed;
    cameraUp = glm::normalize(glm::cross(cameraLeft, cameraFront));
}
inline void camera::move_down() {
    cameraFront += glm::normalize(glm::cross(cameraLeft,cameraPos)) * cameraSpeed;
    cameraUp = glm::normalize(glm::cross(cameraLeft, cameraFront));

}
inline void camera::move_back() {
    cameraPos -= cameraSpeed * cameraFront;
}

inline glm::vec3 camera::getcameraFront() {
    return cameraPos + cameraFront;
}

inline void camera::update() {
    glm::vec3 front;
    front.x = cos(xAngle) * cos(yAngle);
    front.y = sin(yAngle);
    front.z = sin(xAngle) * cos(yAngle);
    cameraFront = glm::normalize(front);
    cameraLeft = glm::normalize(glm::cross(cameraFront, cameraUp));
    cameraUp= glm::normalize(glm::cross(cameraLeft, cameraFront));
}