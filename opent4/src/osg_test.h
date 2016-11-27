#ifndef OSG_TEST_H
#define OSG_TEST_H

#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>

#include "util/array.h"

using namespace opent4::util;

struct PyramidComponent
{
    uint32_t i;
};

class PyramidComponentManager {
    public:
        PyramidComponentManager() : _setup(false) {}

        void addToScene(osg::Group* parent)
        {
            parent->addChild(osgRoot);
        }

        PyramidComponent create(float x, float y, float z)
        {
            setup_geometry();
            osg::Geode* geode = new osg::Geode();
            osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform();
            transform->setPosition(osg::Vec3(x,y,z));

            geode->addDrawable(_pyramidGeometry);
            transform->addChild(geode);
            osgRoot->addChild(transform);

            _data.geode.push_back(geode);
            _data.transform.push_back(transform);
            uint32_t idx = _data.geode.size()-1;
            return make_instance(idx);
        }

        void destroy(PyramidComponent instance)
        {
            osgRoot->removeChild(_data.transform[instance.i]);
        }

        PyramidComponent make_instance(uint32_t i)
        {
            PyramidComponent inst = {i};
            return inst;
        }

        void gc()
        {

        }

    protected:
        void setup_geometry()
        {
            if(_setup) return;
            _setup = true;
            osgRoot = new osg::Group;
            _pyramidGeometry = new osg::Geometry();

            osg::Vec3Array *pyramidVertices = new osg::Vec3Array;
            pyramidVertices->push_back(osg::Vec3( 0, 0, 0)); // front left;
            pyramidVertices->push_back(osg::Vec3(10, 0, 0)); // front right;
            pyramidVertices->push_back(osg::Vec3(10,10, 0)); // back right;
            pyramidVertices->push_back(osg::Vec3( 0,10, 0)); // back left;
            pyramidVertices->push_back(osg::Vec3( 5, 5,10)); // peak
            _pyramidGeometry->setVertexArray(pyramidVertices);

            osg::DrawElementsUInt *pyramidBase = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
            pyramidBase->push_back(3);
            pyramidBase->push_back(2);
            pyramidBase->push_back(1);
            pyramidBase->push_back(0);
            _pyramidGeometry->addPrimitiveSet(pyramidBase);

            osg::DrawElementsUInt* pyramidFaces = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
            pyramidFaces->push_back(0);
            pyramidFaces->push_back(1);
            pyramidFaces->push_back(4);

            pyramidFaces->push_back(1);
            pyramidFaces->push_back(2);
            pyramidFaces->push_back(4);

            pyramidFaces->push_back(2);
            pyramidFaces->push_back(3);
            pyramidFaces->push_back(4);

            pyramidFaces->push_back(3);
            pyramidFaces->push_back(0);
            pyramidFaces->push_back(4);
            _pyramidGeometry->addPrimitiveSet(pyramidFaces);
            osg::Vec4Array* colors = new osg::Vec4Array;
            colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 0 red
            colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) ); //index 1 green
            colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) ); //index 2 blue
            colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 3 white
            colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 4 red
            _pyramidGeometry->setColorArray(colors);
            _pyramidGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
        }

        struct PyramidInstanceData {
            Array<osg::Geode*> geode;
            Array<osg::PositionAttitudeTransform*> transform;
        };

        PyramidInstanceData _data;
        osg::Geometry *_pyramidGeometry;
        osg::Group *osgRoot;
        bool _setup;
};

#endif
