#include "SceneEndcredit.h"

SceneEndCredit::SceneEndCredit(/* args */)
{
    // CODE
    texQuadEndCreditSlide_1 = new TexturedQuad();
    texQuadEndCreditSlide_1->initialize("resources\\textures\\EndCreditSlide_1.png");

    texQuadEndCreditSlide_2 = new TexturedQuad();
    texQuadEndCreditSlide_2->initialize("resources\\textures\\EndCreditSlide_2.png");
    
    texQuadBirthdaySlide_1 = new TexturedQuad();
    texQuadBirthdaySlide_1->initialize("resources\\textures\\birthday_slide.png");

    sdkCreateTimer(&timer);
}

SceneEndCredit::~SceneEndCredit()
{
    if (texQuadBirthdaySlide_1)
    {
        delete texQuadBirthdaySlide_1;
        texQuadBirthdaySlide_1 = nullptr;
    }
    if (texQuadEndCreditSlide_2)
    {
        delete texQuadEndCreditSlide_2;
        texQuadEndCreditSlide_2 = nullptr;
    }

    if (texQuadEndCreditSlide_1)
    {
        delete texQuadEndCreditSlide_1;
        texQuadEndCreditSlide_1 = nullptr;
    }
}

void SceneEndCredit::initialCommandBuffer(VkCommandBuffer &commandBuffer)
{
    texQuadEndCreditSlide_1->buildCommandBuffers(commandBuffer);
    texQuadEndCreditSlide_2->buildCommandBuffers(commandBuffer);
    texQuadBirthdaySlide_1->buildCommandBuffers(commandBuffer);
}

void SceneEndCredit::update(void)
{
    MVP_UniformData mvp_UniformData;

    elapsed_time = sdkGetTimerValue(&timer);
    elapsed_time = elapsed_time / 1000.0f;

    if (elapsed_time >= TSM::SCENE_ENDCREDITS_TIME)
    {
        completed = true;
    }

    const float yAxis_start_point = -12.0f;
    static float xPos = 0.0f;
    static float yPos = yAxis_start_point;


    if (yPos < -yAxis_start_point)
        yPos += 0.00005f;
    
    if (texQuadBirthdaySlide_1 && yPos < 5)
    {
        memset((void*)&mvp_UniformData, 0, sizeof(MVP_UniformData));
        mvp_UniformData.modelMatrix = glm::mat4(1.0f);
        mvp_UniformData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos+8.0f, -5.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(3.5f, 1.75f, 1.0f));
        mvp_UniformData.viewMatrix = glm::mat4(1.0f);

        texQuadBirthdaySlide_1->update(mvp_UniformData);
    }

    if (texQuadEndCreditSlide_1 && yPos < 8)
    {   
        memset((void*)&mvp_UniformData, 0, sizeof(MVP_UniformData));
        mvp_UniformData.modelMatrix = glm::mat4(1.0f);
        mvp_UniformData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos + 4.0f, -5.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(3.5f, 1.75f, 1.0f));
        mvp_UniformData.viewMatrix = glm::mat4(1.0f);

        texQuadEndCreditSlide_1->update(mvp_UniformData);
    }

    if (texQuadEndCreditSlide_2 && yPos < 10)
    {
        memset((void*)&mvp_UniformData, 0, sizeof(MVP_UniformData));
        mvp_UniformData.modelMatrix = glm::mat4(1.0f);
        mvp_UniformData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, -5.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(3.5f, 1.75f, 1.0f));
        mvp_UniformData.viewMatrix = glm::mat4(1.0f);

        texQuadEndCreditSlide_2->update(mvp_UniformData);
    }
 
}

void SceneEndCredit::onResize(int width, int height)
{
    texQuadEndCreditSlide_1->resize(width, height);

    texQuadEndCreditSlide_2->resize(width, height);

    texQuadBirthdaySlide_1->resize(width, height);
}
