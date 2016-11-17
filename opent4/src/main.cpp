#include <vector>
#include <string>

#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>

#include <osgViewer/ViewerEventHandlers>

#include "loader/Turok4.h"

osgViewer::Viewer* SetupViewer() {
    osgViewer::Viewer* viewer = new osgViewer::Viewer;

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 1000;
    traits->y = 500;
    traits->width = 1920;
    traits->height = 1080;
    traits->windowName = "OSG Test";
    traits->red = 8;
    traits->green = 8;
    traits->blue = 8;
    traits->alpha = 0; // set to 0 to stop ScreenCaptureHandler reading the alpha channel
    traits->depth = 24;
    traits->stencil = 8;
    traits->vsync = false;
    traits->doubleBuffer = true;
    traits->windowDecoration = true;
    traits->screenNum = 0;

    osg::ref_ptr<osg::GraphicsContext> context = osg::GraphicsContext::createGraphicsContext(traits);

    osg::ref_ptr<osg::Camera> camera = viewer->getCamera();
    camera->setGraphicsContext(context);
    camera->setViewport(0, 0, traits->width, traits->height);
    return viewer;
}


osgViewer::Viewer* viewer;
osg::Group* root;
opent4::ATRFile* atr;

//std::vector<std::vector<osg::Geode*>> ActorMeshes;
//std::vector<std::string> ActorFiles;
void TurokToOsg(opent4::Actor* actor)
{
    std::string fn = actor->GetFilename();
    opent4::ActorMesh* m = actor->GetMesh();
    if(!m) return;
    printf("Has mesh! %d submeshes\n", m->GetSubMeshCount());

    /*int ActorIdx = -1;
    if(actor->GetDef())
    {
        for(size_t i = 0; i < ActorFiles.size(); i++)
        {
            if(ActorFiles[i].length() != fn.length()) continue;
            if(ACtorFiles[i] != fn) continue;
            ActorIdx = i;
            break;
        }
    }*/

    for(size_t i = 0; i < m->GetSubMeshCount(); i++)
    {
        std::string texFileName;
        if(i < m->m_MeshInfos.size())
        {
            if(m->m_MeshInfos[i].TSNR_ID != -1)
            {
                int TexID = m->m_TXSTs[m->m_TSNRs[m->m_MeshInfos[i].TSNR_ID].TXST_ID].TextureID;
                if(TexID < m->m_Textures.size()) texFileName = m->m_Textures[TexID];
            }
        }

        opent4::SubMesh* sm = m->GetSubMesh(i);
        if(sm->GetVertexCount() != 0)
        {
            osg::Vec3Array* meshVerts = new osg::Vec3Array;
            osg::Vec3Array* meshNorms = new osg::Vec3Array;
            osg::Vec4Array* meshColors = new osg::Vec4Array;
            osg::Vec2Array* meshTexCs = new osg::Vec2Array;
            osg::DrawElementsUInt* meshIndices = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_STRIP, 0);

            for(size_t vIdx = 0; vIdx < sm->GetVertexCount(); vIdx++)
            {
                opent4::ActorVec3 v0, n0;
                osg::Vec2 t0;
                sm->GetTexCoord(vIdx, &t0._v[0]);
                sm->GetVertex(vIdx,&v0.x);
                sm->GetNormal(vIdx,&n0.x);

                meshTexCs->push_back(t0);
                meshVerts->push_back(osg::Vec3(v0.x,v0.z,v0.y));
                meshNorms->push_back(osg::Vec3(n0.x,n0.z,n0.y));
                meshColors->push_back(osg::Vec4(1,1,1,1));
            }

            if(sm->GetIndexCount() != 0)
            {
                for(int idx = 0; idx < sm->GetIndexCount(); idx++)
                {
                    int i = sm->GetIndex(idx);
                    meshIndices->push_back(i);
                }

                osg::Geode* meshGeode = new osg::Geode();
                osg::Geometry* meshGeometry = new osg::Geometry();
                meshGeode->addDrawable(meshGeometry);

                meshGeometry->setTexCoordArray(0, meshTexCs);
                meshGeometry->setVertexArray(meshVerts);
                meshGeometry->setNormalArray(meshNorms);
                meshGeometry->setColorArray(meshColors);
                meshGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
                meshGeometry->addPrimitiveSet(meshIndices);
                printf("Adding geometry\n");

                //Setup obj transform
                osg::Vec3 meshPosition;
                if(actor->GetDef())
                {
                    opent4::ActorVec3 pos = actor->GetDef()->Position;
                    meshPosition += osg::Vec3(pos.x, pos.z, pos.y);
                }
                osg::PositionAttitudeTransform* meshXForm = new osg::PositionAttitudeTransform();

                meshXForm->addChild(meshGeode);
                root->addChild(meshXForm);

                if(texFileName.length() > 0)
                {
                    osg::Texture2D* meshTexture = new osg::Texture2D;
                    meshTexture->setDataVariance(osg::Object::DYNAMIC);
                    osg::Image* meshImage = osgDB::readImageFile(texFileName);
                    if(!meshImage)
                    {
                        printf("OSG Failed loading image %s\n", texFileName.c_str());
                    }
                    meshTexture->setImage(meshImage);
                    osg::StateSet* stateOne = new osg::StateSet();
                    stateOne->setTextureAttributeAndModes(0, meshTexture, osg::StateAttribute::ON);
                    meshGeode->setStateSet(stateOne);
                }
            }
        }
    }
}



void addSceneData()
{
    if(!root || !atr) return;

    opent4::ATIFile* ATI = atr->GetActors();
    std::vector<opent4::Actor*> ActorDefs;
    ActorDefs.push_back(atr->GetActor());
    if(ATI)
    {
        for(int i = 0; i < ATI->GetActorCount(); i++) ActorDefs.push_back(ATI->GetActorDef(i)->Actor->GetActor());
    } else {
        printf("Not a level ATR file.\n");
    }

    for(size_t i = 0; i < ActorDefs.size(); i++){
        TurokToOsg(ActorDefs[i]);
    }
}

void RunOsg()
{
    viewer = SetupViewer();
    root = new osg::Group();
    addSceneData();

    //Add stat handler
    osg::ref_ptr<osgViewer::StatsHandler> statsHandler = new osgViewer::StatsHandler;
    statsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_F3);
    viewer->addEventHandler(statsHandler);

    //Set state attributes
    osg::PolygonMode * polygonMode = new osg::PolygonMode;
    polygonMode->setMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
    //root->getOrCreateStateSet()->setAttributeAndModes(polygonMode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    //Setup viewer
    viewer->setSceneData( root );
    viewer->setCameraManipulator(new osgGA::TrackballManipulator());
    viewer->realize();

    osg::Timer frameTimer;
    double simulationTime = 0.0;
    while( !viewer->done() )
    {
        double dt = frameTimer.time_s();
        frameTimer.setStartTick();
        dt = std::min(dt, 0.2);
        simulationTime += dt;

        viewer->advance(simulationTime);
        viewer->eventTraversal();
        viewer->updateTraversal();
        viewer->renderingTraversals();
    }
}

int main(int ArgC,const char* ArgV[])
{
    opent4::SetTurokDirectory("/home/djdduty/dev/projects/TUROK");
    atr = new opent4::ATRFile();
    if(!atr->Load("test.atr")) {
        delete atr;
        return -1;
    }

    RunOsg();
    delete atr;
    delete viewer;
    return 0;
}

