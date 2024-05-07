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
	parent.children.push_back(this);
}

Node::~Node()
{
	std::cout << "Node \"" << this->name << "\" destroyed." << std::endl;
}

Node* Node::addChild(Node& child)
{
	if (child.parent != nullptr)
	{
		if (child.parent == this)
		{
			return this;
		}
		// gross
		auto& parentChildren = child.parent->children;
		auto found = std::find(parentChildren.begin(), parentChildren.end(), &child);
		if (found != parentChildren.end())
		{
			parentChildren.erase(found);
		}
	}
	child.parent = this;
	this->children.push_back(&child);

	return nullptr;
}

Node* Node::setParent(Node* parent)
{
	Node* oldParent = this->parent;
	if (oldParent != nullptr)
	{
		auto& oldParentChildren = oldParent->children;
		auto found = std::find(oldParentChildren.begin(), oldParentChildren.end(), this);
		if (found != oldParentChildren.end())
		{
			oldParentChildren.erase(found);
		}
	}
	this->parent = parent;
	parent->children.push_back(this);
	return oldParent;
}

Node* Node::getParent() const
{
	return this->parent;
}

std::vector<Node*> Node::getChildren() const
{
	return this->children;
}

void Node::printTreeAtNode(const Node& node, size_t depth) const
{
	std::string indent;
	indent.reserve(4 * depth);
	for (int i = 0; i < depth; i++)
	{
		indent += "    ";
	}
	std::cout << indent << node.name << std::endl;
	for (auto child : node.children)
	{
		if (child == nullptr)
		{
			std::cout << indent << "    " << "(ILLEGAL NULL NODE)" << std::endl;
			continue;
		}
		printTreeAtNode(*child, depth + 1);
	}
}

void Node::printLocalTree() const
{
	printTreeAtNode(*this, 0);
}

void Node::printTree() const
{
	const Node* root = this;
	while (root->parent != nullptr)
	{
		root = root->parent;
	}
	printTreeAtNode(*root, 0);
}
