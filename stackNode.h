// Description: A stack node.
// Author: Wheeler Law

#ifndef STACK_NODE_H
#define STACK_NODE_H

// Represents a single node in the stack
typedef struct StackNode {
	char* data;		// data associated with the node	
	struct StackNode* next;	// pointer to next node (NULL if none)
} StackNode;

/// Dynamically allocate a new stack node
/// @param data The data value
/// @param next Pointer to next node (NULL if none)
/// @return The new stack node
StackNode* makeStackNode(char* data, StackNode* next);

#endif

