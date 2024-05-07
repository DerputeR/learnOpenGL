#include "Node.h"
#include <iostream>

Node::Node(std::string name) : parent { nullptr }
{
	this->name = name;
}

Node::Node(std::string name, Node& parent) : parent { &parent }
{
	this->name = name;
	this->parent = &parent;
	// is this even legal??
	parent.children.push_back(std::unique_ptr<Node>(this));
}

Node::~Node()
{
	std::cout << "Node \"" << this->name << "\" destroyed." << std::endl;
}

Node* Node::addChild(std::unique_ptr<Node>& child)
{
	if (child->parent != nullptr)
	{
		if (child->parent == this)
		{
			return this;
		}
		// gross
		auto& parentChildren = child->parent->children;
		auto found = std::find(parentChildren.begin(), parentChildren.end(), child);
		if (found != parentChildren.end())
		{
			parentChildren.erase(found);
		}
	}
	child->parent = this;
	this->children.push_back(std::move(child));

	return nullptr;
}

Node* Node::setParent(Node* parent)
{
	return nullptr;
}

Node* Node::getParent()
{
	return nullptr;
}

static void printTreeAtNode(const Node& node, int depth)
{
	std::ostringstream
}

void Node::printLocalTree()
{

}

void Node::printTree()
{

}
