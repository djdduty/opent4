#ifndef IMGUI_IMPLOSG_H
#define IMGUI_IMPLOSG_H

#include <osgViewer/ViewerEventHandlers>
#include <osg/Camera>

#include "imgui.h"

namespace opent4
{
    //Create new osg::Drawable?
    struct GuiCallback
    {
        virtual void operator() () {}
    };

    class imGuiHandler : public osgGA::GUIEventHandler
    {
        public:
            imGuiHandler(GuiCallback* renderGui);
            void init();
            void newFrame(osg::RenderInfo& renderInfo);
            void render(osg::RenderInfo& renderInfo);
            void setCameraCallbacks(osg::Camera* camera);

            virtual bool handle(const osgGA::GUIEventAdapter& eventAdapter,
                                osgGA::GUIActionAdapter& actionMapper,
                                osg::Object* object, osg::NodeVisitor* visitor);

        private:
            GuiCallback* m_Callback;
            double m_Time;
            bool m_MousePressed[3];
            float m_MouseWheel;
            GLuint g_FontTexture;
    };
}

#endif
