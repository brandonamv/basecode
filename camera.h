#pragma once
class camera {
public:
    camera(glm::vec3 pos, glm::vec3 center, glm::vec3 up, glm::vec3 left);
    
    auto getView();

    void setcameraSpeed(float s, GLfloat delta);
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
    float cameraSpeed = 0.1f;
    float  yAngle= -90.0f;
    float xAngle = 0.0f;
    float deltaTime;
    bool godMode = false;

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

inline void camera::setcameraSpeed(float s,GLfloat delta) {
    cameraSpeed = s;
    deltaTime = delta;
}
inline float camera::getcameraSpeed() {
    return cameraSpeed;
}

inline void camera::move_front() {
    godMode?
        cameraPos += deltaTime * cameraFront * cameraSpeed:
        cameraPos += deltaTime * glm::vec3(0.0f, 0.0f, -1.0f) * cameraSpeed;
}
inline void camera::move_left() {
    //xAngle -= delta;
    glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), deltaTime * cameraSpeed, cameraUp);
    cameraFront = glm::mat3(rotator) * cameraFront;
}
inline void camera::move_right() {
    //xAngle += delta;
    glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), -deltaTime * cameraSpeed, cameraUp);
    cameraFront = glm::mat3(rotator) * cameraFront;
}
inline void camera::move_up() {
    /*cameraFront += glm::normalize(glm::cross(cameraPos, cameraLeft)) * cameraSpeed;
    cameraUp = glm::normalize(glm::cross(cameraLeft, cameraFront));*/
    glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), deltaTime * cameraSpeed, cameraLeft);
    cameraFront = glm::mat3(rotator) * cameraFront;
}
inline void camera::move_down() {
    /*cameraFront += glm::normalize(glm::cross(cameraLeft,cameraPos)) * cameraSpeed;
    cameraUp = glm::normalize(glm::cross(cameraLeft, cameraFront));*/
    glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), -deltaTime * cameraSpeed, cameraLeft);
    cameraFront = glm::mat3(rotator) * cameraFront;

}
inline void camera::move_back() {
    godMode ?
        cameraPos -= deltaTime * cameraFront * cameraSpeed :
        cameraPos -= deltaTime * glm::vec3(0.0f, 0.0f, -1.0f) * cameraSpeed;
}

inline glm::vec3 camera::getcameraFront() {
    return cameraPos + cameraFront;
}

//inline void camera::update(float delta) {
//
//    deltaTime = delta;
//    
//    // Create camera transformations
//    //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//    /*glm::vec3 front;
//    front.x = cos(xAngle) * cos(yAngle);
//    front.y = sin(yAngle);
//    front.z = sin(xAngle) * cos(yAngle);
//    cameraFront = glm::normalize(front);
//    cameraLeft = glm::normalize(glm::cross(cameraFront, cameraUp));
//    cameraUp= glm::normalize(glm::cross(cameraLeft, cameraFront));*/
//}