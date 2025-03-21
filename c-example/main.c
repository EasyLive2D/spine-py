#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spine/spine.h>
#include "spine-opengl.h"


int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 500, "spine opengl", NULL, NULL);

    glfwMakeContextCurrent(window);

    gladLoadGL();

    spAtlas *atlas = spAtlas_createFromFile("../../data/mix-and-match/mix-and-match-pma.atlas", NULL);
	spSkeletonJson *json = spSkeletonJson_create(atlas);
	json->scale = 1.0f;
	spSkeletonData *skeletonData = spSkeletonJson_readSkeletonDataFile(json, "../../data/mix-and-match/mix-and-match-pro.json");

	spAnimationStateData *animationStateData = spAnimationStateData_create(skeletonData);
	animationStateData->defaultMix = 0.2f;

    // create
    spSkeletonDrawable *drawable = spSkeletonDrawable_create(skeletonData, animationStateData);

    // setSkin
    spSkin* skin = spSkeletonData_findSkin(skeletonData, "full-skins/girl-spring-dress");
    spSkeleton_setSkin(drawable->skeleton, skin);
	spSkeleton_setToSetupPose(drawable->skeleton);


	drawable->usePremultipliedAlpha = -1;
	drawable->skeleton->x = 400;
	drawable->skeleton->y = 500;
    // drawable->skeleton->scaleX = 0.5f;
    // drawable->skeleton->scaleY = 0.5f;
    drawable->canvasWidth = 800;
    drawable->canvasHeight = 500;

	spSkeletonDrawable_update(drawable, 0, SP_PHYSICS_UPDATE);
	spAnimationState_setAnimationByName(drawable->animationState, 0, "idle", 0);
	spAnimationState_addAnimationByName(drawable->animationState, 0, "dance", -1, 0);


	double lastFrameTime = glfwGetTime();
    double currentFrameTime, deltaTime;
    int windowWidth, windowHeight;
    // glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        spSkeletonDrawable_update(drawable, deltaTime, SP_PHYSICS_UPDATE);
        spSkeletonDrawable_draw(drawable);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}