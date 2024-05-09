#pragma once
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Class to hold nodes in a node tree.
 * NOTE: figure out how to optimally use smart pointers with this
 * As of now, nodes are NOT owned by each other. It is assumed that they are on the heap
 * Nodes are mostly generic objects and don't do anything on their own.
 * Maybe we can turn this into a simplified component-based system, or just subclass the daylights out of it.
 */
class Node
{
	Node* parent;
	std::vector<Node*> children;
	static void printTreeAtNode(const Node& node, size_t depth);

public:
	std::string name;

	/**
	 * @brief Destructor.
	 * Currently prints out that this node was destroyed.
	 */
	~Node();

	/**
	 * @brief Creates a node without a parent
	 * @param name - Name of this node
	 */
	Node(const std::string& name);

	/**
	 * @brief Creates a node with a parent
	 * @param name - Name of this node
	 * @param parent - Parent node that should own this child.
	 * The parent node will automatically add this new node
	 * to its vector of children.
	 */
	Node(const std::string& name, Node& parent);

	/**
	 * @brief Adds a child node to this node.
	 * If the child node is already a child of another node that
	 * is NOT this node, the child node is removed from its parent
	 * before being reassigned to the this node.
	 * @param child 
	 * @return Previous parent node of `child`, if it had one.
	 */
	Node* addChild(Node& child);

	/**
	 * @brief Sets the parent node of this node.
	 * If this node already has a parent, it will be removed from its children vector
	 * before being assigned a new parent.
	 * This node will be added to the new parent's children vector if it is non-null.
	 * @param parent - can be null to remove parent node.
	 * @return Previous parent node if one was previously assigned.
	 */
	Node* setParent(Node* parent);

	/**
	 * @brief Gets the parent node of this node.
	 * If this node does not have a parent, a nullptr will be returned
	 * @return Parent node
	 */
	Node* getParent() const;

	/**
	 * @brief Removes this node from its parent.
	 * Does not destroy the node or its children.
	 * @return The removed node
	 */
	Node* removeFromParent();


	std::vector<Node*> getChildren() const;

	/**
	 * @brief Prints the structure of this node tree using this node
	 * as the root. 
	 */
	void printLocalTree() const;

	/**
	 * @brief Prints the structure of this node tree using the
	 * top-most node as the root.
	 */
	void printTree() const;
};

