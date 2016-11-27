#include <scene/Entity.h>
#include <chrono>
#include <set>

#include <unordered_map>

#include "t4test.h"
#include "osg_test.h"

void RunOsgTest()
{
    viewer = SetupViewer();
    root = new osg::Group();

    //Add stat handler
    osg::ref_ptr<osgViewer::StatsHandler> statsHandler = new osgViewer::StatsHandler;
    statsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_F3);
    viewer->addEventHandler(statsHandler);

    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    PyramidComponentManager pm;
    PyramidComponent pc = pm.create(0,0,0);
    PyramidComponent pc2 = pm.create(15,0,0);

    pm.addToScene(root);

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

unsigned int get_now() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

int main(int ArgC,const char* ArgV[])
{
    opent4::scene::EntityManager em;

    const uint32_t NUM_ENTITIES = 512;
    opent4::scene::Entity entities[NUM_ENTITIES];
    float before_create = get_now();
    for(int i = 0; i < NUM_ENTITIES; i++)
    {
        entities[i] = em.create();
        printf("Created entity %d, gen %d\n", entities[i].index(), entities[i].generation());
    }
    float after_create = get_now();

    //em.destroy(entities[NUM_ENTITIES-1]);
    //entities[NUM_ENTITIES-1] = em.create();

    float before_alive = get_now();
    uint32_t count_alive = 0;
    for(int i = 0; i < NUM_ENTITIES; i++)
    {
        opent4::scene::Entity e = entities[i];
        count_alive++;
        if(em.alive(e)) {
            //printf("Entity w/ idx: %d generation: %d is ALIVE!\n", e.index(), e.generation());
        } else {
            //printf("Entity w/ idx: %d generation: %d is DEAD!\n", e.index(), e.generation());
        }
    }
    float after_alive = get_now();

    printf("Create took %f ns\n", (after_create-before_create));
    printf("Checking alive status of %d entities took %f ns\n", count_alive, (after_alive-before_alive));
    printf("Total time taken %f ns\n", (after_alive-before_create));

    RunOsgTest();
    return 0;
}

