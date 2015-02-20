#pragma once

#include "GObject.h"

#include <vector>

class Skeleton;
typedef Skeleton *PSkeleton;
typedef std::shared_ptr<Skeleton> SPSkeleton;

class Skeleton : public GObject {
public:
    Skeleton(SPGObject spGObject);
    ~Skeleton() {}

    void render(const D3DXMATRIX* preTransform = nullptr);
    void addChild(SPSkeleton child);
    void draw() {}
    D3DXVECTOR3 getCenter() const {
        return m_spGObject->getCenter();
    }

protected:
    SPGObject m_spGObject;
    std::vector<SPSkeleton> m_children;
};