#include "osg_wrapper.h"

using namespace opent4;

void ImGui_ImplOsg_CreateRenderObjects()
{
    /* Create Font Texture
     * Create Drawable, with that texture
     * Enable GL_BLEND
     * blendEquation GL_FUNC_ADD
     * blendFunc GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
     * disable GL_CULL_FACE
     * enable GL_SCISSOR_TEST
     */

    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->setImage(width, height, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, pixels, osg::Image::NO_DELETE);
}
