#include "Skeleton.h"

Skeleton::Skeleton(SPGObject spGObject)
	: GObject(nullptr)
    , m_spGObject(spGObject)
{}

void Skeleton::render(const D3DXMATRIX* preTransform)
{
    m_spGObject->render(preTransform);
	for (auto child : m_children)
	{
        child->render(m_spGObject->getTransformMatrix());
	}
}

void Skeleton::addChild(SPSkeleton child)
{
	m_children.push_back(child);
}